//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
/*

===== tf_client.cpp ========================================================

  HL2 client/server game specific stuff

*/

#include "cbase.h"
#include "hl2mp_player.h"
#include "hl2mp_gamerules.h"
#include "gamerules.h"
#include "teamplay_gamerules.h"
#include "entitylist.h"
#include "physics.h"
#include "game.h"
#include "player_resource.h"
#include "engine/IEngineSound.h"
#include "team.h"
#include "viewport_panel_names.h"

#include "tier0/vprof.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

void Host_Say( edict_t *pEdict, bool teamonly );

#if ENGINE_OLD
extern CBaseEntity*	FindPickerEntityClass( CBasePlayer *pPlayer, char *classname );
#endif
extern bool			g_fGameOver;

ConVar	deathmatch("deathmatch", "0", FCVAR_NOTIFY, "Running a deathmatch server.");
ConVar	coop("coop", "1", FCVAR_NOTIFY, "Cooperative play.");

void FinishClientPutInServer( CHL2MP_Player *pPlayer, bool bLoadGame )
{
	pPlayer->InitialSpawn();

	// will need to check if this player is outside the transition area, oof
	if (!bLoadGame)
	{
		pPlayer->Spawn();
	}

	char sName[128];
	Q_strncpy( sName, pPlayer->GetPlayerName(), sizeof( sName ) );
	
	// First parse the name and remove any %'s
	for ( char *pApersand = sName; pApersand != NULL && *pApersand != 0; pApersand++ )
	{
		// Replace it with a space
		if ( *pApersand == '%' )
				*pApersand = ' ';
	}

	// notify other clients of player joining the game
	UTIL_ClientPrintAll( HUD_PRINTNOTIFY, "#Game_connected", sName[0] != 0 ? sName : "<unconnected>" );

	if ( HL2MPRules()->IsTeamplay() == true )
	{
		ClientPrint( pPlayer, HUD_PRINTTALK, "You are on team %s1\n", pPlayer->GetTeam()->GetName() );
	}

	/*if (!bLoadGame)
	{
		const ConVar *hostname = cvar->FindVar( "hostname" );
		const char *title = (hostname) ? hostname->GetString() : "MESSAGE OF THE DAY";

		KeyValues *data = new KeyValues("data");
		data->SetString( "title", title );		// info panel title
		data->SetString( "type", "1" );			// show userdata from stringtable entry
		data->SetString( "msg",	"motd" );		// use this stringtable entry

		pPlayer->ShowViewPortPanel( PANEL_INFO, true, data );

		data->deleteThis();
	}*/
}

/*
===========
ClientPutInServer

called each time a player is spawned into the game
============
*/
void ClientPutInServer( edict_t *pEdict, const char *playername )
{
	// Allocate a Player for pev, and call spawn
	CHL2MP_Player *pPlayer = CHL2MP_Player::CreatePlayer( "player", pEdict );
	pPlayer->SetPlayerName( playername );
}


void ClientActive( edict_t *pEdict, bool bLoadGame )
{
	CHL2MP_Player *pPlayer = ToHL2MPPlayer( CBaseEntity::Instance( pEdict ) );
	FinishClientPutInServer( pPlayer, bLoadGame );
}


/*
===============
const char *GetGameDescription()

Returns the descriptive name of this .dll.  E.g., Half-Life, or Team Fortress 2
===============
*/
const char *GetGameDescription()
{
	if ( g_pGameRules ) // this function may be called before the world has spawned, and the game rules initialized
		return g_pGameRules->GetGameDescription();
	else
		return "Half-Life 2 Deathmatch";
}

//-----------------------------------------------------------------------------
// Purpose: Given a player and optional name returns the entity of that 
//			classname that the player is nearest facing
//			
// Input  :
// Output :
//-----------------------------------------------------------------------------
CBaseEntity* FindEntity( edict_t *pEdict, char *classname)
{
	// If no name was given set bits based on the picked
	if (FStrEq(classname,"")) 
	{
		CBasePlayer *pPlayer = static_cast<CBasePlayer*>(GetContainingEntity(pEdict));
		if ( pPlayer )
		{
			return pPlayer->FindPickerEntityClass( classname );
		}
	}
	return NULL;
}

void ClientFullyConnect(edict_t *pEntity)
{

}

//-----------------------------------------------------------------------------
// Purpose: Precache game-specific models & sounds
//-----------------------------------------------------------------------------
void ClientGamePrecache( void )
{
	CBaseEntity::PrecacheModel("models/player.mdl");
	CBaseEntity::PrecacheModel( "models/gibs/agibs.mdl" );
	CBaseEntity::PrecacheModel ("models/weapons/v_hands.mdl");

	CBaseEntity::PrecacheScriptSound( "HUDQuickInfo.LowAmmo" );
	CBaseEntity::PrecacheScriptSound( "HUDQuickInfo.LowHealth" );

	CBaseEntity::PrecacheScriptSound( "FX_AntlionImpact.ShellImpact" );
	CBaseEntity::PrecacheScriptSound( "Missile.ShotDown" );
	CBaseEntity::PrecacheScriptSound( "Bullets.DefaultNearmiss" );
	CBaseEntity::PrecacheScriptSound( "Bullets.GunshipNearmiss" );
	CBaseEntity::PrecacheScriptSound( "Bullets.StriderNearmiss" );
	
	CBaseEntity::PrecacheScriptSound( "Geiger.BeepHigh" );
	CBaseEntity::PrecacheScriptSound( "Geiger.BeepLow" );
}


// called by ClientKill and DeadThink
void respawn( CBaseEntity *pEdict, bool fCopyCorpse )
{
	CHL2MP_Player *pPlayer = ToHL2MPPlayer( pEdict );

	if ( pPlayer )
	{
		if ( gpGlobals->curtime > pPlayer->GetDeathTime() + DEATH_ANIMATION_TIME )
		{		
			// respawn player
			pPlayer->Spawn();			
		}
		else
		{
			pPlayer->SetNextThink( gpGlobals->curtime + 0.1f );
		}
	}
}

void GameStartFrame( void )
{
	VPROF("GameStartFrame()");
	if ( g_fGameOver )
		return;

	teamplay.SetValue(demez_gamemode.GetInt() == DEMEZ_GAMEMODE_TEAMPLAY);
	coop.SetValue(demez_gamemode.GetInt() == DEMEZ_GAMEMODE_COOP);
	deathmatch.SetValue(demez_gamemode.GetInt() == DEMEZ_GAMEMODE_DEATHMATCH);

	gpGlobals->teamplay =	(demez_gamemode.GetInt() == DEMEZ_GAMEMODE_TEAMPLAY);
	gpGlobals->coop =		(demez_gamemode.GetInt() == DEMEZ_GAMEMODE_COOP);
	gpGlobals->deathmatch = (demez_gamemode.GetInt() == DEMEZ_GAMEMODE_DEATHMATCH);

#ifdef DEBUG
	extern void Bot_RunAll();
	Bot_RunAll();
#endif
}

//=========================================================
// instantiate the proper game rules object
//=========================================================
void InstallGameRules()
{
	// i can have separate gamerules and pick which one here, interesting...
	// could use for coop, deathmatch, and team deathmatch
	CreateGameRulesObject( "CHL2MPRules" );
}

