//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//

#pragma once

#include "gamerules.h"
#include "teamplay_gamerules.h"
#include "gamevars_shared.h"
#include "hl2_shareddefs.h"
#include "d_ammodef.h"
#include "engine_defines.h"

#ifndef CLIENT_DLL
#include "hl2mp_player.h"
#include "triggers.h"
#include "trigger_checkpoint.h"
#include "d_changelevel.h"
#endif

#if ENGINE_CSGO
#include "cstrike15_usermessages.pb.h"
#include "hl2_usermessages.pb.h"
#endif

#define VEC_CROUCH_TRACE_MIN	HL2MPRules()->GetHL2MPViewVectors()->m_vCrouchTraceMin
#define VEC_CROUCH_TRACE_MAX	HL2MPRules()->GetHL2MPViewVectors()->m_vCrouchTraceMax


extern ConVar	demez_gamemode;

#define DEMEZ_GAMEMODE_DEATHMATCH 0
#define DEMEZ_GAMEMODE_TEAMPLAY 1
#define DEMEZ_GAMEMODE_COOP 2


enum
{
	// coop mode
	TEAM_COOP = 3,

	// team deathmatch mode
	TEAM_COMBINE = 2,
	TEAM_REBELS,
};


#ifdef CLIENT_DLL
	#define CHL2MPRules C_HL2MPRules
	#define CHL2MPGameRulesProxy C_HL2MPGameRulesProxy
#endif

class CHL2MPGameRulesProxy : public CGameRulesProxy
{
public:
	DECLARE_CLASS( CHL2MPGameRulesProxy, CGameRulesProxy );
	DECLARE_NETWORKCLASS();
};

class HL2MPViewVectors : public CViewVectors
{
public:
	HL2MPViewVectors( 
		Vector vView,
		Vector vHullMin,
		Vector vHullMax,
		Vector vDuckHullMin,
		Vector vDuckHullMax,
		Vector vDuckView,
		Vector vObsHullMin,
		Vector vObsHullMax,
		Vector vDeadViewHeight,
		Vector vCrouchTraceMin,
		Vector vCrouchTraceMax ) :
			CViewVectors( 
				vView,
				vHullMin,
				vHullMax,
				vDuckHullMin,
				vDuckHullMax,
				vDuckView,
				vObsHullMin,
				vObsHullMax,
				vDeadViewHeight )
	{
		m_vCrouchTraceMin = vCrouchTraceMin;
		m_vCrouchTraceMax = vCrouchTraceMax;
	}

	Vector m_vCrouchTraceMin;
	Vector m_vCrouchTraceMax;	
};


#ifdef GAME_DLL
class CEntitySpawnInfo
{
public:
	CEntitySpawnInfo( CBaseEntity *pEntity );
	~CEntitySpawnInfo();

	void SetNeedsRespawn( bool enabled );
	CBaseEntity* CreateEntity();

	CBaseEntity* m_pEntity;

	bool    m_bCanRespawn;
	bool    m_bNeedsRespawn;

	float   m_flNextRespawnTime;

	Vector		m_vSpawnOrigin;
	QAngle		m_vSpawnAngles;
};
#endif

// TODO: not inherit from CTeamplayRules (probably never lol)
class CHL2MPRules : public CTeamplayRules
{
public:
	DECLARE_CLASS( CHL2MPRules, CTeamplayRules );

#ifdef CLIENT_DLL

	DECLARE_CLIENTCLASS_NOBASE(); // This makes datatables able to access our private vars.

#else

	DECLARE_SERVERCLASS_NOBASE(); // This makes datatables able to access our private vars.
#endif
	
	CHL2MPRules();
	virtual ~CHL2MPRules();

	virtual void LevelInitPreEntity();
	virtual void LevelInitPostEntity();

	virtual void Precache( void );
	virtual bool ShouldCollide( int collisionGroup0, int collisionGroup1 );
	virtual bool ClientCommand( CBaseEntity *pEdict, const CCommand &args );

	virtual float FlWeaponRespawnTime( CBaseCombatWeapon *pWeapon );
	virtual float FlWeaponTryRespawn( CBaseCombatWeapon *pWeapon );
	virtual Vector VecWeaponRespawnSpot( CBaseCombatWeapon *pWeapon );
	virtual int WeaponShouldRespawn( CBaseCombatWeapon *pWeapon );
	virtual void Think( void );
	virtual void CreateStandardEntities( void );
	virtual void ClientSettingsChanged( CBasePlayer *pPlayer );
	virtual int PlayerRelationship( CBaseEntity *pPlayer, CBaseEntity *pTarget );
	virtual void GoToIntermission( void );
	virtual void DeathNotice( CBasePlayer *pVictim, const CTakeDamageInfo &info );

	// derive this function if you mod uses encrypted weapon info files
	virtual const unsigned char*	GetEncryptionKey( void ) { return (unsigned char *)"x9Ke0BY7"; }
	virtual const char*             GetGameDescription( void );
	virtual const CViewVectors*     GetViewVectors() const;
	const HL2MPViewVectors*         GetHL2MPViewVectors() const;

	virtual bool IsMultiplayer( void )
	{
		return (gpGlobals->maxClients > 1);
	}

