//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "hl2mp_gamerules.h"
#include "viewport_panel_names.h"
#include "gameeventdefs.h"
#include <KeyValues.h>
#include "ammodef.h"
#include "hl2_shareddefs.h"
#include "tier1/fmtstr.h"
#include "d_ammodef.h"
#include "d_gamemanager.h"

#ifdef CLIENT_DLL
	#include "c_hl2mp_player.h"
	#include "cliententitylist.h"
	#include "clientmode_hl2mpnormal.h"
#else
	#include "globalstate.h"
	#include "eventqueue.h"
	#include "player.h"
	#include "gamerules.h"
	#include "game.h"
	#include "items.h"
	#include "entitylist.h"
	#include "mapentities.h"
	#include "in_buttons.h"
	#include <ctype.h>
	#include "voice_gamemgr.h"
	#include "iscorer.h"
	#include "hl2mp_player.h"
	#include "basehlcombatweapon.h"
	#include "team.h"
	#include "voice_gamemgr.h"
	#include "hl2mp_gameinterface.h"
	#include "hl2mp_cvars.h"

	#include "demez_items.h"
	#include "datacache/imdlcache.h"

#ifdef DEBUG	
	#include "hl2mp_bot_temp.h"
#endif

extern void respawn(CBaseEntity *pEdict, bool fCopyCorpse);

extern bool FindInList( const char **pStrings, const char *pToFind );

static ConVar  physcannon_mega_enabled( "physcannon_mega_enabled", "0", FCVAR_CHEAT | FCVAR_REPLICATED );

ConVar sv_hl2mp_weapon_respawn_time( "d_sv_respawn_time_weapon", "20", FCVAR_GAMEDLL | FCVAR_NOTIFY );
ConVar sv_hl2mp_item_respawn_time( "d_sv_respawn_time_item", "30", FCVAR_GAMEDLL | FCVAR_NOTIFY );
ConVar sv_hl2mp_other_respawn_time( "d_sv_respawn_time_other", "30", FCVAR_GAMEDLL | FCVAR_NOTIFY );
ConVar sv_report_client_settings("sv_report_client_settings", "0", FCVAR_GAMEDLL | FCVAR_NOTIFY );

// Set the gamemode
// maybe FCVAR_NOT_CONNECTED?
ConVar demez_gamemode("d_gamemode", "2", FCVAR_GAMEDLL | FCVAR_ARCHIVE | FCVAR_REPLICATED,
					  "0 - Deathmatch\n1 - Team Deathmatch\n2 - Coop (Default)", true, 0, true, 2 );

ConVar demez_friendlyfire("d_friendlyfire", "0", FCVAR_GAMEDLL | FCVAR_ARCHIVE | FCVAR_REPLICATED);

ConVar  alyx_darkness_force("alyx_darkness_force", "0", FCVAR_CHEAT | FCVAR_REPLICATED);

// dumb!!!
#if ENGINE_CSGO
ConVar mp_chattime(
	"mp_chattime", 
	"10", 
	FCVAR_REPLICATED,
	"amount of time players can chat after the game is over",
	true, 1,
	true, 120 );
#else
extern ConVar mp_chattime;
#endif

extern CBaseEntity	 *g_pLastCombineSpawn;
extern CBaseEntity	 *g_pLastRebelSpawn;

#define WEAPON_MAX_DISTANCE_FROM_SPAWN 64

#endif


REGISTER_GAMERULES_CLASS( CHL2MPRules );

BEGIN_NETWORK_TABLE_NOBASE( CHL2MPRules, DT_HL2MPRules )

	#ifdef CLIENT_DLL
		RecvPropBool( RECVINFO( m_bTeamPlayEnabled ) ),
		RecvPropBool( RECVINFO( m_bCoOpEnabled ) ),
		RecvPropBool( RECVINFO( m_bMegaPhysgun ) ),
	#else
		SendPropBool( SENDINFO( m_bTeamPlayEnabled ) ),
		SendPropBool( SENDINFO( m_bCoOpEnabled ) ),
		SendPropBool( SENDINFO( m_bMegaPhysgun ) ),
	#endif

END_NETWORK_TABLE()

LINK_ENTITY_TO_CLASS_DUMB( hl2mp_gamerules, CHL2MPGameRulesProxy );
IMPLEMENT_NETWORKCLASS_ALIASED( HL2MPGameRulesProxy, DT_HL2MPGameRulesProxy )

static HL2MPViewVectors g_HL2MPViewVectors(
	Vector( 0, 0, 64 ),       //VEC_VIEW (m_vView) 
							  
	Vector(-16, -16, 0 ),	  //VEC_HULL_MIN (m_vHullMin)
	Vector( 16,  16,  72 ),	  //VEC_HULL_MAX (m_vHullMax)
							  					
	Vector(-16, -16, 0 ),	  //VEC_DUCK_HULL_MIN (m_vDuckHullMin)
	Vector( 16,  16,  36 ),	  //VEC_DUCK_HULL_MAX	(m_vDuckHullMax)
	Vector( 0, 0, 28 ),		  //VEC_DUCK_VIEW		(m_vDuckView)
							  					
	Vector(-10, -10, -10 ),	  //VEC_OBS_HULL_MIN	(m_vObsHullMin)
	Vector( 10,  10,  10 ),	  //VEC_OBS_HULL_MAX	(m_vObsHullMax)
							  					
	Vector( 0, 0, 14 ),		  //VEC_DEAD_VIEWHEIGHT (m_vDeadViewHeight)

	Vector(-16, -16, 0 ),	  //VEC_CROUCH_TRACE_MIN (m_vCrouchTraceMin)
	Vector( 16,  16,  60 )	  //VEC_CROUCH_TRACE_MAX (m_vCrouchTraceMax)
);

static const char *s_PreserveEnts[] =
{
	"ai_network",
	"ai_hint",
	"hl2mp_gamerules",
	"team_manager",
	"player_manager",
	"env_soundscape",
	"env_soundscape_proxy",
	"env_soundscape_triggerable",
	"env_sun",
	"env_wind",
	"env_fog_controller",
	"func_brush",
	"func_wall",
	"func_buyzone",
	"func_illusionary",
	"infodecal",
	"info_projecteddecal",
	"info_node",
	"info_target",
	"info_node_hint",
	"info_player_deathmatch",
	"info_player_combine",
	"info_player_rebel",
	"info_map_parameters",
	"keyframe_rope",
	"move_rope",
	"info_ladder",
	"player",
	"point_viewcontrol",
	"scene_manager",
	"shadow_control",
	"sky_camera",
	"soundent",
	"trigger_soundscape",
	"viewmodel",
	"predicted_viewmodel",
	"worldspawn",
	"point_devshot_camera",
	"", // END Marker
};



#ifdef CLIENT_DLL
	void RecvProxy_HL2MPRules( const RecvProp *pProp, void **pOut, void *pData, int objectID )
	{
		CHL2MPRules *pRules = HL2MPRules();
		Assert( pRules );
		*pOut = pRules;
	}

	BEGIN_RECV_TABLE( CHL2MPGameRulesProxy, DT_HL2MPGameRulesProxy )
		RecvPropDataTable( "hl2mp_gamerules_data", 0, 0, &REFERENCE_RECV_TABLE( DT_HL2MPRules ), RecvProxy_HL2MPRules )
	END_RECV_TABLE()
#else
	void* SendProxy_HL2MPRules( const SendProp *pProp, const void *pStructBase, const void *pData, CSendProxyRecipients *pRecipients, int objectID )
	{
		CHL2MPRules *pRules = HL2MPRules();
		Assert( pRules );
		return pRules;
	}

	BEGIN_SEND_TABLE( CHL2MPGameRulesProxy, DT_HL2MPGameRulesProxy )
		SendPropDataTable( "hl2mp_gamerules_data", 0, &REFERENCE_SEND_TABLE( DT_HL2MPRules ), SendProxy_HL2MPRules )
	END_SEND_TABLE()
#endif

#ifndef CLIENT_DLL

	class CVoiceGameMgrHelper : public IVoiceGameMgrHelper
	{
	public:
		virtual bool		CanPlayerHearPlayer( CBasePlayer *pListener, CBasePlayer *pTalker, bool &bProximity )
		{
			return ( pListener->GetTeamNumber() == pTalker->GetTeamNumber() );
		}
	};
	CVoiceGameMgrHelper g_VoiceGameMgrHelper;
	IVoiceGameMgrHelper *g_pVoiceGameMgrHelper = &g_VoiceGameMgrHelper;

#endif

// NOTE: the indices here must match TEAM_TERRORIST, TEAM_CT, TEAM_SPECTATOR, etc.
char *sTeamNames[] =
{
	"Unassigned",
	"Spectator",
	"Combine",
	"Rebels",
};


// idk where to put this
#if ENGINE_CSGO
#include "achievementmgr.h"

// #ifdef GAME_DLL
CAchievementMgr * CAchievementMgr::GetInstance()
{
	IAchievementMgr *pMgr = engine->GetAchievementMgr();
	class CAchievementMgrDelegateIAchievementMgr_friend : public CAchievementMgrDelegateIAchievementMgr
	{
	public: CAchievementMgr * GetDelegate() const { return m_pDelegate; }
	private: CAchievementMgrDelegateIAchievementMgr_friend() : CAchievementMgrDelegateIAchievementMgr( NULL ) {}
	};
	return reinterpret_cast< CAchievementMgrDelegateIAchievementMgr_friend * >( pMgr )->GetDelegate();
}
IAchievementMgr * CAchievementMgr::GetInstanceInterface()
{
	return engine->GetAchievementMgr();
}
// #endif // GAME_DLL

ISteamRemoteStorage *GetISteamRemoteStorage()
{
#ifndef NO_STEAM
	return ( steamapicontext != NULL ) ? steamapicontext->SteamRemoteStorage() : NULL;
#else
	return NULL;
#endif
}

