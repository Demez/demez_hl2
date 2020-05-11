//===== Copyright � 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
//===========================================================================//

#include "cbase.h"
#include "demez_flashlight_base.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// TEMP
static ConVar r_headlight_offset_x( "r_headlight_offset_x", "0.0", FCVAR_CHEAT );
static ConVar r_headlight_offset_y( "r_headlight_offset_y", "0.0", FCVAR_CHEAT );
static ConVar r_headlight_offset_z( "r_headlight_offset_z", "0.0", FCVAR_CHEAT );


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nEntIndex - The m_nEntIndex of the client entity that is creating us.
//			vecPos - The position of the light emitter.
//			vecDir - The direction of the light emission.
//-----------------------------------------------------------------------------
CFlashlightEffect::CFlashlightEffect(int nEntIndex)
{
	m_FlashlightHandle = CLIENTSHADOW_INVALID_HANDLE;
	m_nEntIndex = nEntIndex;

	m_bIsOn = false;

#if !ENGINE_2013
	m_flCurrentPullBackDist = 1.0f;
#endif

	m_pPointLight = NULL;

	if ( g_pMaterialSystemHardwareConfig->SupportsBorderColor() )
	{
		m_FlashlightTexture.Init( "effects/flashlight_border", TEXTURE_GROUP_OTHER, true );
	}
	else
	{
		m_FlashlightTexture.Init( "effects/flashlight001", TEXTURE_GROUP_OTHER, true );
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CFlashlightEffect::~CFlashlightEffect()
{
	LightOff();
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFlashlightEffect::TurnOn()
{
	m_bIsOn = true;
#if !ENGINE_2013
	m_flCurrentPullBackDist = 1.0f;
#endif
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFlashlightEffect::TurnOff()
{
	if (m_bIsOn)
	{
		m_bIsOn = false;
		LightOff();
	}
}

// Custom trace filter that skips the player and the view model.
// If we don't do this, we'll end up having the light right in front of us all
// the time.
class CTraceFilterSkipPlayerAndViewModel : public CTraceFilter
{
public:
	virtual bool ShouldHitEntity( IHandleEntity *pServerEntity, int contentsMask )
	{
		// Test against the vehicle too?
		// FLASHLIGHTFIXME: how do you know that you are actually inside of the vehicle?
		C_BaseEntity *pEntity = EntityFromEntityHandle( pServerEntity );
		if ( !pEntity )
			return true;

		if ( ( dynamic_cast<C_BaseViewModel *>( pEntity ) != NULL ) ||
			 ( dynamic_cast<C_BasePlayer *>( pEntity ) != NULL ) ||
			 pEntity->GetCollisionGroup() == COLLISION_GROUP_DEBRIS ||
			 pEntity->GetCollisionGroup() == COLLISION_GROUP_INTERACTIVE_DEBRIS )
		{
			return false;
		}

		return true;
	}
};

//-----------------------------------------------------------------------------
// Purpose: Do the headlight
//-----------------------------------------------------------------------------
void CFlashlightEffect::UpdateLightNew(const Vector &vecPos, const Vector &vecForward, const Vector &vecRight, const Vector &vecUp )
{
	VPROF_BUDGET( "CFlashlightEffect::UpdateLightNew", VPROF_BUDGETGROUP_SHADOW_DEPTH_TEXTURING );

	FlashlightState_t state;

	// We will lock some of the flashlight params if player is on a ladder, to prevent oscillations due to the trace-rays
	bool bPlayerOnLadder = ( C_BasePlayer::GetLocalPlayer()->GetMoveType() == MOVETYPE_LADDER );

#if ENGINE_QUIVER
	const float flEpsilon = 0.1f;			// Offset flashlight position along vecUp
	const float flDistCutoff = r_flashlighttracedistcutoff.GetFloat();
	//const float flDistDrag = 0.2;
	const float flDistDrag = 0;
#endif

	CTraceFilterSkipPlayerAndViewModel traceFilter;
	float flOffsetY = r_flashlightoffsety.GetFloat();

	if( r_swingflashlight.GetBool() )
	{
		// This projects the view direction backwards, attempting to raise the vertical
		// offset of the flashlight, but only when the player is looking down.
		Vector vecSwingLight = vecPos + vecForward * -12.0f;
		if( vecSwingLight.z > vecPos.z )
		{
			flOffsetY += (vecSwingLight.z - vecPos.z);
		}
	}

	Vector vOrigin = vecPos + flOffsetY * vecUp;

	// Not on ladder...trace a hull
	if ( !bPlayerOnLadder ) 
	{
		trace_t pmOriginTrace;
		UTIL_TraceHull( vecPos, vOrigin, Vector(-4, -4, -4), Vector(4, 4, 4), MASK_SOLID & ~(CONTENTS_HITBOX), &traceFilter, &pmOriginTrace );

		if ( pmOriginTrace.DidHit() )
		{
			vOrigin = vecPos;
		}
	}
	else // on ladder...skip the above hull trace
	{
		vOrigin = vecPos;
	}

	// Now do a trace along the flashlight direction to ensure there is nothing within range to pull back from
	int iMask = MASK_OPAQUE_AND_NPCS;
	iMask &= ~CONTENTS_HITBOX;
	iMask |= CONTENTS_WINDOW;

	Vector vTarget = vecPos + vecForward * r_flashlightfar.GetFloat();

	// Work with these local copies of the basis for the rest of the function
	Vector vDir   = vTarget - vOrigin;
	Vector vRight = vecRight;
	Vector vUp    = vecUp;
	VectorNormalize( vDir   );
	VectorNormalize( vRight );
	VectorNormalize( vUp    );

	// Orthonormalize the basis, since the flashlight texture projection will require this later...
	vUp -= DotProduct( vDir, vUp ) * vDir;
	VectorNormalize( vUp );
	vRight -= DotProduct( vDir, vRight ) * vDir;
	VectorNormalize( vRight );
	vRight -= DotProduct( vUp, vRight ) * vUp;
	VectorNormalize( vRight );

	AssertFloatEquals( DotProduct( vDir, vRight ), 0.0f, 1e-3 );
	AssertFloatEquals( DotProduct( vDir, vUp    ), 0.0f, 1e-3 );
	AssertFloatEquals( DotProduct( vRight, vUp  ), 0.0f, 1e-3 );

	trace_t pmDirectionTrace;
	UTIL_TraceHull( vOrigin, vTarget, Vector( -4, -4, -4 ), Vector( 4, 4, 4 ), iMask, &traceFilter, &pmDirectionTrace );

	if ( r_flashlightvisualizetrace.GetBool() == true )
	{
		debugoverlay->AddBoxOverlay( pmDirectionTrace.endpos, Vector( -4, -4, -4 ), Vector( 4, 4, 4 ), QAngle( 0, 0, 0 ), 0, 0, 255, 16, 0 );
		debugoverlay->AddLineOverlay( vOrigin, pmDirectionTrace.endpos, 255, 0, 0, false, 0 );
	}

#if !ENGINE_2013
	float flDist = (pmDirectionTrace.endpos - vOrigin).Length();
	if ( flDist < flDistCutoff )
	{
		// We have an intersection with our cutoff range
		// Determine how far to pull back, then trace to see if we are clear
		float flPullBackDist = bPlayerOnLadder ? r_flashlightladderdist.GetFloat() : flDistCutoff - flDist;	// Fixed pull-back distance if on ladder
		m_flCurrentPullBackDist = Lerp( flDistDrag, m_flCurrentPullBackDist, flPullBackDist );
		
		if ( !bPlayerOnLadder )
		{
			trace_t pmBackTrace;
			UTIL_TraceHull( vOrigin, vOrigin - vDir*(flPullBackDist-flEpsilon), Vector( -4, -4, -4 ), Vector( 4, 4, 4 ), iMask, &traceFilter, &pmBackTrace );
			if( pmBackTrace.DidHit() )
			{
				// We have an intersection behind us as well, so limit our m_flCurrentPullBackDist
				float flMaxDist = (pmBackTrace.endpos - vOrigin).Length() - flEpsilon;
				if( m_flCurrentPullBackDist > flMaxDist )
					m_flCurrentPullBackDist = flMaxDist;
			}
		}
	}
	else
	{
		m_flCurrentPullBackDist = Lerp( flDistDrag, m_flCurrentPullBackDist, 0.0f );
	}
	vOrigin = vOrigin - vDir * m_flCurrentPullBackDist;
#endif

	state.m_vecLightOrigin = vOrigin;

	BasisToQuaternion( vDir, vRight, vUp, state.m_quatOrientation );

	bool bFlicker = false;

	C_BaseHLPlayer *pPlayer = (C_BaseHLPlayer *)C_BasePlayer::GetLocalPlayer();
	if ( pPlayer )
	{
		float flBatteryPower = ( pPlayer->m_HL2Local.m_flFlashBattery >= 0.0f ) ? ( pPlayer->m_HL2Local.m_flFlashBattery ) : pPlayer->m_HL2Local.m_flSuitPower;
		if ( flBatteryPower <= 10.0f )
		{
			float flScale;
			if ( flBatteryPower >= 0.0f )
			{	
				flScale = ( flBatteryPower <= 4.5f ) ? SimpleSplineRemapVal( flBatteryPower, 4.5f, 0.0f, 1.0f, 0.0f ) : 1.0f;
			}
			else
			{
				flScale = SimpleSplineRemapVal( flBatteryPower, 10.0f, 4.8f, 1.0f, 0.0f );
			}
			
			flScale = clamp( flScale, 0.0f, 1.0f );

			if ( flScale < 0.35f )
			{
				float flFlicker = cosf( gpGlobals->curtime * 6.0f ) * sinf( gpGlobals->curtime * 15.0f );
				
				if ( flFlicker > 0.25f && flFlicker < 0.75f )
				{
					// On
					state.m_fLinearAtten = r_flashlight_linear.GetFloat() * flScale;
				}
				else
				{
					// Off
					state.m_fLinearAtten = 0.0f;
				}
			}
			else
			{
				float flNoise = cosf( gpGlobals->curtime * 7.0f ) * sinf( gpGlobals->curtime * 25.0f );
				state.m_fLinearAtten = r_flashlight_linear.GetFloat() * flScale + 1.5f * flNoise;
			}

			state.m_fHorizontalFOVDegrees = r_flashlight_fov.GetFloat() - ( 16.0f * (1.0f-flScale) );
			state.m_fVerticalFOVDegrees = r_flashlight_fov.GetFloat() - ( 16.0f * (1.0f-flScale) );
			
			bFlicker = true;
		}
	}

	if ( bFlicker == false )
	{
		state.m_fLinearAtten = r_flashlight_linear.GetFloat();
		state.m_fHorizontalFOVDegrees = r_flashlight_fov.GetFloat();
		state.m_fVerticalFOVDegrees = r_flashlight_fov.GetFloat();
	}

	state.m_fQuadraticAtten = r_flashlight_quadratic.GetFloat();
	state.m_fConstantAtten = r_flashlight_constant.GetFloat();
	state.m_Color[0] = 1.0f;
	state.m_Color[1] = 1.0f;
	state.m_Color[2] = 1.0f;
	state.m_Color[3] = r_flashlightambient.GetFloat();
	//state.m_NearZ = r_flashlightnear.GetFloat() + m_flCurrentPullBackDist;	// Push near plane out so that we don't clip the world when the flashlight pulls back 
	state.m_NearZ = r_flashlightnear.GetFloat() + r_flashlightnearoffsetscale.GetFloat()
#if !ENGINE_2013
		* m_flCurrentPullBackDist
#endif
		;	// Optionally Push near plane out so that we don't clip the world when the flashlight pulls back 
	state.m_FarZ = r_flashlightfar.GetFloat();
	//state.m_bEnableShadows = r_flashlightdepthtexture.GetBool();
	state.m_bEnableShadows = true;

#if ENGINE_QUIVER
	state.m_flShadowMapResolution = r_shadowmapresolution.GetInt();
#else
	state.m_flShadowMapResolution = r_flashlightdepthres.GetInt();
#endif

	state.m_flShadowFilterSize = r_flashlight_filtersize.GetFloat();

	state.m_pSpotlightTexture = m_FlashlightTexture;
	state.m_nSpotlightTextureFrame = 0;

	state.m_flShadowAtten = r_flashlightshadowatten.GetFloat();

#if ENGINE_QUIVER
	state.m_flShadowSlopeScaleDepthBias = g_pMaterialSystemHardwareConfig->GetShadowSlopeScaleDepthBias();
	state.m_flShadowDepthBias = g_pMaterialSystemHardwareConfig->GetShadowDepthBias();
#endif

	if( m_FlashlightHandle == CLIENTSHADOW_INVALID_HANDLE )
	{
		m_FlashlightHandle = g_pClientShadowMgr->CreateFlashlight( state );
	}
	else
	{
		if( !r_flashlightlockposition.GetBool() )
		{
			g_pClientShadowMgr->UpdateFlashlightState( m_FlashlightHandle, state );
		}
	}
	
	g_pClientShadowMgr->UpdateProjectedTexture( m_FlashlightHandle, true );
	
	// Kill the old flashlight method if we have one.
	LightOffOld();

#ifndef NO_TOOLFRAMEWORK
	if ( clienttools->IsInRecordingMode() )
	{
		KeyValues *msg = new KeyValues( "FlashlightState" );
		msg->SetFloat( "time", gpGlobals->curtime );
		msg->SetInt( "entindex", m_nEntIndex );
		msg->SetInt( "flashlightHandle", m_FlashlightHandle );
		msg->SetPtr( "flashlightState", &state );
		ToolFramework_PostToolMessage( HTOOLHANDLE_INVALID, msg );
		msg->deleteThis();
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose: Do the headlight
//-----------------------------------------------------------------------------
void CFlashlightEffect::UpdateLightOld(const Vector &vecPos, const Vector &vecDir, int nDistance)
{
	if ( !m_pPointLight || ( m_pPointLight->key != m_nEntIndex ))
	{
		// Set up the environment light
		m_pPointLight = effects->CL_AllocDlight(m_nEntIndex);
		m_pPointLight->flags = 0.0f;
		m_pPointLight->radius = 80;
	}
	
	// For bumped lighting
	VectorCopy(vecDir, m_pPointLight->m_Direction);
	
	Vector end;
	end = vecPos + nDistance * vecDir;
	
	// Trace a line outward, skipping the player model and the view model.
	trace_t pm;
	CTraceFilterSkipPlayerAndViewModel traceFilter;
	UTIL_TraceLine( vecPos, end, MASK_ALL, &traceFilter, &pm );
	VectorCopy( pm.endpos, m_pPointLight->origin );
	
	float falloff = pm.fraction * nDistance;
	
	if ( falloff < 500 )
		falloff = 1.0;
	else
		falloff = 500.0 / falloff;
	
	falloff *= falloff;
	
	m_pPointLight->radius = 80;
	m_pPointLight->color.r = m_pPointLight->color.g = m_pPointLight->color.b = 255 * falloff;
	m_pPointLight->color.exponent = 0;
	
	// Make it live for a bit
	m_pPointLight->die = gpGlobals->curtime + 0.2f;
	
	// Update list of surfaces we influence
	render->TouchLight( m_pPointLight );
	
	// kill the new flashlight if we have one
	LightOffNew();
}

//-----------------------------------------------------------------------------
// Purpose: Do the headlight
//-----------------------------------------------------------------------------
void CFlashlightEffect::UpdateLight(const Vector &vecPos, const Vector &vecDir, const Vector &vecRight, const Vector &vecUp, int nDistance)
{
	if ( !m_bIsOn )
	{
		return;
	}
	if( r_newflashlight.GetBool() )
	{
		UpdateLightNew( vecPos, vecDir, vecRight, vecUp );
	}
	else
	{
		UpdateLightOld( vecPos, vecDir, nDistance );
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFlashlightEffect::LightOffNew()
{
#ifndef NO_TOOLFRAMEWORK
	if ( clienttools->IsInRecordingMode() )
	{
		KeyValues *msg = new KeyValues( "FlashlightState" );
		msg->SetFloat( "time", gpGlobals->curtime );
		msg->SetInt( "entindex", m_nEntIndex );
		msg->SetInt( "flashlightHandle", m_FlashlightHandle );
		msg->SetPtr( "flashlightState", NULL );
		ToolFramework_PostToolMessage( HTOOLHANDLE_INVALID, msg );
		msg->deleteThis();
	}
#endif

	// Clear out the light
	if( m_FlashlightHandle != CLIENTSHADOW_INVALID_HANDLE )
	{
		g_pClientShadowMgr->DestroyFlashlight( m_FlashlightHandle );
		m_FlashlightHandle = CLIENTSHADOW_INVALID_HANDLE;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFlashlightEffect::LightOffOld()
{	
	if ( m_pPointLight && ( m_pPointLight->key == m_nEntIndex ) )
	{
		m_pPointLight->die = gpGlobals->curtime;
		m_pPointLight = NULL;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFlashlightEffect::LightOff()
{	
	LightOffOld();
	LightOffNew();
}

CHeadlightEffect::CHeadlightEffect() {}
CHeadlightEffect::~CHeadlightEffect() {}

void CHeadlightEffect::UpdateLight( const Vector &vecPos, const Vector &vecDir, const Vector &vecRight, const Vector &vecUp, int nDistance )
{
	if ( IsOn() == false )
		 return;

	Vector basisX, basisY, basisZ;
	basisX = vecDir;
	basisY = vecRight;
	basisZ = vecUp;
	VectorNormalize(basisX);
	VectorNormalize(basisY);
	VectorNormalize(basisZ);

	FlashlightState_t state;

	BasisToQuaternion( basisX, basisY, basisZ, state.m_quatOrientation );
	state.m_vecLightOrigin = vecPos;
	state.m_pSpotlightTexture = m_FlashlightTexture;

	state.m_fHorizontalFOVDegrees = 120.0f;
	state.m_fVerticalFOVDegrees = 120.0f;
	state.m_fQuadraticAtten = 30000.0f; //r_flashlightquadratic.GetFloat();
	state.m_fLinearAtten = 0.0f; //r_flashlightlinear.GetFloat();
	state.m_fConstantAtten = 0.0f; //r_flashlightconstant.GetFloat();
	state.m_Color[0] = 1.0f;  // 255
	state.m_Color[1] = 0.8f; // 222
	state.m_Color[2] = 0.6f; // 200
	state.m_Color[3] = r_flashlightambient.GetFloat();
	state.m_NearZ = r_flashlightnear.GetFloat();
	state.m_FarZ = nDistance;
	state.m_bEnableShadows = true;
	state.m_pSpotlightTexture = m_FlashlightTexture;
	state.m_nSpotlightTextureFrame = 0;

//	state.m_flShadowAtten = r_flashlightshadowatten.GetFloat();
//	state.m_flShadowSlopeScaleDepthBias = g_pMaterialSystemHardwareConfig->GetShadowSlopeScaleDepthBias();
//	state.m_flShadowDepthBias = g_pMaterialSystemHardwareConfig->GetShadowDepthBias();

	if( GetFlashlightHandle() == CLIENTSHADOW_INVALID_HANDLE )
	{
		SetFlashlightHandle( g_pClientShadowMgr->CreateFlashlight( state ) );
	}
	else
	{
		g_pClientShadowMgr->UpdateFlashlightState( GetFlashlightHandle(), state );
	}
	
	g_pClientShadowMgr->UpdateProjectedTexture( GetFlashlightHandle(), true );
}