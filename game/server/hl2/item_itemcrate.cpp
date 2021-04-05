//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: The various ammo types for HL2	
//
//=============================================================================//

#include "cbase.h"
#include "props.h"
#include "items.h"
#include "item_dynamic_resupply.h"
#include "demez_items.h"
#include "hl2mp_gamerules.h"
#include "vehicle_base.h"
#include "explode.h"
#include "ai_basenpc.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

const char *pszItemCrateModelName[] =
{
	{"models/items/item_item_crate.mdl"},
	("models/items/item_beacon_crate.mdl"),
};

//-----------------------------------------------------------------------------
// A breakable crate that drops items
//-----------------------------------------------------------------------------
class CItem_ItemCrate : public CPhysicsProp // , CRespawnableEntity
// class CItem_ItemCrate : public CDemezItem
{
public:
	DECLARE_CLASS( CItem_ItemCrate, CPhysicsProp );
	// DECLARE_CLASS( CItem_ItemCrate, CDemezItem );
	DECLARE_DATADESC();

	~CItem_ItemCrate();

	void Precache( void );
	void Spawn( void );

	virtual int	ObjectCaps() { return BaseClass::ObjectCaps() | FCAP_WCEDIT_POSITION; };

	virtual int		OnTakeDamage( const CTakeDamageInfo &info );
	virtual void	Event_Killed( const CTakeDamageInfo &info );

	void InputKill( inputdata_t &data );

	virtual void VPhysicsCollision( int index, gamevcollisionevent_t *pEvent );
	virtual void OnPhysGunPickup( CBasePlayer *pPhysGunUser, PhysGunPickup_t reason );
	virtual void Break( CBaseEntity *pBreaker, const CTakeDamageInfo &info );

protected:
	virtual void OnBreak( const Vector &vecVelocity, const AngularImpulse &angVel, CBaseEntity *pBreaker );

private:
	// Crate types. Add more!
	enum CrateType_t
	{
		CRATE_SPECIFIC_ITEM = 0,
		CRATE_TYPE_COUNT,
	};

	enum CrateAppearance_t
	{
		CRATE_APPEARANCE_DEFAULT = 0,
		CRATE_APPEARANCE_RADAR_BEACON,
	};

private:
	CrateType_t			m_CrateType;
	string_t			m_strItemClass;
	int					m_nItemCount;
	string_t			m_strAlternateMaster;
	CrateAppearance_t	m_CrateAppearance;

	COutputEvent m_OnCacheInteraction;

	bool m_bShouldRemoveOnKill = true;
};


LINK_ENTITY_TO_CLASS(item_item_crate, CItem_ItemCrate);