Color g_WorkshopLogColor( 0, 255, 255, 255 );
BEGIN_DEFINE_LOGGING_CHANNEL( LOG_WORKSHOP, "Workshop", LCF_CONSOLE_ONLY, LS_WARNING, g_WorkshopLogColor );
ADD_LOGGING_CHANNEL_TAG( "UGCOperation" );
ADD_LOGGING_CHANNEL_TAG( "WorkshopOperation" );
END_DEFINE_LOGGING_CHANNEL();

#endif // ENGINE_CSGO


CHL2MPRules::CHL2MPRules()
{
#ifndef CLIENT_DLL
	// Create the team managers
	for ( int i = 0; i < ARRAYSIZE( sTeamNames ); i++ )
	{
		CTeam *pTeam = static_cast<CTeam*>(CreateEntityByName( "team_manager" ));
		pTeam->Init( sTeamNames[i], i );

		g_Teams.AddToTail( pTeam );
	}

	InitDefaultAIRelationships();

	m_bTeamPlayEnabled = (demez_gamemode.GetInt() == DEMEZ_GAMEMODE_TEAMPLAY);
	m_bCoOpEnabled = (demez_gamemode.GetInt() == DEMEZ_GAMEMODE_COOP);
	m_bMegaPhysgun = false;
	m_flIntermissionEndTime = 0.0f;
	m_flGameStartTime = 0;

	m_hRespawnableItemsAndWeapons.RemoveAll();
	m_tmNextPeriodicThink = 0;
	m_flRestartGameTime = 0;
	m_flTransitionTimerEnd = 0.0f;
	m_bCompleteReset = false;
	m_bHeardAllPlayersReady = false;
	m_bAwaitingReadyRestart = false;

	m_vecRespawnableEntities.RemoveAll();

	m_bTransitionTimerOn = false;
	m_pCheckpoint = NULL;
	m_pProxy = NULL;

#endif
}



#if ENGINE_NEW
#ifdef GAME_DLL
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
HSCRIPT CHL2MPRules::GetScriptInstance()
{
	/*if ( !m_hScriptInstance )
	{
		if ( m_iszScriptId == NULL_STRING )
		{
			char *szName = (char *)stackalloc( 1024 );
			g_pScriptVM->GenerateUniqueKey( "HL2MPRules", szName, 1024 );
			m_iszScriptId = AllocPooledString( szName );
		}

		// m_hScriptInstance = g_pScriptVM->RegisterInstance( GetScriptDesc(), this );
		m_hScriptInstance = g_pScriptVM->RegisterInstance( NULL, this );
		g_pScriptVM->SetInstanceUniqeId( m_hScriptInstance, STRING(m_iszScriptId) );
	}*/
	return m_hScriptInstance;
}

//-----------------------------------------------------------------------------
// Using my edict, cook up a unique VScript scope that's private to me, and
// persistent.
//-----------------------------------------------------------------------------
bool CHL2MPRules::ValidateScriptScope()
{
	/*if ( !m_ScriptScope.IsInitialized() )
	{
		if( scriptmanager == NULL )
		{
			ExecuteOnce( DevMsg( "Cannot execute script because scripting is disabled (-scripting)\n" ) );
			return false;
		}

		if( g_pScriptVM == NULL )
		{
			ExecuteOnce( DevMsg(" Cannot execute script because there is no available VM\n" ) );
			return false;
		}

		// Force instance creation
		GetScriptInstance();

		// EHANDLE hThis;
		// hThis.Set( this );

		bool bResult = m_ScriptScope.Init( STRING(m_iszScriptId) );

		if( !bResult )
		{
			DevMsg( "HL2MPRules couldn't create ScriptScope!\n" );
			return false;
		}

		g_pScriptVM->SetValue( m_ScriptScope, "self", GetScriptInstance() );
	}*/

	return true;
}
#endif
#endif


void CHL2MPRules::LevelInitPreEntity()
{
	// UpdateAmmoDef();
}


#ifdef GAME_DLL
extern void UpdatePhysEnvironment();
#endif

void CHL2MPRules::LevelInitPostEntity()
{
#ifdef GAME_DLL

	HSCRIPT hLevelInitPostEntity = g_pScriptVM->LookupFunction( "LevelInitPostEntity" );
	if ( hLevelInitPostEntity )
	{
		variant_t variant;
		variant.SetString( MAKE_STRING("LevelInitPostEntity") );
		g_EventQueue.AddEvent( m_pProxy, "CallScriptFunction", variant, 0, m_pProxy, m_pProxy );
		m_ScriptScope.ReleaseFunction( hLevelInitPostEntity );
	}

	UpdatePhysEnvironment();

#endif
}


const CViewVectors* CHL2MPRules::GetViewVectors()const
{
	return &g_HL2MPViewVectors;
}

const HL2MPViewVectors* CHL2MPRules::GetHL2MPViewVectors()const
{
	return &g_HL2MPViewVectors;
}
	
CHL2MPRules::~CHL2MPRules( void )
{
#ifndef CLIENT_DLL
	// Note, don't delete each team since they are in the gEntList and will 
	// automatically be deleted from there, instead.
	g_Teams.Purge();
#endif
}


void CHL2MPRules::CreateStandardEntities( void )
{

#ifndef CLIENT_DLL
	// Create the entity that will send our data to the client.

	BaseClass::CreateStandardEntities();

	g_pLastCombineSpawn = NULL;
	g_pLastRebelSpawn = NULL;

	m_pProxy = CreateEntityByName( "hl2mp_gamerules" );
	m_pProxy->SetLocalOrigin( vec3_origin );
	m_pProxy->SetLocalAngles( vec3_angle );

#if ENGINE_NEW
	m_pProxy->m_iszVScripts = MAKE_STRING("gamerules.nut");
#endif

	gEntList.NotifyCreateEntity( m_pProxy );

	DispatchSpawn( m_pProxy );

	// m_pProxy = CBaseEntity::Create( "hl2mp_gamerules", vec3_origin, vec3_angle );
	Assert( m_pProxy );
#endif
}

//=========================================================
// FlWeaponRespawnTime - what is the time in the future
// at which this weapon may spawn?
//=========================================================
float CHL2MPRules::FlWeaponRespawnTime( CBaseCombatWeapon *pWeapon )
{
#ifndef CLIENT_DLL
	if ( weaponstay.GetInt() > 0 )
	{
		// make sure it's only certain weapons
		if ( !(pWeapon->GetWeaponFlags() & ITEM_FLAG_LIMITINWORLD) )
		{
			return 0;		// weapon respawns almost instantly
		}
	}

	return sv_hl2mp_weapon_respawn_time.GetFloat();
#endif

	return 0;		// weapon respawns almost instantly
}


bool CHL2MPRules::IsIntermission( void )
{
#ifndef CLIENT_DLL
	return m_flIntermissionEndTime > gpGlobals->curtime;
#endif

	return false;
}

void CHL2MPRules::PlayerKilled( CBasePlayer *pVictim, const CTakeDamageInfo &info )
{
#ifndef CLIENT_DLL
	if ( IsIntermission() )
		return;
	BaseClass::PlayerKilled( pVictim, info );
#endif
}


#ifdef GAME_DLL
/*CON_COMMAND(d_transition_end, "", 0)
{
	HL2MPRules()->EndTransitionTimer();
}*/

ConVar d_sv_changelevel_enabled("d_sv_changelevel_enabled", "1", 0, "Disable Transition Timer");
#endif


void CHL2MPRules::Think( void )
{

#ifndef CLIENT_DLL
	
	CGameRules::Think();

	if( physcannon_mega_enabled.GetBool() == true )
	{
		m_bMegaPhysgun = true;
	}
	else
	{
		// FIXME: Is there a better place for this?
		m_bMegaPhysgun = ( GlobalEntity_GetState("super_phys_gun") == GLOBAL_ON );
	}

	if ( g_fGameOver )   // someone else quit the game already
	{
		// check to see if we should change levels now
		if ( m_flIntermissionEndTime < gpGlobals->curtime )
		{
			ChangeLevel(); // intermission is over
		}

		return;
	}

	if ( IsDeathmatch() )
	{
	//	float flTimeLimit = mp_timelimit.GetFloat() * 60;
		float flFragLimit = fraglimit.GetFloat();
	
		if ( GetMapRemainingTime() < 0 )
		{
			GoToIntermission();
			return;
		}

		if ( flFragLimit )
		{
			if( IsTeamplay() == true )
			{
				CTeam *pCombine = g_Teams[TEAM_COMBINE];
				CTeam *pRebels = g_Teams[TEAM_REBELS];

				if ( pCombine->GetScore() >= flFragLimit || pRebels->GetScore() >= flFragLimit )
				{
					GoToIntermission();
					return;
				}
			}
			else
			{
				// check if any player is over the frag limit
				for ( int i = 1; i <= gpGlobals->maxClients; i++ )
				{
					CBasePlayer *pPlayer = UTIL_PlayerByIndex( i );

					if ( pPlayer && pPlayer->FragCount() >= flFragLimit )
					{
						GoToIntermission();
						return;
					}
				}
			}
		}
	}

	if ( gpGlobals->curtime > m_tmNextPeriodicThink )
	{		
		CheckAllPlayersReady();
		CheckRestartGame();
		m_tmNextPeriodicThink = gpGlobals->curtime + 1.0;
	}

	if ( m_flRestartGameTime > 0.0f && m_flRestartGameTime <= gpGlobals->curtime )
	{
		RestartGame();
	}

	if( m_bAwaitingReadyRestart && m_bHeardAllPlayersReady )
	{
		UTIL_ClientPrintAll( HUD_PRINTCENTER, "All players ready. Game will restart in 5 seconds" );
		UTIL_ClientPrintAll( HUD_PRINTCONSOLE, "All players ready. Game will restart in 5 seconds" );

		m_flRestartGameTime = gpGlobals->curtime + 5;
		m_bAwaitingReadyRestart = false;
	}

	ManageObjectRelocation();
	RespawnEntities();

	if ( !d_sv_changelevel_enabled.GetBool() && m_bTransitionTimerOn )
	{
		UTIL_FOREACHPLAYER(i)
		{
			UTIL_GETNEXTPLAYER(i);
			CHL2MP_Player* hl2Player = (CHL2MP_Player*)pPlayer;
			hl2Player->m_bInTransition = false;
			pPlayer->UnlockPlayer();
		}

		m_pChangeLevel->m_iTransitionPlayers = 0;
		EndTransitionTimer();
	}

	if ( m_bTransitionTimerOn )
	{
		int timeLeft = m_flTransitionTimerEnd - gpGlobals->curtime;

		char text[128];
		V_snprintf( text, 128, "Changing Level in %d seconds", timeLeft );
		UTIL_ClientPrintAll( HUD_PRINTCENTER, text );

		if ( timeLeft <= 0 )
		{
			m_flTransitionTimerEnd = 0.0f;
			m_bTransitionTimerOn = false;
			m_pChangeLevel->ChangeLevelNow( NULL, true );
		}
	}

#endif
}