	float GetMapRemainingTime();
	void CleanUpMap();
	void CheckRestartGame();
	void RestartGame();
	
#ifndef CLIENT_DLL
#if ENGINE_NEW
	HSCRIPT GetScriptInstance();
	bool ValidateScriptScope();
#endif

	virtual Vector VecItemRespawnSpot( CItem *pItem );
	virtual QAngle VecItemRespawnAngles( CItem *pItem );
	virtual float	FlItemRespawnTime( CItem *pItem );
	virtual bool	CanHavePlayerItem( CBasePlayer *pPlayer, CBaseCombatWeapon *pItem );
	virtual bool FShouldSwitchWeapon( CBasePlayer *pPlayer, CBaseCombatWeapon *pWeapon );

	void	InitDefaultAIRelationships(void);
	void	AddLevelDesignerPlacedObject( CBaseEntity *pEntity );
	void	RemoveLevelDesignerPlacedObject( CBaseEntity *pEntity );
	void	ManageObjectRelocation( void );

	void    CheckChatForReadySignal( CHL2MP_Player *pPlayer, const char *chatmsg );
	const char *GetChatFormat( bool bTeamOnly, CBasePlayer *pPlayer );
	virtual float GetAmmoDamage( CBaseEntity *pAttacker, CBaseEntity *pVictim, int nAmmoType );

	virtual bool IsAlyxInDarknessMode();

	void	SetAICriteria( AI_CriteriaSet& set );

	void StartTransitionTimer( CChangeLevel* changeLevel );
	void EndTransitionTimer();

	CEntitySpawnInfo* GetEntityRespawnInfo( CBaseEntity* pEntity );
	void AddRespawnableEntity( CBaseEntity* pEntity );
	void RemoveRespawnableEntity( CBaseEntity* pEntity );
	void SetEntityNeedsRespawn( CBaseEntity* pEntity, bool needed = true );
	void RespawnEntities();

	void PlayerSpawn( CBasePlayer* pPlayer );

	void RegisterCheckpoint( CCheckpoint* checkpoint );
	bool SetCheckpoint( CCheckpoint* checkpoint );
	CCheckpoint* m_pCheckpoint;
	// CUtlVector< CTriggerCheckpoint* > m_vecCheckpoints;

#endif
	virtual void ClientDisconnected( edict_t *pClient );

	bool CheckGameOver( void );
	bool IsIntermission( void );

	bool AllowDamage( CBaseEntity *pVictim, const CTakeDamageInfo &info );
	void PlayerKilled( CBasePlayer *pVictim, const CTakeDamageInfo &info );

	bool	IsTeamplay( void )		{ return m_bTeamPlayEnabled; }
	bool	IsDeathmatch( void )	{ return !m_bCoOpEnabled; }
	bool	IsCoOp( void )			{ return m_bCoOpEnabled; }

	void	SetGameMode( int gamemode );

	bool	MegaPhyscannonActive( void ) { return m_bMegaPhysgun; }

	void	CheckAllPlayersReady( void );

	int ItemShouldRespawn( CItem *pItem );

	void RegisterScriptFunctions();

#ifdef GAME_DLL
	bool	NPC_ShouldDropGrenade( CBasePlayer *pRecipient );
	bool	NPC_ShouldDropHealth( CBasePlayer *pRecipient );
	void	NPC_DroppedHealth( void );
	void	NPC_DroppedGrenade( void );
#endif
	
private:
	
	CNetworkVar( bool, m_bTeamPlayEnabled );
	CNetworkVar( bool, m_bCoOpEnabled );
	CNetworkVar( float, m_flGameStartTime );
	CNetworkVar( bool, m_bMegaPhysgun );

	CUtlVector<EHANDLE> m_hRespawnableItemsAndWeapons;
	float m_tmNextPeriodicThink;
	float m_flRestartGameTime;
	bool m_bCompleteReset;
	bool m_bAwaitingReadyRestart;
	bool m_bHeardAllPlayersReady;

#ifdef GAME_DLL
	CBaseEntity* m_pProxy;

	float	m_flLastHealthDropTime;
	float	m_flLastGrenadeDropTime;

public:
	float m_flTransitionTimerEnd;

private:
	bool m_bTransitionTimerOn;
	CChangeLevel* m_pChangeLevel;

	CUtlVector<CEntitySpawnInfo*> m_vecRespawnableEntities;

#if ENGINE_NEW
	CScriptScope	m_ScriptScope;
	HSCRIPT			m_hScriptInstance;
	string_t		m_iszScriptId;
#endif

#if ENGINE_CSGO
	float m_flIntermissionEndTime;
#endif

#endif
};


inline CHL2MPRules* HL2MPRules()
{
	return static_cast<CHL2MPRules*>(g_pGameRules);
}

inline CHL2MPRules* HL2GameRules()
{
	return static_cast<CHL2MPRules*>(g_pGameRules);
}


typedef CHL2MPRules CDemezGameRules;

inline CDemezGameRules* DemezGameRules()
{
	return static_cast<CDemezGameRules*>(g_pGameRules);
}

inline CDemezGameRules* DGameRules()
{
	return static_cast<CDemezGameRules*>(g_pGameRules);
}

