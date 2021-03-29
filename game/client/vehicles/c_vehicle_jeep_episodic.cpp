//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "c_prop_vehicle.h"
#include "c_vehicle_jeep.h"
#include "movevars_shared.h"
#include "view.h"
#include "flashlighteffect.h"
#include "c_baseplayer.h"
#include "c_te_effect_dispatch.h"
#include "hl2_vehicle_radar.h"
#include "usermessages.h"
#include "hud_radar.h"
#include "hud_macros.h"

#if ENGINE_CSGO
#include "hl2_usermessages.pb.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//=============================================================================
//
// Client-side Episodic Jeep (Jalopy) Class
//
class C_PropJeepEpisodic : public C_PropJeep
{

	DECLARE_CLASS( C_PropJeepEpisodic, C_PropJeep );

public:
	DECLARE_CLIENTCLASS();

public:
	C_PropJeepEpisodic();

	void OnEnteredVehicle( C_BasePlayer *pPlayer );

	void HandleUpdateJalopyRadar();

#if ENGINE_CSGO
	CUserMessageBinder m_UMCMsgUpdateJalopyRadar;
	bool MsgFunc_UpdateJalopyRadar(const CCSUsrMsg_UpdateJalopyRadar &msg) 
	{
		HandleUpdateJalopyRadar();
		return true;
	}
#else
	void MsgFunc_UpdateJalopyRadar(bf_read &msg) 
	{
		HandleUpdateJalopyRadar();
	}
#endif

#if ENGINE_NEW
	bool Simulate( void );
#else
	void Simulate( void );
#endif

public:
	int		m_iNumRadarContacts;
	Vector	m_vecRadarContactPos[ RADAR_MAX_CONTACTS ];
	int		m_iRadarContactType[ RADAR_MAX_CONTACTS ];
};
C_PropJeepEpisodic *g_pJalopy = NULL;

IMPLEMENT_CLIENTCLASS_DT( C_PropJeepEpisodic, DT_CPropJeepEpisodic, CPropJeepEpisodic )
	//CNetworkVar( int, m_iNumRadarContacts );
	RecvPropInt( RECVINFO(m_iNumRadarContacts) ),

	//CNetworkArray( Vector, m_vecRadarContactPos, RADAR_MAX_CONTACTS );
	RecvPropArray( RecvPropVector(RECVINFO(m_vecRadarContactPos[0])), m_vecRadarContactPos ),

	//CNetworkArray( int, m_iRadarContactType, RADAR_MAX_CONTACTS );
	RecvPropArray( RecvPropInt( RECVINFO(m_iRadarContactType[0] ) ), m_iRadarContactType ),

END_RECV_TABLE()

DECLARE_HUD_MESSAGE( C_PropJeepEpisodic, UpdateJalopyRadar );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_PropJeepEpisodic::HandleUpdateJalopyRadar() 
{
	// TODO: how do you handle multiple jalopys for coop?

	// Radar code here!
	if( !GetHudRadar() )
		return;

	// Sometimes we update more quickly when we need to track something in high resolution.
	// Usually we do not, so default to false.
	GetHudRadar()->m_bUseFastUpdate = false;

	for( int i = 0 ; i < m_iNumRadarContacts ; i++ )
	{
		if( m_iRadarContactType[i] == RADAR_CONTACT_DOG )
		{
			GetHudRadar()->m_bUseFastUpdate = true;
			break;
		}
	}

	float flContactTimeToLive;

	if( GetHudRadar()->m_bUseFastUpdate )
	{
		flContactTimeToLive = RADAR_UPDATE_FREQUENCY_FAST;
	}
	else
	{
		flContactTimeToLive = RADAR_UPDATE_FREQUENCY;
	}

	for( int i = 0 ; i < m_iNumRadarContacts ; i++ )
	{
		GetHudRadar()->AddRadarContact( m_vecRadarContactPos[i], m_iRadarContactType[i], flContactTimeToLive );	
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
C_PropJeepEpisodic::C_PropJeepEpisodic()
{
	if( g_pJalopy == NULL )
	{
/*#if ENGINE_CSGO

#else
		usermessages->HookMessage( "UpdateJalopyRadar", __MsgFunc_UpdateJalopyRadar );
#endif*/
	}

	g_pJalopy = this;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
#if ENGINE_NEW
bool C_PropJeepEpisodic::Simulate()
#else
void C_PropJeepEpisodic::Simulate()
#endif
{
	// Keep trying to hook to the radar.
	if( GetHudRadar() != NULL )
	{
		// This is not our ideal long-term solution. This will only work if you only have 
		// one jalopy in a given level. The Jalopy and the Radar Screen are currently both
		// assumed to be singletons. This is appropriate for EP2, however. (sjb)
		GetHudRadar()->SetVehicle( this );
	}

	return BaseClass::Simulate();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_PropJeepEpisodic::OnEnteredVehicle( C_BasePlayer *pPlayer )
{
	BaseClass::OnEnteredVehicle( pPlayer );
}