void CHL2MPRules::GoToIntermission( void )
{
#ifndef CLIENT_DLL
	if ( g_fGameOver )
		return;

	g_fGameOver = true;

	m_flIntermissionEndTime = gpGlobals->curtime + mp_chattime.GetInt();

	for ( int i = 0; i < MAX_PLAYERS; i++ )
	{
		CBasePlayer *pPlayer = UTIL_PlayerByIndex( i );

		if ( !pPlayer )
			continue;

		pPlayer->ShowViewPortPanel( PANEL_SCOREBOARD );
		pPlayer->AddFlag( FL_FROZEN );
	}
#endif
	
}

bool CHL2MPRules::CheckGameOver()
{
#ifndef CLIENT_DLL
	if ( g_fGameOver )   // someone else quit the game already
	{
		// check to see if we should change levels now
		if ( m_flIntermissionEndTime < gpGlobals->curtime )
		{
			ChangeLevel(); // intermission is over			
		}

		return true;
	}
#endif

	return false;
}


#ifdef GAME_DLL
void CHL2MPRules::SetAICriteria( AI_CriteriaSet& set )
{
	// Demez: funny hl2 check
	if ( DemezGameManager()->IsHL2Map() )
	{
		set.AppendCriteria( "game_hl2", "1" );
	}
	else if ( DemezGameManager()->IsEP1Map() )
	{
		set.AppendCriteria( "game_episodic", "1" );
		set.AppendCriteria( "game_ep1", "1" );
	}
	else if ( DemezGameManager()->IsEP2Map() )
	{
		set.AppendCriteria( "game_episodic", "1" );
		set.AppendCriteria( "game_ep2", "1" );
	}
}
#endif


// when we are within this close to running out of entities,  items 
// marked with the ITEM_FLAG_LIMITINWORLD will delay their respawn
#define ENTITY_INTOLERANCE	100

//=========================================================
// FlWeaponRespawnTime - Returns 0 if the weapon can respawn 
// now,  otherwise it returns the time at which it can try
// to spawn again.
//=========================================================
float CHL2MPRules::FlWeaponTryRespawn( CBaseCombatWeapon *pWeapon )
{
#ifndef CLIENT_DLL
	if ( pWeapon && (pWeapon->GetWeaponFlags() & ITEM_FLAG_LIMITINWORLD) )
	{
		if ( gEntList.NumberOfEntities() < (gpGlobals->maxEntities - ENTITY_INTOLERANCE) )
			return 0;

		// we're past the entity tolerance level,  so delay the respawn
		return FlWeaponRespawnTime( pWeapon );
	}
#endif
	return 0;
}

//=========================================================
// VecWeaponRespawnSpot - where should this weapon spawn?
// Some game variations may choose to randomize spawn locations
//=========================================================
Vector CHL2MPRules::VecWeaponRespawnSpot( CBaseCombatWeapon *pWeapon )
{
#ifndef CLIENT_DLL
	CBaseHLCombatWeapon *pHL2Weapon = dynamic_cast< CBaseHLCombatWeapon*>( pWeapon );

	if ( pHL2Weapon )
	{
		return pHL2Weapon->GetOriginalSpawnOrigin();
	}
#endif
	
	return pWeapon->GetAbsOrigin();
}

#ifndef CLIENT_DLL

CDemezItem* IsManagedObjectAnItem( CBaseEntity *pObject )
{
	return dynamic_cast< CDemezItem*>( pObject );
}

CBaseHLCombatWeapon* IsManagedObjectAWeapon( CBaseEntity *pObject )
{
	return dynamic_cast< CBaseHLCombatWeapon*>( pObject );
}

bool GetObjectsOriginalParameters( CBaseEntity *pObject, Vector &vOriginalOrigin, QAngle &vOriginalAngles )
{
	if ( CDemezItem *pItem = IsManagedObjectAnItem( pObject ) )
	{
		if ( pItem->m_flNextResetCheckTime > gpGlobals->curtime )
			 return false;
		
		vOriginalOrigin = pItem->GetOriginalSpawnOrigin();
		vOriginalAngles = pItem->GetOriginalSpawnAngles();

		pItem->m_flNextResetCheckTime = gpGlobals->curtime + sv_hl2mp_item_respawn_time.GetFloat();
		return true;
	}

	if ( CBaseHLCombatWeapon *pWeapon = IsManagedObjectAWeapon( pObject )) 
	{
		if ( pWeapon->m_flNextResetCheckTime > gpGlobals->curtime )
			 return false;

		vOriginalOrigin = pWeapon->GetOriginalSpawnOrigin();
		vOriginalAngles = pWeapon->GetOriginalSpawnAngles();

		pWeapon->m_flNextResetCheckTime = gpGlobals->curtime + sv_hl2mp_weapon_respawn_time.GetFloat();
		return true;
	}

	return false;
}

void CHL2MPRules::ManageObjectRelocation( void )
{
	for ( int i = 0; i < m_hRespawnableItemsAndWeapons.Count(); i++ )
	{
		CBaseEntity *pObject = m_hRespawnableItemsAndWeapons[i].Get();

		if ( pObject == NULL )
			continue;
			
		Vector vSpawOrigin;
		QAngle vSpawnAngles;

		if ( !GetObjectsOriginalParameters( pObject, vSpawOrigin, vSpawnAngles ) )
			continue;

		float flDistanceFromSpawn = (pObject->GetAbsOrigin() - vSpawOrigin ).Length();

		if ( flDistanceFromSpawn < WEAPON_MAX_DISTANCE_FROM_SPAWN )
			continue;

		IPhysicsObject *pPhysics = pObject->VPhysicsGetObject();
		// bool shouldReset = (pPhysics) ? pPhysics->IsAsleep() : (pObject->GetFlags() & FL_ONGROUND) ? true : false;

		// if ( shouldReset )
		// should we reset this entities position?
		if ( (pPhysics) ? pPhysics->IsAsleep() : (pObject->GetFlags() & FL_ONGROUND) )
		{
			pObject->Teleport( &vSpawOrigin, &vSpawnAngles, NULL );
			pObject->EmitSound( "AlyxEmp.Charge" );

			IPhysicsObject *pPhys = pObject->VPhysicsGetObject();

			if ( pPhys )
			{
				pPhys->Wake();
			}
		}
	}
}

//=========================================================
//AddLevelDesignerPlacedWeapon
//=========================================================
void CHL2MPRules::AddLevelDesignerPlacedObject( CBaseEntity *pEntity )
{
	if ( m_hRespawnableItemsAndWeapons.Find( pEntity ) == -1 )
	{
		m_hRespawnableItemsAndWeapons.AddToTail( pEntity );
	}
}

//=========================================================
//RemoveLevelDesignerPlacedWeapon
//=========================================================
void CHL2MPRules::RemoveLevelDesignerPlacedObject( CBaseEntity *pEntity )
{
	if ( m_hRespawnableItemsAndWeapons.Find( pEntity ) != -1 )
	{
		m_hRespawnableItemsAndWeapons.FindAndRemove( pEntity );
	}
}

//=========================================================
// Where should this item respawn?
// Some game variations may choose to randomize spawn locations
//=========================================================
Vector CHL2MPRules::VecItemRespawnSpot( CItem *pItem )
{
	return pItem->GetOriginalSpawnOrigin();
}

//=========================================================
// What angles should this item use to respawn?
//=========================================================
QAngle CHL2MPRules::VecItemRespawnAngles( CItem *pItem )
{
	return pItem->GetOriginalSpawnAngles();
}

//=========================================================
// At what time in the future may this Item respawn?
//=========================================================
float CHL2MPRules::FlItemRespawnTime( CItem *pItem )
{
	return sv_hl2mp_item_respawn_time.GetFloat();
}


//=========================================================
// CanHaveWeapon - returns false if the player is not allowed
// to pick up this weapon
//=========================================================
bool CHL2MPRules::CanHavePlayerItem( CBasePlayer *pPlayer, CBaseCombatWeapon *pItem )
{
	if ( weaponstay.GetInt() > 0 )
	{
		if ( pPlayer->Weapon_OwnsThisType( pItem->GetClassname(), pItem->GetSubType() ) )
			 return false;
	}

	return BaseClass::CanHavePlayerItem( pPlayer, pItem );
}

#endif

//=========================================================
// WeaponShouldRespawn - any conditions inhibiting the
// respawning of this weapon?
//=========================================================
int CHL2MPRules::WeaponShouldRespawn( CBaseCombatWeapon *pWeapon )
{
#ifndef CLIENT_DLL
	if ( pWeapon->HasSpawnFlags( SF_NORESPAWN ) )
	{
		return GR_WEAPON_RESPAWN_NO;
	}
#endif

	return GR_WEAPON_RESPAWN_YES;
}