//-----------------------------------------------------------------------------
// Save/load: 
//-----------------------------------------------------------------------------
BEGIN_DATADESC( CItem_ItemCrate )

	DEFINE_KEYFIELD( m_CrateType, FIELD_INTEGER, "CrateType" ),	
	DEFINE_KEYFIELD( m_strItemClass, FIELD_STRING, "ItemClass" ),	
	DEFINE_KEYFIELD( m_nItemCount, FIELD_INTEGER, "ItemCount" ),	
	DEFINE_KEYFIELD( m_strAlternateMaster, FIELD_STRING, "SpecificResupply" ),	
	DEFINE_KEYFIELD( m_CrateAppearance, FIELD_INTEGER, "CrateAppearance" ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Kill", InputKill ),
	DEFINE_OUTPUT( m_OnCacheInteraction, "OnCacheInteraction" ),

END_DATADESC()


CItem_ItemCrate::~CItem_ItemCrate()
{
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CItem_ItemCrate::Precache( void )
{
	// Set this here to quiet base prop warnings
	PrecacheModel( pszItemCrateModelName[m_CrateAppearance] );
	SetModel( pszItemCrateModelName[m_CrateAppearance] );

	BaseClass::Precache();
	if ( m_CrateType == CRATE_SPECIFIC_ITEM )
	{
		if ( NULL_STRING != m_strItemClass )
		{
			// Don't precache if this is a null string. 
			UTIL_PrecacheOther( STRING(m_strItemClass) );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CItem_ItemCrate::Spawn( void )
{
	if ( g_pGameRules->IsAllowedToSpawn( this ) == false )
	{
		UTIL_Remove( this );
		return;
	}

	DisableAutoFade();
	SetModelName( AllocPooledString( pszItemCrateModelName[m_CrateAppearance] ) );

	if ( NULL_STRING == m_strItemClass )
	{
		Warning( "CItem_ItemCrate(%i):  CRATE_SPECIFIC_ITEM with NULL ItemClass string (deleted)!!!\n", entindex() );
		UTIL_Remove( this );
		return;
	}

	Precache( );
	SetModel( pszItemCrateModelName[m_CrateAppearance] );
	AddEFlags( EFL_NO_ROTORWASH_PUSH );
	BaseClass::Spawn( );

	if ( V_strcmp(m_strItemClass.ToCStr(), "item_dynamic_resupply") == 0 )
	{
		m_bShouldRemoveOnKill = false;
		HL2MPRules()->AddRespawnableEntity( this );
		// SetRemoveOnBreak( false );
	}

	if ( IsEffectActive( EF_NODRAW ))
		RemoveEffects( EF_NODRAW );
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &data - 
//-----------------------------------------------------------------------------
void CItem_ItemCrate::InputKill( inputdata_t &data )
{
	UTIL_Remove( this );
}


//-----------------------------------------------------------------------------
// Item crates blow up immediately
//-----------------------------------------------------------------------------
int CItem_ItemCrate::OnTakeDamage( const CTakeDamageInfo &info )
{
	if ( info.GetDamageType() & DMG_AIRBOAT )
	{
		CTakeDamageInfo dmgInfo = info;
		dmgInfo.ScaleDamage( 10.0 );
		return BaseClass::OnTakeDamage( dmgInfo );
	}

	return BaseClass::OnTakeDamage( info );
}


//-----------------------------------------------------------------------------
// Don't remove the crate
//-----------------------------------------------------------------------------
void CItem_ItemCrate::Event_Killed( const CTakeDamageInfo &info )
{
	if( info.GetAttacker() )
	{
		info.GetAttacker()->Event_KilledOther(this, info);
	}

	IPhysicsObject *pPhysics = VPhysicsGetObject();
	if ( pPhysics && !pPhysics->IsMoveable() )
	{
		pPhysics->EnableMotion( true );
		VPhysicsTakeDamage( info );
	}

	Break( info.GetInflictor(), info );

	if ( m_bShouldRemoveOnKill )
	{
		m_takedamage = DAMAGE_NO;
		m_lifeState = LIFE_DEAD;

		UTIL_Remove( this );
	}
	else
	{
		AddEffects( EF_NODRAW );
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CItem_ItemCrate::VPhysicsCollision( int index, gamevcollisionevent_t *pEvent )
{
	float flDamageScale = 1.0f;
	if ( FClassnameIs( pEvent->pEntities[!index], "prop_vehicle_airboat" ) ||
		 FClassnameIs( pEvent->pEntities[!index], "prop_vehicle_jeep" ) )
	{
		flDamageScale = 100.0f;
	}

	m_impactEnergyScale *= flDamageScale;
	BaseClass::VPhysicsCollision( index, pEvent );
	m_impactEnergyScale /= flDamageScale;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CItem_ItemCrate::OnBreak( const Vector &vecVelocity, const AngularImpulse &angImpulse, CBaseEntity *pBreaker )
{
	// FIXME: We could simply store the name of an entity to put into the crate 
	// as a string entered in by worldcraft. Should we?	I'd do it for sure
	// if it was easy to get a dropdown with all entity types in it.

	m_OnCacheInteraction.FireOutput(pBreaker,this);

	for ( int i = 0; i < m_nItemCount; ++i )
	{
		CBaseEntity *pSpawn = NULL;
		switch( m_CrateType )
		{
		case CRATE_SPECIFIC_ITEM:
			pSpawn = CreateEntityByName( STRING(m_strItemClass) );
			break;

		default:
			break;
		}

		if ( !pSpawn )
			return;

		// Give a little randomness...
		Vector vecOrigin;
		CollisionProp()->RandomPointInBounds( Vector(0.25, 0.25, 0.25), Vector( 0.75, 0.75, 0.75 ), &vecOrigin );
		pSpawn->SetAbsOrigin( vecOrigin );

		QAngle vecAngles;
		vecAngles.x = random->RandomFloat( -20.0f, 20.0f );
		vecAngles.y = random->RandomFloat( 0.0f, 360.0f );
		vecAngles.z = random->RandomFloat( -20.0f, 20.0f );
		pSpawn->SetAbsAngles( vecAngles );

		Vector vecActualVelocity;
		vecActualVelocity.Random( -10.0f, 10.0f );
//		vecActualVelocity += vecVelocity;
		pSpawn->SetAbsVelocity( vecActualVelocity );

		QAngle angVel;
		AngularImpulseToQAngle( angImpulse, angVel );
		pSpawn->SetLocalAngularVelocity( angVel );

		// If we're creating an item, it can't be picked up until it comes to rest
		// But only if it wasn't broken by a vehicle
		CItem *pItem = dynamic_cast<CItem*>(pSpawn);
		if ( pItem && !pBreaker->GetServerVehicle())
		{
			pItem->ActivateWhenAtRest();
		}

		/*CDemezItem *pDemezItem = dynamic_cast<CDemezItem*>(pSpawn);
		if ( pDemezItem )
		{
			pDemezItem->m_bCanRespawn = false;
		}*/

		pSpawn->Spawn();

		// Avoid missing items drops by a dynamic resupply because they don't think immediately
		if ( FClassnameIs( pSpawn, "item_dynamic_resupply" ) )
		{
			if ( m_strAlternateMaster != NULL_STRING )
			{
				DynamicResupply_InitFromAlternateMaster( pSpawn, m_strAlternateMaster );
			}
			if ( i == 0 )
			{
				pSpawn->AddSpawnFlags( SF_DYNAMICRESUPPLY_ALWAYS_SPAWN );
			}
			pSpawn->SetNextThink( gpGlobals->curtime );
		}
	}

	HL2MPRules()->SetEntityNeedsRespawn( this, true );
}

void CItem_ItemCrate::OnPhysGunPickup( CBasePlayer *pPhysGunUser, PhysGunPickup_t reason )
{
	BaseClass::OnPhysGunPickup( pPhysGunUser, reason );

	m_OnCacheInteraction.FireOutput( pPhysGunUser, this );

	if ( reason == PUNTED_BY_CANNON && m_CrateAppearance != CRATE_APPEARANCE_RADAR_BEACON )
	{
		Vector vForward;
		AngleVectors( pPhysGunUser->EyeAngles(), &vForward, NULL, NULL );
		Vector vForce = Pickup_PhysGunLaunchVelocity( this, vForward, PHYSGUN_FORCE_PUNTED );
		AngularImpulse angular = AngularImpulse( 0, 0, 0 );

		IPhysicsObject *pPhysics = VPhysicsGetObject();

		if ( pPhysics )
		{
			pPhysics->AddVelocity( &vForce, &angular );
		}

		TakeDamage( CTakeDamageInfo( pPhysGunUser, pPhysGunUser, GetHealth(), DMG_GENERIC ) );
	}
}


extern int g_BreakPropEvent;
void CItem_ItemCrate::Break( CBaseEntity *pBreaker, const CTakeDamageInfo &info )
{
	const char *pModelName = STRING( GetModelName() );
	if ( pModelName && Q_stristr( pModelName, "crate" ) )
	{
		bool bSmashed = false;
		if ( pBreaker && pBreaker->IsPlayer() )
		{
			bSmashed = true;
		}
		else if ( m_hPhysicsAttacker.Get() && m_hPhysicsAttacker->IsPlayer() )
		{
			bSmashed = true;
		}
		else if ( pBreaker && dynamic_cast< CPropVehicleDriveable * >( pBreaker ) )
		{
			CPropVehicleDriveable *veh = static_cast< CPropVehicleDriveable * >( pBreaker );
			CBaseEntity *driver = veh->GetDriver();
			if ( driver && driver->IsPlayer() )
			{
				bSmashed = true;
			}
		}
		if ( bSmashed )
		{
			// gamestats->Event_CrateSmashed();
		}
	}

#if ENGINE_NEW
	IGameEvent * event = gameeventmanager->CreateEvent( "break_prop", false, &g_BreakPropEvent );
#else
	IGameEvent * event = gameeventmanager->CreateEvent( "break_prop", false );
#endif

	if ( event )
	{
		if ( pBreaker && pBreaker->IsPlayer() )
		{
			event->SetInt( "userid", ToBasePlayer( pBreaker )->GetUserID() );
		}
		else
		{
			event->SetInt( "userid", 0 );
		}
		event->SetInt( "entindex", entindex() );
		gameeventmanager->FireEvent( event );
	}

	m_takedamage = DAMAGE_NO;
	m_OnBreak.FireOutput( pBreaker, this );

	Vector velocity;
	AngularImpulse angVelocity;
	IPhysicsObject *pPhysics = GetRootPhysicsObjectForBreak();

	Vector origin;
	QAngle angles;
	AddSolidFlags( FSOLID_NOT_SOLID );
	if ( pPhysics )
	{
		pPhysics->GetVelocity( &velocity, &angVelocity );
		pPhysics->GetPosition( &origin, &angles );
		pPhysics->RecheckCollisionFilter();
	}
	else
	{
		velocity = GetAbsVelocity();
		QAngleToAngularImpulse( GetLocalAngularVelocity(), angVelocity );
		origin = GetAbsOrigin();
		angles = GetAbsAngles();
	}

	PhysBreakSound( this, VPhysicsGetObject(), GetAbsOrigin() );

	bool bExploded = false;

	CBaseEntity *pAttacker = info.GetAttacker();
	if ( m_hLastAttacker )
	{
		// Pass along the person who made this explosive breakable explode.
		// This way the player allies can get immunity from barrels exploded by the player.
		pAttacker = m_hLastAttacker;
	}
	else if( m_hPhysicsAttacker )
	{
		// If I have a physics attacker and was influenced in the last 2 seconds,
		// Make the attacker my physics attacker. This helps protect citizens from dying
		// in the explosion of a physics object that was thrown by the player's physgun
		// and exploded on impact.
		if( gpGlobals->curtime - m_flLastPhysicsInfluenceTime <= 2.0f )
		{
			pAttacker = m_hPhysicsAttacker;
		}
	}

	if ( m_explodeDamage > 0 || m_explodeRadius > 0 )
	{
		if( HasInteraction( PROPINTER_PHYSGUN_BREAK_EXPLODE ) )
		{
			ExplosionCreate( WorldSpaceCenter(), angles, pAttacker, m_explodeDamage, m_explodeRadius, 
							SF_ENVEXPLOSION_NOSPARKS | SF_ENVEXPLOSION_NODLIGHTS | SF_ENVEXPLOSION_NOSMOKE | SF_ENVEXPLOSION_SURFACEONLY | SF_ENVEXPLOSION_NOSOUND,
							0.0f, this );
			EmitSound("PropaneTank.Burst");
		}
#if ENGINE_NEW
		else if( HasInteraction( PROPINTER_PHYSGUN_BREAK_EXPLODE_ICE ) )
		{
			ExplosionCreate( WorldSpaceCenter(), angles, pAttacker, m_explodeDamage, m_explodeRadius, 
							SF_ENVEXPLOSION_NODAMAGE | SF_ENVEXPLOSION_NOSPARKS | SF_ENVEXPLOSION_NODLIGHTS | SF_ENVEXPLOSION_NOSMOKE | SF_ENVEXPLOSION_SURFACEONLY | SF_ENVEXPLOSION_NOSOUND | SF_ENVEXPLOSION_ICE,
							0.0f, this );
			EmitSound("PropaneTank.Burst");
		}
#endif
		else
		{

			float flScale = 1.0f;

			ExplosionCreate( WorldSpaceCenter(), angles, pAttacker, m_explodeDamage * flScale, m_explodeRadius * flScale,
							SF_ENVEXPLOSION_NOSPARKS | SF_ENVEXPLOSION_NODLIGHTS | SF_ENVEXPLOSION_NOSMOKE | SF_ENVEXPLOSION_SURFACEONLY,
							0.0f, this );
		}

		bExploded = true;
	}

	// Allow derived classes to emit special things
	OnBreak( velocity, angVelocity, pBreaker );

	breakablepropparams_t params( origin, angles, velocity, angVelocity );
	params.impactEnergyScale = m_impactEnergyScale;
	params.defCollisionGroup = GetCollisionGroup();
	if ( params.defCollisionGroup == COLLISION_GROUP_NONE )
	{
		// don't automatically make anything COLLISION_GROUP_NONE or it will
		// collide with debris being ejected by breaking
		params.defCollisionGroup = COLLISION_GROUP_INTERACTIVE;
	}

	params.defBurstScale = 100;
	// in multiplayer spawn break models as clientside temp ents

	if ( gpGlobals->maxClients > 1 /*&& breakable_multiplayer.GetBool()*/ )
	{
		CPASFilter filter( WorldSpaceCenter() );

		Vector velocity; velocity.Init();

		if ( pPhysics )
			pPhysics->GetVelocity( &velocity, NULL );

		switch ( GetMultiplayerBreakMode() )
		{
		case MULTIPLAYER_BREAK_DEFAULT:		// default is to break client-side
		case MULTIPLAYER_BREAK_CLIENTSIDE:
#if ENGINE_CSGO
			te->PhysicsProp( filter, -1, GetModelIndex(), m_nSkin, GetAbsOrigin(), GetAbsAngles(), velocity, true, GetEffects(), GetRenderColor()  );
#else
			te->PhysicsProp( filter, -1, GetModelIndex(), m_nSkin, GetAbsOrigin(), GetAbsAngles(), velocity, true, GetEffects() );
#endif
			break;
		case MULTIPLAYER_BREAK_SERVERSIDE:	// server-side break
			if ( m_PerformanceMode != PM_NO_GIBS /*|| breakable_disable_gib_limit.GetBool()*/ )
			{
				PropBreakableCreateAll( GetModelIndex(), pPhysics, params, this, -1, ( m_PerformanceMode == PM_FULL_GIBS ), false );
			}
			break;
		case MULTIPLAYER_BREAK_BOTH:	// pieces break from both dlls
#if ENGINE_CSGO
			te->PhysicsProp( filter, -1, GetModelIndex(), m_nSkin, GetAbsOrigin(), GetAbsAngles(), velocity, true, GetEffects(), GetRenderColor()  );
#else
			te->PhysicsProp( filter, -1, GetModelIndex(), m_nSkin, GetAbsOrigin(), GetAbsAngles(), velocity, true, GetEffects() );
#endif
			if ( m_PerformanceMode != PM_NO_GIBS /*|| breakable_disable_gib_limit.GetBool()*/ )
			{
				PropBreakableCreateAll( GetModelIndex(), pPhysics, params, this, -1, ( m_PerformanceMode == PM_FULL_GIBS ), false );
			}
			break;
		}
	}
	// no damage/damage force? set a burst of 100 for some movement
	else if ( m_PerformanceMode != PM_NO_GIBS /*|| breakable_disable_gib_limit.GetBool()*/ )
	{
		PropBreakableCreateAll( GetModelIndex(), pPhysics, params, this, -1, ( m_PerformanceMode == PM_FULL_GIBS ) );
	}

	if( HasInteraction( PROPINTER_PHYSGUN_BREAK_EXPLODE ) )
	{
		if ( bExploded == false )
		{
			ExplosionCreate( origin, angles, pAttacker, 1, m_explodeRadius, 
							SF_ENVEXPLOSION_NODAMAGE | SF_ENVEXPLOSION_NOSPARKS | SF_ENVEXPLOSION_NODLIGHTS | SF_ENVEXPLOSION_NOSMOKE, 0.0f, this );			
		}

		// Find and ignite all NPC's within the radius
		CBaseEntity *pEntity = NULL;
		for ( CEntitySphereQuery sphere( origin, m_explodeRadius ); ( pEntity = sphere.GetCurrentEntity() ) != NULL; sphere.NextEntity() )
		{
			if( pEntity && pEntity->MyCombatCharacterPointer() )
			{
				// Check damage filters so we don't ignite friendlies
				if ( pEntity->PassesDamageFilter( info ) )
				{
					pEntity->MyCombatCharacterPointer()->Ignite( 30 );
				}
			}
		}
	}

#if ENGINE_NEW
	if( HasInteraction( PROPINTER_PHYSGUN_BREAK_EXPLODE_ICE ) )
	{
		if ( bExploded == false )
		{
			ExplosionCreate( origin, angles, pAttacker, 1, m_explodeRadius, 
							SF_ENVEXPLOSION_NOSPARKS | SF_ENVEXPLOSION_NODLIGHTS | SF_ENVEXPLOSION_NOSMOKE | SF_ENVEXPLOSION_ICE, 0.0f, this );			
		}

		// Find and freeze all NPC's within the radius
		CBaseEntity *pEntity = NULL;
		for ( CEntitySphereQuery sphere( origin, m_explodeRadius ); ( pEntity = sphere.GetCurrentEntity() ) != NULL; sphere.NextEntity() )
		{
			if( pEntity && pEntity->MyCombatCharacterPointer() )
			{
				// Check damage filters so we don't ignite friendlies
				if ( pEntity->PassesDamageFilter( info ) )
				{
					CAI_BaseNPC *pNPC = dynamic_cast<CAI_BaseNPC*>( pEntity );
					if ( pNPC )
					{
						pNPC->Freeze( 4.0f, pAttacker );
					}
				}
			}
		}
	}
#endif
}
