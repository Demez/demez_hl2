//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose:		Base NPC character with AI
//
//=============================================================================//

#include "cbase.h"
#include "c_ai_demez_npc.h"
#include "c_basehlplayer.h"

C_AI_DemezNPC::C_AI_DemezNPC()
{
}

C_AI_DemezNPC::~C_AI_DemezNPC()
{
}

IMPLEMENT_CLIENTCLASS_DT( C_AI_DemezNPC, DT_AI_DemezNPC, CAI_DemezNPC )
END_RECV_TABLE()

void C_AI_DemezNPC::ClientThink( void )
{
	BaseClass::ClientThink();

	/*C_BaseHLPlayer *pPlayer = dynamic_cast<C_BaseHLPlayer*>( C_BasePlayer::GetLocalPlayer() );

	if ( pPlayer && m_flTimePingEffect > gpGlobals->curtime )
	{
		float fPingEffectTime = m_flTimePingEffect - gpGlobals->curtime;

		if ( fPingEffectTime > 0.0f )
		{
			Vector vRight, vUp;
			Vector vMins, vMaxs;

			float fFade;

			if( fPingEffectTime <= 1.0f )
			{
				fFade = 1.0f - (1.0f - fPingEffectTime);
			}
			else
			{
				fFade = 1.0f;
			}

			GetRenderBounds( vMins, vMaxs );
			AngleVectors (pPlayer->GetAbsAngles(), NULL, &vRight, &vUp );
			Vector p1 = GetAbsOrigin() + vRight * vMins.x + vUp * vMins.z;
			Vector p2 = GetAbsOrigin() + vRight * vMaxs.x + vUp * vMins.z;
			Vector p3 = GetAbsOrigin() + vUp * vMaxs.z;

			int r = 0 * fFade;
			int g = 255 * fFade;
			int b = 0 * fFade;

#if ENGINE_NEW
			debugoverlay->AddLineOverlay( p1, p2, r, g, b, true, 0.05f );
			debugoverlay->AddLineOverlay( p2, p3, r, g, b, true, 0.05f );
			debugoverlay->AddLineOverlay( p3, p1, r, g, b, true, 0.05f );
#endif
		}
	}*/
}


int C_AI_DemezNPC::DrawModel( int flags )
{
	RenderableInstance_t instance;
	return DrawModel(flags, instance);
}

int C_AI_DemezNPC::DrawModel( int flags, const RenderableInstance_t &instance )
{
#if ENGINE_NEW
	return BaseClass::DrawModel(flags, instance);
#else
	return BaseClass::DrawModel(flags);
#endif
}