//-----------------------------------------------------------------------------
// Purpose: Player has just left the game
//-----------------------------------------------------------------------------
void CHL2MPRules::ClientDisconnected( edict_t *pClient )
{
#ifndef CLIENT_DLL
	// Msg( "CLIENT DISCONNECTED, REMOVING FROM TEAM.\n" );

	CBasePlayer *pPlayer = (CBasePlayer *)CBaseEntity::Instance( pClient );
	if ( pPlayer )
	{
		// Remove the player from his team
		if ( pPlayer->GetTeam() )
		{
			pPlayer->GetTeam()->RemovePlayer( pPlayer );
		}
	}

	BaseClass::ClientDisconnected( pClient );

#endif
}


//=========================================================
// Deathnotice. 
//=========================================================
void CHL2MPRules::DeathNotice( CBasePlayer *pVictim, const CTakeDamageInfo &info )
{
#ifndef CLIENT_DLL
	// Work out what killed the player, and send a message to all clients about it
	const char *killer_weapon_name = "world";		// by default, the player is killed by the world
	int killer_ID = 0;

	// Find the killer & the scorer
	CBaseEntity *pInflictor = info.GetInflictor();
	CBaseEntity *pKiller = info.GetAttacker();
	CBasePlayer *pScorer = GetDeathScorer( pKiller, pInflictor );

	// Custom kill type?
	if ( info.GetDamageCustom() )
	{
		killer_weapon_name = GetDamageCustomString( info );
		if ( pScorer )
		{
			killer_ID = pScorer->GetUserID();
		}
	}
	else
	{
		// Is the killer a client?
		if ( pScorer )
		{
			killer_ID = pScorer->GetUserID();
			
			if ( pInflictor )
			{
				if ( pInflictor == pScorer )
				{
					// If the inflictor is the killer,  then it must be their current weapon doing the damage
					if ( pScorer->GetActiveWeapon() )
					{
						killer_weapon_name = pScorer->GetActiveWeapon()->GetClassname();
					}
				}
				else
				{
					killer_weapon_name = pInflictor->GetClassname();  // it's just that easy
				}
			}
		}
		else
		{
			killer_weapon_name = pInflictor->GetClassname();
		}

		// strip the NPC_* or weapon_* from the inflictor's classname
		if ( strncmp( killer_weapon_name, "weapon_", 7 ) == 0 )
		{
			killer_weapon_name += 7;
		}
		else if ( strncmp( killer_weapon_name, "npc_", 4 ) == 0 )
		{
			killer_weapon_name += 4;
		}
		else if ( strncmp( killer_weapon_name, "func_", 5 ) == 0 )
		{
			killer_weapon_name += 5;
		}
		else if ( strstr( killer_weapon_name, "physics" ) )
		{
			killer_weapon_name = "physics";
		}

		if ( strcmp( killer_weapon_name, "prop_combine_ball" ) == 0 )
		{
			killer_weapon_name = "combine_ball";
		}
		else if ( strcmp( killer_weapon_name, "grenade_ar2" ) == 0 )
		{
			killer_weapon_name = "smg1_grenade";
		}
		else if ( strcmp( killer_weapon_name, "satchel" ) == 0 || strcmp( killer_weapon_name, "tripmine" ) == 0)
		{
			killer_weapon_name = "slam";
		}


	}

	IGameEvent *event = gameeventmanager->CreateEvent( "player_death" );
	if( event )
	{
		event->SetInt("userid", pVictim->GetUserID() );
		event->SetInt("attacker", killer_ID );
		event->SetString("weapon", killer_weapon_name );
		event->SetInt( "priority", 7 );
		gameeventmanager->FireEvent( event );
	}
#endif

}

void CHL2MPRules::ClientSettingsChanged( CBasePlayer *pPlayer )
{
#ifndef CLIENT_DLL
	
	CHL2MP_Player *pHL2Player = ToHL2MPPlayer( pPlayer );

	if ( pHL2Player == NULL )
		return;

	const char *pCurrentModel = modelinfo->GetModelName( pPlayer->GetModel() );
	const char *szModelName = engine->GetClientConVarValue( ENTINDEX( pPlayer->edict() ), "cl_playermodel" );

	//If we're different.
	if ( stricmp( szModelName, pCurrentModel ) )
	{
		//Too soon, set the cvar back to what it was.
		//Note: this will make this function be called again
		//but since our models will match it'll just skip this whole dealio.
		if ( pHL2Player->GetNextModelChangeTime() >= gpGlobals->curtime )
		{
			char szReturnString[512];

			Q_snprintf( szReturnString, sizeof (szReturnString ), "cl_playermodel %s\n", pCurrentModel );
			engine->ClientCommand ( pHL2Player->edict(), szReturnString );

			Q_snprintf( szReturnString, sizeof( szReturnString ), "Please wait %d more seconds before trying to switch.\n", (int)(pHL2Player->GetNextModelChangeTime() - gpGlobals->curtime) );
			ClientPrint( pHL2Player, HUD_PRINTTALK, szReturnString );
			return;
		}

		if ( HL2MPRules()->IsTeamplay() == false )
		{
			pHL2Player->SetPlayerModel();

			const char *pszCurrentModelName = modelinfo->GetModelName( pHL2Player->GetModel() );

			char szReturnString[128];
			Q_snprintf( szReturnString, sizeof( szReturnString ), "Your player model is: %s\n", pszCurrentModelName );

			ClientPrint( pHL2Player, HUD_PRINTTALK, szReturnString );
		}
		else
		{
			if ( Q_stristr( szModelName, "models/human") )
			{
				pHL2Player->ChangeTeam( TEAM_REBELS );
			}
			else
			{
				pHL2Player->ChangeTeam( TEAM_COMBINE );
			}
		}
	}
	if ( sv_report_client_settings.GetInt() == 1 )
	{
		UTIL_LogPrintf( "\"%s\" cl_cmdrate = \"%s\"\n", pHL2Player->GetPlayerName(), engine->GetClientConVarValue( pHL2Player->entindex(), "cl_cmdrate" ));
	}

	// BaseClass::ClientSettingsChanged( pPlayer );
	const char* pszName = engine->GetClientConVarValue(pPlayer->entindex(), "name");

	const char* pszOldName = pPlayer->GetPlayerName();

	// msg everyone if someone changes their name,  and it isn't the first time (changing no name to current name)
	// Note, not using FStrEq so that this is case sensitive
	if (pszOldName[0] != 0 && Q_strcmp(pszOldName, pszName))
	{
		char text[256];
		Q_snprintf(text, sizeof(text), "%s changed name to %s\n", pszOldName, pszName);

		UTIL_ClientPrintAll(HUD_PRINTTALK, text);

		IGameEvent* event = gameeventmanager->CreateEvent("player_changename");
		if (event)
		{
			event->SetInt("userid", pPlayer->GetUserID());
			event->SetString("oldname", pszOldName);
			event->SetString("newname", pszName);
			gameeventmanager->FireEvent(event);
		}

		pPlayer->SetPlayerName(pszName);
	}

	const char* pszFov = engine->GetClientConVarValue(pPlayer->entindex(), "d_fov");
	if (pszFov)
	{
		pPlayer->SetDefaultFOV(atoi(pszFov));
	}
#endif
	
}

int CHL2MPRules::PlayerRelationship( CBaseEntity *pPlayer, CBaseEntity *pTarget )
{
#ifndef CLIENT_DLL
	if ( !pPlayer || !pTarget || !pTarget->IsPlayer() || ( !IsTeamplay() && !IsCoOp() ) )
	{
		return GR_NOTTEAMMATE;
	}

	/*if ( IsTeamplay() )
	{
		if ( (*GetTeamID(pPlayer) != '\0') && (*GetTeamID(pTarget) != '\0') && !stricmp( GetTeamID(pPlayer), GetTeamID(pTarget) ) )
		{
			return GR_TEAMMATE;
		}

		return GR_TEAMMATE;
	}*/

	if ( IsCoOp() )
	{
		return GR_TEAMMATE;
	}

#endif

	return GR_NOTTEAMMATE;
}

const char *CHL2MPRules::GetGameDescription( void )
{ 
	if ( IsCoOp() )
		return "Demez HL2 - Coop"; 

	if ( IsTeamplay() )
		return "Demez HL2 - Team Deathmatch"; 

	if ( IsDeathmatch() )
		return "Demez HL2 - Deathmatch"; 

	return "Demez HL2 - Unknown"; 
}


void CHL2MPRules::SetGameMode( int gamemode )
{
	switch (gamemode)
	{
	case DEMEZ_GAMEMODE_DEATHMATCH:
		m_bCoOpEnabled = false;
		m_bTeamPlayEnabled = false;
		break;

	case DEMEZ_GAMEMODE_TEAMPLAY:
		m_bCoOpEnabled = false;
		m_bTeamPlayEnabled = true;
		break;

	case DEMEZ_GAMEMODE_COOP:
	default:
		m_bCoOpEnabled = true;
		m_bTeamPlayEnabled = false;
		break;
	}
}


float CHL2MPRules::GetMapRemainingTime()
{
	// if timelimit is disabled, return 0
	if ( mp_timelimit.GetInt() <= 0 )
		return 0;

	// timelimit is in minutes

	float timeleft = (m_flGameStartTime + mp_timelimit.GetInt() * 60.0f ) - gpGlobals->curtime;

	return timeleft;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHL2MPRules::Precache( void )
{
	CBaseEntity::PrecacheScriptSound( "AlyxEmp.Charge" );
}

bool CHL2MPRules::ShouldCollide( int collisionGroup0, int collisionGroup1 )
{
	if ( collisionGroup0 > collisionGroup1 )
	{
		// swap so that lowest is always first
		V_swap(collisionGroup0,collisionGroup1);
	}

	/*if ( (collisionGroup0 == COLLISION_GROUP_PLAYER || collisionGroup0 == COLLISION_GROUP_PLAYER_MOVEMENT) &&
		collisionGroup1 == COLLISION_GROUP_WEAPON )
	{
		return false;
	}*/

	// Prevent the player movement from colliding with spit globs (caused the player to jump on top of globs while in water)
	if ( collisionGroup0 == COLLISION_GROUP_PLAYER_MOVEMENT && collisionGroup1 == HL2COLLISION_GROUP_SPIT )
		return false;

	// HL2 treats movement and tracing against players the same, so just remap here
	if ( collisionGroup0 == COLLISION_GROUP_PLAYER_MOVEMENT )
	{
		collisionGroup0 = COLLISION_GROUP_PLAYER;
	}

	if( collisionGroup1 == COLLISION_GROUP_PLAYER_MOVEMENT )
	{
		collisionGroup1 = COLLISION_GROUP_PLAYER;
	}

	if ( collisionGroup0 == COLLISION_GROUP_PLAYER && collisionGroup1 == COLLISION_GROUP_PLAYER )
	{
		return false;
	}

	//If collisionGroup0 is not a player then NPC_ACTOR behaves just like an NPC.
	if ( collisionGroup1 == COLLISION_GROUP_NPC_ACTOR && collisionGroup0 != COLLISION_GROUP_PLAYER )
	{
		collisionGroup1 = COLLISION_GROUP_NPC;
	}

	// This is only for the super physcannon
	if ( m_bMegaPhysgun )
	{
		if ( collisionGroup0 == COLLISION_GROUP_INTERACTIVE_DEBRIS && collisionGroup1 == COLLISION_GROUP_PLAYER )
			return false;
	}

	if ( collisionGroup0 == HL2COLLISION_GROUP_COMBINE_BALL )
	{
		if ( collisionGroup1 == HL2COLLISION_GROUP_COMBINE_BALL )
			return false;
	}

	if ( collisionGroup0 == HL2COLLISION_GROUP_COMBINE_BALL && collisionGroup1 == HL2COLLISION_GROUP_COMBINE_BALL_NPC )
		return false;

	if ( ( collisionGroup0 == COLLISION_GROUP_WEAPON ) ||
		( collisionGroup0 == COLLISION_GROUP_PLAYER ) ||
		( collisionGroup0 == COLLISION_GROUP_PROJECTILE ) )
	{
		if ( collisionGroup1 == HL2COLLISION_GROUP_COMBINE_BALL )
			return false;
	}

	if ( collisionGroup0 == COLLISION_GROUP_DEBRIS )
	{
		if ( collisionGroup1 == HL2COLLISION_GROUP_COMBINE_BALL )
			return true;
	}

	if (collisionGroup0 == HL2COLLISION_GROUP_HOUNDEYE && collisionGroup1 == HL2COLLISION_GROUP_HOUNDEYE )
		return false;

	if (collisionGroup0 == HL2COLLISION_GROUP_HOMING_MISSILE && collisionGroup1 == HL2COLLISION_GROUP_HOMING_MISSILE )
		return false;

	if ( collisionGroup1 == HL2COLLISION_GROUP_CROW )
	{
		if ( collisionGroup0 == COLLISION_GROUP_PLAYER || collisionGroup0 == COLLISION_GROUP_NPC ||
			 collisionGroup0 == HL2COLLISION_GROUP_CROW )
			return false;
	}

	if ( ( collisionGroup0 == HL2COLLISION_GROUP_HEADCRAB ) && ( collisionGroup1 == HL2COLLISION_GROUP_HEADCRAB ) )
		return false;

	// striders don't collide with other striders
	if ( collisionGroup0 == HL2COLLISION_GROUP_STRIDER && collisionGroup1 == HL2COLLISION_GROUP_STRIDER )
		return false;

	// gunships don't collide with other gunships
	if ( collisionGroup0 == HL2COLLISION_GROUP_GUNSHIP && collisionGroup1 == HL2COLLISION_GROUP_GUNSHIP )
		return false;

	// weapons and NPCs don't collide
	if ( collisionGroup0 == COLLISION_GROUP_WEAPON && (collisionGroup1 >= HL2COLLISION_GROUP_FIRST_NPC && collisionGroup1 <= HL2COLLISION_GROUP_LAST_NPC ) )
		return false;

	//players don't collide against NPC Actors.
	//I could've done this up where I check if collisionGroup0 is NOT a player but I decided to just
	//do what the other checks are doing in this function for consistency sake.
	if ( collisionGroup1 == COLLISION_GROUP_NPC_ACTOR && collisionGroup0 == COLLISION_GROUP_PLAYER )
		return false;
		
	// In cases where NPCs are playing a script which causes them to interpenetrate while riding on another entity,
	// such as a train or elevator, you need to disable collisions between the actors so the mover can move them.
	if ( collisionGroup0 == COLLISION_GROUP_NPC_SCRIPTED && collisionGroup1 == COLLISION_GROUP_NPC_SCRIPTED )
		return false;

	// Spit doesn't touch other spit
	if ( collisionGroup0 == HL2COLLISION_GROUP_SPIT && collisionGroup1 == HL2COLLISION_GROUP_SPIT )
		return false;

	return BaseClass::ShouldCollide( collisionGroup0, collisionGroup1 ); 

}

bool CHL2MPRules::ClientCommand( CBaseEntity *pEdict, const CCommand &args )
{
#ifndef CLIENT_DLL
	if( BaseClass::ClientCommand( pEdict, args ) )
		return true;


	CHL2MP_Player *pPlayer = (CHL2MP_Player *) pEdict;

	if ( pPlayer->ClientCommand( args ) )
		return true;
#endif

	return false;
}


#ifdef CLIENT_DLL

	ConVar cl_autowepswitch(
		"cl_autowepswitch",
		"1",
		FCVAR_ARCHIVE | FCVAR_USERINFO,
		"Automatically switch to picked up weapons (if more powerful)" );

#else

#ifdef DEBUG

	// Handler for the "bot" command.
	void Bot_f()
	{		
		// Look at -count.
		int count = 1;
		count = clamp( count, 1, 16 );

		int iTeam = TEAM_COMBINE;
				
		// Look at -frozen.
		bool bFrozen = false;
			
		// Ok, spawn all the bots.
		while ( --count >= 0 )
		{
			BotPutInServer( bFrozen, iTeam );
		}
	}


	ConCommand cc_Bot( "bot", Bot_f, "Add a bot.", FCVAR_CHEAT );

#endif

	bool CHL2MPRules::FShouldSwitchWeapon( CBasePlayer *pPlayer, CBaseCombatWeapon *pWeapon )
	{		
		if ( pPlayer->GetActiveWeapon() && pPlayer->IsNetClient() )
		{
			// Player has an active item, so let's check cl_autowepswitch.
			const char *cl_autowepswitch = engine->GetClientConVarValue( ENTINDEX( pPlayer->edict() ), "cl_autowepswitch" );
			if ( cl_autowepswitch && atoi( cl_autowepswitch ) <= 0 )
			{
				return false;
			}
		}

		return BaseClass::FShouldSwitchWeapon( pPlayer, pWeapon );
	}

#endif

#ifndef CLIENT_DLL

ConVar d_restarthack("d_restarthack", "0", 0);

void CHL2MPRules::RestartGame()
{
	if ( d_restarthack.GetBool() )
	{
		char cmd[128];
		V_snprintf(cmd, 128, "changelevel %s\n", gpGlobals->mapname);
		engine->ServerCommand(cmd);
		return;
	}

	// bounds check
	if ( mp_timelimit.GetInt() < 0 )
	{
		mp_timelimit.SetValue( 0 );
	}
	m_flGameStartTime = gpGlobals->curtime;
	if ( !IsFinite( m_flGameStartTime.Get() ) )
	{
		Warning( "Trying to set a NaN game start time\n" );
		m_flGameStartTime.GetForModify() = 0.0f;
	}

	CleanUpMap();
	
	// now respawn all players
	for (int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CHL2MP_Player *pPlayer = (CHL2MP_Player*) UTIL_PlayerByIndex( i );

		if ( !pPlayer )
			continue;

		if ( pPlayer->GetActiveWeapon() )
		{
			pPlayer->GetActiveWeapon()->Holster();
		}
		pPlayer->RemoveAllItems( true );
		respawn( pPlayer, false );
		pPlayer->Reset();
	}

	// Respawn entities (glass, doors, etc..)

	CTeam *pRebels = GetGlobalTeam( TEAM_REBELS );
	CTeam *pCombine = GetGlobalTeam( TEAM_COMBINE );

	if ( pRebels )
	{
		pRebels->SetScore( 0 );
	}

	if ( pCombine )
	{
		pCombine->SetScore( 0 );
	}

	m_flIntermissionEndTime = 0;
	m_flRestartGameTime = 0.0;		
	m_bCompleteReset = false;

	if ( IsDeathmatch() )
	{
		IGameEvent * event = gameeventmanager->CreateEvent( "round_start" );
		if ( event )
		{
			event->SetInt("fraglimit", 0 );
			event->SetInt( "priority", 6 ); // HLTV event priority, not transmitted

			event->SetString("objective", "DEATHMATCH");

			gameeventmanager->FireEvent( event );
		}
	}

	m_pCheckpoint = NULL;
	// m_vecCheckpoints.Purge();

	// hmmm
	LevelInitPostEntity();
}

void CHL2MPRules::CleanUpMap()
{
	// Recreate all the map entities from the map data (preserving their indices),
	// then remove everything else except the players.

	// Get rid of all entities except players.
	CBaseEntity *pCur = gEntList.FirstEnt();
	while ( pCur )
	{
		CBaseHLCombatWeapon *pWeapon = dynamic_cast< CBaseHLCombatWeapon* >( pCur );
		// Weapons with owners don't want to be removed..
		if ( pWeapon )
		{
			if ( !pWeapon->GetPlayerOwner() )
			{
				UTIL_Remove( pCur );
			}
		}
		// remove entities that has to be restored on roundrestart (breakables etc)
		else if ( !pCur->IsEFlagSet( EFL_KEEP_ON_RECREATE_ENTITIES ) && !FindInList( s_PreserveEnts, pCur->GetClassname() ) )
		{
			UTIL_Remove( pCur );
		}

		pCur = gEntList.NextEnt( pCur );
	}

	// Really remove the entities so we can have access to their slots below.
	gEntList.CleanupDeleteList();

	// Cancel all queued events, in case a func_bomb_target fired some delayed outputs that
	// could kill respawning CTs
	g_EventQueue.Clear();

	// Now reload the map entities.
	class CHL2MPMapEntityFilter : public IMapEntityFilter
	{
	public:
		virtual bool ShouldCreateEntity( const char *pClassname )
		{
			// Don't recreate the preserved entities.
			if ( !FindInList( s_PreserveEnts, pClassname ) )
			{
				return true;
			}
			else
			{
				// Increment our iterator since it's not going to call CreateNextEntity for this ent.
				if ( m_iIterator != g_MapEntityRefs.InvalidIndex() )
					m_iIterator = g_MapEntityRefs.Next( m_iIterator );

				return false;
			}
		}


		virtual CBaseEntity* CreateNextEntity( const char *pClassname )
		{
			if ( m_iIterator == g_MapEntityRefs.InvalidIndex() )
			{
				// This shouldn't be possible. When we loaded the map, it should have used 
				// CCSMapLoadEntityFilter, which should have built the g_MapEntityRefs list
				// with the same list of entities we're referring to here.
				Assert( false );
				return NULL;
			}
			else
			{
				CMapEntityRef &ref = g_MapEntityRefs[m_iIterator];
				m_iIterator = g_MapEntityRefs.Next( m_iIterator );	// Seek to the next entity.

				if ( ref.m_iEdict == -1 || INDEXENT( ref.m_iEdict ) )
				{
					// Doh! The entity was delete and its slot was reused.
					// Just use any old edict slot. This case sucks because we lose the baseline.
					return CreateEntityByName( pClassname );
				}
				else
				{
					// Cool, the slot where this entity was is free again (most likely, the entity was 
					// freed above). Now create an entity with this specific index.
					return CreateEntityByName( pClassname, ref.m_iEdict );
				}
			}
		}

	public:
		int m_iIterator; // Iterator into g_MapEntityRefs.
	};
	CHL2MPMapEntityFilter filter;
	filter.m_iIterator = g_MapEntityRefs.Head();

	// DO NOT CALL SPAWN ON info_node ENTITIES!

	MapEntity_ParseAllEntities( engine->GetMapEntitiesString(), &filter, true );
}

void CHL2MPRules::CheckChatForReadySignal( CHL2MP_Player *pPlayer, const char *chatmsg )
{
	if( m_bAwaitingReadyRestart && FStrEq( chatmsg, mp_ready_signal.GetString() ) )
	{
		if( !pPlayer->IsReady() )
		{
			pPlayer->SetReady( true );
		}		
	}
}

void CHL2MPRules::CheckRestartGame( void )
{
	// Restart the game if specified by the server
	int iRestartDelay = mp_restartgame.GetInt();

	if ( iRestartDelay > 0 )
	{
		if ( iRestartDelay > 60 )
			iRestartDelay = 60;


		// let the players know
		char strRestartDelay[64];
		Q_snprintf( strRestartDelay, sizeof( strRestartDelay ), "%d", iRestartDelay );
		UTIL_ClientPrintAll( HUD_PRINTCENTER, "Game will restart in %s1 %s2", strRestartDelay, iRestartDelay == 1 ? "SECOND" : "SECONDS" );
		UTIL_ClientPrintAll( HUD_PRINTCONSOLE, "Game will restart in %s1 %s2", strRestartDelay, iRestartDelay == 1 ? "SECOND" : "SECONDS" );

		m_flRestartGameTime = gpGlobals->curtime + iRestartDelay;
		m_bCompleteReset = true;
		mp_restartgame.SetValue( 0 );
	}

	if( mp_readyrestart.GetBool() )
	{
		m_bAwaitingReadyRestart = true;
		m_bHeardAllPlayersReady = false;
		

		const char *pszReadyString = mp_ready_signal.GetString();


		// Don't let them put anything malicious in there
		if( pszReadyString == NULL || Q_strlen(pszReadyString) > 16 )
		{
			pszReadyString = "ready";
		}

		IGameEvent *event = gameeventmanager->CreateEvent( "hl2mp_ready_restart" );
		if ( event )
			gameeventmanager->FireEvent( event );

		mp_readyrestart.SetValue( 0 );

		// cancel any restart round in progress
		m_flRestartGameTime = -1;
	}
}

void CHL2MPRules::CheckAllPlayersReady( void )
{
	for (int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CHL2MP_Player *pPlayer = (CHL2MP_Player*) UTIL_PlayerByIndex( i );

		if ( !pPlayer )
			continue;
		if ( !pPlayer->IsReady() )
			return;
	}
	m_bHeardAllPlayersReady = true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CHL2MPRules::GetChatFormat( bool bTeamOnly, CBasePlayer *pPlayer )
{
	// DEMEZ HACK: chat works if this returns NULL, could look into it more, but this is fine
	return NULL;

	if ( !pPlayer )  // dedicated server output
	{
		return NULL;
	}

	const char *pszFormat = NULL;

	// team only
	if ( bTeamOnly == TRUE )
	{
		if ( pPlayer->GetTeamNumber() == TEAM_SPECTATOR )
		{
			pszFormat = "HL2MP_Chat_Spec";
		}
		else
		{
			const char *chatLocation = GetChatLocation( bTeamOnly, pPlayer );
			if ( chatLocation && *chatLocation )
			{
				pszFormat = "HL2MP_Chat_Team_Loc";
			}
			else
			{
				pszFormat = "HL2MP_Chat_Team";
			}
		}
	}
	// everyone
	else
	{
		if ( pPlayer->GetTeamNumber() != TEAM_SPECTATOR )
		{
			pszFormat = "HL2MP_Chat_All";	
		}
		else
		{
			pszFormat = "HL2MP_Chat_AllSpec";
		}
	}

	return pszFormat;
}

bool CHL2MPRules::IsAlyxInDarknessMode()
{
	if (alyx_darkness_force.GetBool())
		return true;
	return (GlobalEntity_GetState("ep_alyx_darknessmode") == GLOBAL_ON);
}

float CHL2MPRules::GetAmmoDamage( CBaseEntity *pAttacker, CBaseEntity *pVictim, int nAmmoType )
{
	if (pAttacker == NULL || pVictim == NULL)
		return 0.0f;

	// Demez: is there a better place or way to do this?
	if ( pAttacker && pAttacker->IsPlayer() && pVictim && pVictim->IsPlayer() && IsCoOp() && !demez_friendlyfire.GetBool() )
	{
		return 0.0f;
	}

	float flDamage = 0.0f;
	CAmmoDef *pAmmoDef = GetAmmoDef();

	if ( pAmmoDef->DamageType( nAmmoType ) & DMG_SNIPER )
	{
		// If this damage is from a SNIPER, we do damage based on what the bullet
		// HITS, not who fired it. All other bullets have their damage values
		// arranged according to the owner of the bullet, not the recipient.
		if ( pVictim->IsPlayer() )
		{
			// Player
			flDamage = pAmmoDef->PlrDamage( nAmmoType );
		}
		else
		{
			// NPC or breakable
			flDamage = pAmmoDef->NPCDamage( nAmmoType );
		}
	}
	else
	{
		flDamage = BaseClass::GetAmmoDamage( pAttacker, pVictim, nAmmoType );
	}

	if( pAttacker->IsPlayer() && pVictim->IsNPC() )
	{
		if( pVictim->MyCombatCharacterPointer() )
		{
			// Player is shooting an NPC. Adjust the damage! This protects breakables
			// and other 'non-living' entities from being easier/harder to break
			// in different skill levels.
			flDamage = pAmmoDef->PlrDamage( nAmmoType );
			flDamage = AdjustPlayerDamageInflicted( flDamage );
		}
	}

	return flDamage;
}


int CHL2MPRules::ItemShouldRespawn( CItem *pItem )
{
	CDemezItem *pDemezItem = dynamic_cast<CDemezItem*>(pItem);
	if ( pDemezItem && pDemezItem->CanRespawn() )
	{
		return GR_ITEM_RESPAWN_NO;
	}

	return BaseClass::ItemShouldRespawn( pItem );
}

void CHL2MPRules::StartTransitionTimer( CChangeLevel* changeLevel )
{
	if ( !m_bTransitionTimerOn && d_sv_changelevel_enabled.GetBool() )
	{
		m_bTransitionTimerOn = true;
		m_flTransitionTimerEnd = gpGlobals->curtime + 30.0f;
		m_pChangeLevel = changeLevel;
	}
}

void CHL2MPRules::EndTransitionTimer()
{
	m_bTransitionTimerOn = false;
	m_flTransitionTimerEnd = 0.0f;
	m_pChangeLevel = NULL;
}


CON_COMMAND( d_sv_changelevel_now, "Force changelevel now if there is a changelevel timer" )
{
	DemezGameRules()->m_flTransitionTimerEnd += DemezGameRules()->m_flTransitionTimerEnd - gpGlobals->curtime;
}


CON_COMMAND( d_sv_changelevel_cancel, "Stop the changelevel timer" )
{
	DemezGameRules()->m_flTransitionTimerEnd += DemezGameRules()->m_flTransitionTimerEnd - gpGlobals->curtime;
}


//=========================================================
//GetEntityRespawnInfo
//=========================================================
CEntitySpawnInfo* CHL2MPRules::GetEntityRespawnInfo( CBaseEntity *pEntity )
{
	for ( int i = 0; i < m_vecRespawnableEntities.Count(); i++ )
	{
		CEntitySpawnInfo* respawnInfo = m_vecRespawnableEntities[i];

		if ( respawnInfo->m_pEntity == pEntity )
		{
			return respawnInfo;
		}
	}

	return NULL;
}


//=========================================================
//AddRespawnableEntity
//=========================================================
void CHL2MPRules::AddRespawnableEntity( CBaseEntity *pEntity )
{
	if ( GetEntityRespawnInfo( pEntity ) == NULL )
	{
		m_vecRespawnableEntities.AddToTail( new CEntitySpawnInfo( pEntity ) );
	}
}

//=========================================================
//RemoveRespawnableEntity
//=========================================================
void CHL2MPRules::RemoveRespawnableEntity( CBaseEntity *pEntity )
{
	CEntitySpawnInfo* pInfo = new CEntitySpawnInfo( pEntity );

	if ( pInfo && m_vecRespawnableEntities.Find( pInfo ) != -1 )
	{
		m_vecRespawnableEntities.FindAndRemove( pInfo );
		delete pInfo;
	}
}


//=========================================================
//RemoveRespawnableEntity
//=========================================================
void CHL2MPRules::SetEntityNeedsRespawn( CBaseEntity *pEntity, bool needed )
{
	CEntitySpawnInfo* pInfo = GetEntityRespawnInfo( pEntity );

	if ( pInfo )
	{
		pInfo->SetNeedsRespawn( needed );
	}
}


void CHL2MPRules::RespawnEntities( void )
{
	int iTotal = m_vecRespawnableEntities.Count();

	if ( iTotal > 0 )
	{
		for ( int i = 0; i < iTotal; i++ )
		{
			CEntitySpawnInfo *pInfo = m_vecRespawnableEntities[i];

			if ( pInfo )
			{
				if ( !pInfo->m_bNeedsRespawn )
				{
					continue;
				}

				if ( gpGlobals->curtime < pInfo->m_flNextRespawnTime )
				{
					continue;
				}

				CBaseEntity *pObject = pInfo->m_pEntity;

				if ( !pObject )
				{
					pInfo->SetNeedsRespawn( false );
					continue;
				}

				DispatchSpawn(pObject);
				pObject->Teleport( &pInfo->m_vSpawnOrigin, &pInfo->m_vSpawnAngles, NULL );
				pObject->EmitSound( "AlyxEmp.Charge" );

				pInfo->SetNeedsRespawn( false );
			}
		}
	}
}

// ====================================================================

CEntitySpawnInfo::CEntitySpawnInfo( CBaseEntity* pEntity )
{
	m_bCanRespawn = true;
	m_bNeedsRespawn = false;
	m_flNextRespawnTime = 0.0f;

	m_pEntity = pEntity;
	m_vSpawnAngles = pEntity->GetAbsAngles();
	m_vSpawnOrigin = pEntity->GetAbsOrigin();
}


CEntitySpawnInfo::~CEntitySpawnInfo()
{
}

void CEntitySpawnInfo::SetNeedsRespawn( bool enabled )
{
	m_bNeedsRespawn = enabled;

	if ( enabled )
	{
		m_flNextRespawnTime = gpGlobals->curtime + 2.0f;
	}
	else
	{
		m_flNextRespawnTime = 0.0f;
	}
}

// hmmm
CBaseEntity* CEntitySpawnInfo::CreateEntity()
{
	return m_pEntity;
}


void CHL2MPRules::PlayerSpawn( CBasePlayer *pPlayer )
{
	bool		addDefault;
	CBaseEntity	*pWeaponEntity = NULL;

	addDefault = true;

	while ( (pWeaponEntity = gEntList.FindEntityByClassname( pWeaponEntity, "game_player_equip" )) != NULL)
	{
		pWeaponEntity->Touch( pPlayer );
		addDefault = false;
	}

	if ( m_pCheckpoint != NULL )
	{
		m_pCheckpoint->TeleportPlayer( pPlayer );
	}
}


void CHL2MPRules::RegisterCheckpoint( CCheckpoint* checkpoint )
{
	/*if ( m_vecCheckpoints.Find(checkpoint) == -1 )
	{
		m_vecCheckpoints.AddToTail( checkpoint );

		if ( checkpoint->m_nIndex == -1 )
		{
			checkpoint->SetIndex( m_vecCheckpoints.Count() - 1 );
		}
	}*/
}


bool CHL2MPRules::SetCheckpoint( CCheckpoint* checkpoint )
{
	if ( m_pCheckpoint != NULL )
	{
		if ( m_pCheckpoint->m_nIndex > checkpoint->m_nIndex )
		{
			return false;
		}

		m_pCheckpoint->m_bActive = false;
	}

	m_pCheckpoint = checkpoint;
	m_pCheckpoint->m_bActive = true;

	// maybe nuke any previous checkpoints or mark as disabled?

	return true;
}


#endif  // not CLIENT_DLL


#if ENGINE_NEW
#ifdef CLIENT_DLL



#else

HSCRIPT ScriptCreateEntity( const char *className )
{
	bool precache = CBaseEntity::IsPrecacheAllowed();
	CBaseEntity::SetAllowPrecache( true );

	CBaseEntity *pBaseEntity = CreateEntityByName( className );
	DispatchSpawn( pBaseEntity );

	CBaseEntity::SetAllowPrecache( precache );

	return ToHScript( pBaseEntity );
}

/*void ScriptDispatchSpawn(  )
{
	CBaseEntity *pBaseEntity = CreateEntityByName( className );
	return ToHScript( pBaseEntity );
}*/

/*HSCRIPT CreateProp( const char *pszEntityName, const Vector &vOrigin, const char *pszModelName, int iAnim )
{
	CBaseAnimating *pBaseEntity = (CBaseAnimating *)CreateEntityByName( pszEntityName );
	pBaseEntity->SetAbsOrigin( vOrigin );
	pBaseEntity->SetModel( pszModelName );
	pBaseEntity->SetPlaybackRate( 1.0f );

	int iSequence = pBaseEntity->SelectWeightedSequence( (Activity)iAnim );

	if ( iSequence != -1 )
	{
		pBaseEntity->SetSequence( iSequence );
	}

	return ToHScript( pBaseEntity );
}*/

void ScriptDeleteEntity( const char* targetName )
{
}


bool ScriptIsHL2Map()
{
	return DemezGameManager()->IsHL2Map();
}

bool ScriptIsEP1Map()
{
	return DemezGameManager()->IsEP1Map();
}

bool ScriptIsEP2Map()
{
	return DemezGameManager()->IsEP2Map();
}

bool ScriptIsHL2()
{
	return DemezGameManager()->IsHL2();
}

bool ScriptIsEpisodic()
{
	return DemezGameManager()->IsEpisodic();
}

#endif


void CHL2MPRules::RegisterScriptFunctions()
{
	BaseClass::RegisterScriptFunctions();

#ifdef CLIENT_DLL



#else

	ScriptRegisterFunctionNamed( g_pScriptVM, ScriptCreateEntity, "CreateEntity", "Create an entity" );

	ScriptRegisterFunctionNamed( g_pScriptVM, ScriptIsHL2Map,    "IsHL2Map",    "Is Half-Life 2 Map" );
	ScriptRegisterFunctionNamed( g_pScriptVM, ScriptIsEP1Map,    "IsEP1Map",    "Is Episode 1 Map" );
	ScriptRegisterFunctionNamed( g_pScriptVM, ScriptIsEP2Map,    "IsEP2Map",    "Is Episode 2 Map" );

	ScriptRegisterFunctionNamed( g_pScriptVM, ScriptIsHL2,       "IsHL2",       "Is Half-Life 2" );
	ScriptRegisterFunctionNamed( g_pScriptVM, ScriptIsEpisodic,  "IsEpisodic",  "Is Episodic" );

#endif

	// static global func
	// ScriptRegisterFunction( g_pScriptVM, GivePlayerItem, "Get the name of the map." );
}
#endif


// blech
#if ENGINE_NEW

CBaseEntity* GetEntity( const char* arg )
{
	int index = atoi( arg );
	if ( index )
	{
		return CBaseEntity::Instance( index );
	}
	else if ( FStrEq(arg, "!picker") )
	{
		// do this later
		Warning("sorry, !picker not set up yet!!\n");
		return NULL;
	}
	else
	{
		// Otherwise set bits based on name or classname
#ifdef GAME_DLL
		CBaseEntity *ent = NULL;
		while ( (ent = gEntList.NextEnt(ent)) != NULL )
#else
		for ( C_BaseEntity *ent = ClientEntityList().FirstBaseEntity(); ent; ent = ClientEntityList().NextBaseEntity(ent) )
#endif
		{
			if (  (ent->GetEntityName() != NULL_STRING && FStrEq(arg, STRING(ent->GetEntityName())))	|| 
				(ent->m_iClassname != NULL_STRING && FStrEq(arg, STRING(ent->m_iClassname))) ||
				(ent->GetClassname()!=NULL && FStrEq(arg, ent->GetClassname())))
			{
				return ent;
			}
		}

		return NULL;
	}
}


#ifdef GAME_DLL
#define CLIENT_MSG( ... ) \
	if ( UTIL_GetCommandClient() ) \
		ClientPrint( UTIL_GetCommandClient(), HUD_PRINTCONSOLE, UTIL_VarArgs( __VA_ARGS__ ) ); \
	else \
		Msg( __VA_ARGS__ )


//------------------------------------------------------------------------------
// idk where to put this right now
//------------------------------------------------------------------------------


/*CON_COMMAND_F( ent_setkv, "Applies the comma delimited key=value pairs to the entity with the given Hammer ID.\n\tFormat: ent_keyvalue <entity id> <key1>=<value1>,<key2>=<value2>,...,<keyN>=<valueN>\n", FCVAR_CHEAT )
{
	// Must have an odd number of arguments.
	if ( ( args.ArgC() < 4 ) || ( args.ArgC() & 1 ) )
	{
		CLIENT_MSG( "Format: ent_keyvalue <entity name/id/> \"key1\"=\"value1\" \"key2\" \"value2\" ... \"keyN\" \"valueN\"\n" );
		return;
	}

	CBaseEntity* pEntity = GetEntity( args[1] );

	int nArg = 2;
	while ( nArg < args.ArgC() )
	{
		const char *pszKey = args[ nArg ];
		const char *pszValue = args[ nArg + 1 ];
		nArg += 2;

		pEntity->GetKeyValue

		g_ServerTools.SetKeyValue( pEnt, pszKey, pszValue );
	}
}


CON_COMMAND_F( ent_getkv, "Get entity keyvalues\n", FCVAR_CHEAT )
{
	// Must have an odd number of arguments.
	if ( ( args.ArgC() < 4 ) || ( args.ArgC() & 1 ) )
	{
		CLIENT_MSG( "Format: ent_keyvalue <entity id> \"key1\"=\"value1\" \"key2\" \"value2\" ... \"keyN\" \"valueN\"\n" );
		return;
	}

	CBaseEntity* pEntity = GetEntity( args[1] );

	int nArg = 2;
	while ( nArg < args.ArgC() )
	{
		const char *pszKey = args[ nArg ];
		const char *pszValue = args[ nArg + 1 ];
		nArg += 2;

		pEntity->GetKeyValue();
	}
}*/


CON_COMMAND( d_ent_count, "Get the current entity count and the entity limit" )
{
	CLIENT_MSG( "Number of Edicts:   %d / %d", gEntList.NumberOfEdicts(), MAX_EDICTS );
	CLIENT_MSG( "Number of Entities: %d / %d", gEntList.NumberOfEntities(), NUM_ENT_ENTRIES );
}


#endif


#ifdef CLIENT_DLL

#define UTIL_GetCommandClient() C_BasePlayer::GetLocalPlayer()

#endif


static bool UtlStringLessFunc( const CUtlString &lhs, const CUtlString &rhs )
{
	return Q_stricmp( lhs.String(), rhs.String() ) < 0;
}


#define DUMB_MAX_CMD 1024

/*#ifdef CLIENT_DLL
class CEntFireAutoCompletionFunctor : public ICommandCompletionCallback
#else*/
class CEntFireAutoCompletionFunctor : public ICommandCallback, public ICommandCompletionCallback
// #endif
{
public:

	CEntFireAutoCompletionFunctor( bool isServer )
	{
		server = isServer;
	}

	bool server;

	const char* GetCmdName()
	{
		return server ? "ent_fire_sv" : "ent_fire";
	}

	const char* GetSubString( const char *partial )
	{
		char *substring = (char *)partial;
		if ( Q_strstr( partial, GetCmdName() ) )
		{
			substring = (char *)partial + strlen( GetCmdName() ) + 1;
		}

		return substring;
	}

	virtual void CommandCallback( const CCommand &command )
	{
		#ifdef CLIENT_DLL

		// uhhhh
		char cmd[DUMB_MAX_CMD];
		V_snprintf(cmd, DUMB_MAX_CMD, "ent_fire_sv");

		for (int i = 1; i < command.ArgC(); i++)
		{
			V_snprintf(cmd, DUMB_MAX_CMD, "%s %s", cmd, command.Arg(i));
		}

		engine->ClientCmd_Unrestricted( cmd );

		#else

		CBasePlayer *pPlayer = ToBasePlayer( UTIL_GetCommandClient() );
		if (!pPlayer)
		{
			return;
		}

		// fires a command from the console
		if ( command.ArgC() < 2 )
		{
			ClientPrint( pPlayer, HUD_PRINTCONSOLE, "Usage:\n   ent_fire <target> [action] [value] [delay]\n" );
		}
		else
		{
			const char *target = "", *action = "Use";
			variant_t value;
			int delay = 0;

			target = STRING( AllocPooledString(command.Arg( 1 ) ) );

			// Don't allow them to run anything on a point_servercommand unless they're the host player. Otherwise they can ent_fire
			// and run any command on the server. Admittedly, they can only do the ent_fire if sv_cheats is on, but 
			// people complained about users resetting the rcon password if the server briefly turned on cheats like this:
			//    give point_servercommand
			//    ent_fire point_servercommand command "rcon_password mynewpassword"
			if ( gpGlobals->maxClients > 1 && V_stricmp( target, "point_servercommand" ) == 0 )
			{
				#ifdef GAME_DLL
				if ( engine->IsDedicatedServer() )
					return;

				CBasePlayer *pHostPlayer = UTIL_GetListenServerHost();
				if ( pPlayer != pHostPlayer )
					return;
				#else
				return;
				#endif
			}

			if ( command.ArgC() >= 3 )
			{
				action = STRING( AllocPooledString(command.Arg( 2 )) );
			}
			if ( command.ArgC() >= 4 )
			{
				value.SetString( AllocPooledString(command.Arg( 3 )) );
			}
			if ( command.ArgC() >= 5 )
			{
				delay = atoi( command.Arg( 4 ) );
			}

			g_EventQueue.AddEvent( target, action, value, delay, pPlayer, pPlayer );
		}
		#endif
	}
	virtual int CommandCompletionCallback( const char *partial, CUtlVector< CUtlString > &commands )
	{
		if ( !g_pGameRules )
		{
			return 0;
		}

		const char *cmdname = GetCmdName();
		const char *substring = GetSubString( partial );

		int checklen = 0;
		char *space = Q_strstr( substring, " " );
		if ( space )
		{
			return EntFire_AutoCompleteInput( partial, commands );;
		}
		else
		{
			checklen = Q_strlen( substring );
		}

		CUtlRBTree< CUtlString > symbols( 0, 0, UtlStringLessFunc );

#ifdef CLIENT_DLL
		for ( C_BaseEntity *pos = ClientEntityList().FirstBaseEntity(); pos; pos = ClientEntityList().NextBaseEntity(pos) )
#else
		CBaseEntity *pos = NULL;
		while ( ( pos = gEntList.NextEnt( pos ) ) != NULL )
#endif
		{
			// Check target name against partial string
			if ( pos->GetEntityName() == NULL_STRING )
				continue;

			if ( Q_strnicmp( STRING( pos->GetEntityName() ), substring, checklen ) )
				continue;

			CUtlString sym = STRING( pos->GetEntityName() );
			int idx = symbols.Find( sym );
			if ( idx == symbols.InvalidIndex() )
			{
				symbols.Insert( sym );
			}

			// Too many
			if ( symbols.Count() >= COMMAND_COMPLETION_MAXITEMS )
				break;
		}

		// Now fill in the results
		for ( int i = symbols.FirstInorder(); i != symbols.InvalidIndex(); i = symbols.NextInorder( i ) )
		{
			const char *name = symbols[ i ].String();

			char buf[ 512 ];
			Q_strncpy( buf, name, sizeof( buf ) );
			Q_strlower( buf );

			CUtlString command;
			command = CFmtStr( "%s %s", cmdname, buf );
			commands.AddToTail( command );
		}

		return symbols.Count();
	}


	int EntFire_AutoCompleteInput( const char *partial, CUtlVector< CUtlString > &commands )
	{
		const char *cmdname = GetCmdName();
		const char *substring = GetSubString( partial );

		int checklen = 0;
		char *space = Q_strstr( substring, " " );
		if ( !space )
		{
			Assert( !"CC_EntFireAutoCompleteInputFunc is broken\n" );
			return 0;
		}

		checklen = Q_strlen( substring );

		char targetEntity[ 256 ];
		targetEntity[0] = 0;
		int nEntityNameLength = (space-substring);
		Q_strncat( targetEntity, substring, sizeof( targetEntity ), nEntityNameLength );

		// Find the target entity by name
		// CBaseEntity *target = gEntList.FindEntityByName( NULL, targetEntity );
		CBaseEntity *target = GetEntity( targetEntity );
		if ( target == NULL )
			return 0;

		CUtlRBTree< CUtlString > symbols( 0, 0, UtlStringLessFunc );

		// Find the next portion of the text chain, if any (removing space)
		int nInputNameLength = (checklen-nEntityNameLength-1);

		// Starting past the last space, this is the remainder of the string
		char *inputPartial = ( checklen > nEntityNameLength ) ? (space+1) : NULL;

		for ( datamap_t *dmap = target->GetDataDescMap(); dmap != NULL; dmap = dmap->baseMap )
		{
			// Make sure we don't keep adding things in if the satisfied the limit
			if ( symbols.Count() >= COMMAND_COMPLETION_MAXITEMS )
				break;

			int c = dmap->dataNumFields;
			for ( int i = 0; i < c; i++ )
			{
				typedescription_t *field = &dmap->dataDesc[ i ];

				// Only want inputs
				if ( !( field->flags & FTYPEDESC_INPUT ) )
					continue;

				// Only want input functions
				if ( field->flags & FTYPEDESC_SAVE )
					continue;

				// See if we've got a partial string for the input name already
				if ( inputPartial != NULL )
				{
					if ( Q_strnicmp( inputPartial, field->externalName, nInputNameLength ) )
						continue;
				}

				CUtlString sym = field->externalName;

				int idx = symbols.Find( sym );
				if ( idx == symbols.InvalidIndex() )
				{
					symbols.Insert( sym );
				}

				// Too many items have been added
				if ( symbols.Count() >= COMMAND_COMPLETION_MAXITEMS )
					break;
			}
		}

		// Now fill in the results
		for ( int i = symbols.FirstInorder(); i != symbols.InvalidIndex(); i = symbols.NextInorder( i ) )
		{
			const char *name = symbols[ i ].String();

			char buf[ 512 ];
			Q_strncpy( buf, name, sizeof( buf ) );
			Q_strlower( buf );

			CUtlString command;
			command = CFmtStr( "%s %s %s", cmdname, targetEntity, buf );
			commands.AddToTail( command );
		}

		return symbols.Count();
}
};


#ifdef GAME_DLL
static CEntFireAutoCompletionFunctor g_EntFireAutoComplete(true);
static ConCommand ent_fire_sv("ent_fire_sv", &g_EntFireAutoComplete, "Usage:\n   ent_fire_sv <target> [action] [value] [delay]\n", FCVAR_CHEAT, &g_EntFireAutoComplete );
#else
static CEntFireAutoCompletionFunctor g_EntFireAutoComplete(false);
#endif

static ConCommand ent_fire("ent_fire", &g_EntFireAutoComplete, "Usage:\n   ent_fire <target> [action] [value] [delay]\n", FCVAR_CHEAT, &g_EntFireAutoComplete );

#endif



