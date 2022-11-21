#include "cbase.h"
#include "d_gamemanager.h"
#include "gamerules_register.h"

#ifdef CLIENT_DLL
	#include "clientmode_hl2mpnormal.h"
#endif


extern ConVar hl2_episodic;


// Demez: TEMP, move to a keyvalues file or a vscript
static const char *s_hl2Maps[] =
{
	"background01",
	"background02",
	"background03",
	"background04",
	"background05",
	"background06",
	"background07",
	"credits",
	"d1_canals_01",
	"d1_canals_01a",
	"d1_canals_02",
	"d1_canals_03",
	"d1_canals_05",
	"d1_canals_06",
	"d1_canals_07",
	"d1_canals_08",
	"d1_canals_09",
	"d1_canals_10",
	"d1_canals_11",
	"d1_canals_12",
	"d1_canals_13",
	"d1_eli_01",
	"d1_eli_02",
	"d1_town_01",
	"d1_town_01a",
	"d1_town_02",
	"d1_town_02a",
	"d1_town_03",
	"d1_town_04",
	"d1_town_05",
	"d1_trainstation_01",
	"d1_trainstation_02",
	"d1_trainstation_03",
	"d1_trainstation_04",
	"d1_trainstation_05",
	"d1_trainstation_06",
	"d2_coast_01",
	"d2_coast_02",
	"d2_coast_03",
	"d2_coast_04",
	"d2_coast_05",
	"d2_coast_07",
	"d2_coast_08",
	"d2_coast_09",
	"d2_coast_10",
	"d2_coast_11",
	"d2_coast_12",
	"d2_prison_01",
	"d2_prison_02",
	"d2_prison_03",
	"d2_prison_04",
	"d2_prison_05",
	"d2_prison_06",
	"d2_prison_07",
	"d2_prison_08",
	"d3_breen_01",
	"d3_c17_01",
	"d3_c17_02",
	"d3_c17_02_camera",
	"d3_c17_03",
	"d3_c17_04",
	"d3_c17_05",
	"d3_c17_06a",
	"d3_c17_06b",
	"d3_c17_07",
	"d3_c17_08",
	"d3_c17_09",
	"d3_c17_10a",
	"d3_c17_10b",
	"d3_c17_11",
	"d3_c17_12",
	"d3_c17_12b",
	"d3_c17_13",
	"d3_citadel_01",
	"d3_citadel_02",
	"d3_citadel_03",
	"d3_citadel_04",
	"d3_citadel_05",
	"", // END Marker
};


static const char *s_portalMaps[] =
{
	"background1",
	"background2",

	"testchmb_a_00",
	"testchmb_a_01",
	"testchmb_a_02",
	"testchmb_a_03",
	"testchmb_a_04",
	"testchmb_a_05",
	"testchmb_a_06",
	"testchmb_a_07",
	"testchmb_a_08",
	"testchmb_a_09",
	"testchmb_a_10",
	"testchmb_a_11",
	"testchmb_a_13",
	"testchmb_a_14",
	"testchmb_a_15",

	"testchmb_a_08_advanced",
	"testchmb_a_09_advanced",
	"testchmb_a_10_advanced",
	"testchmb_a_11_advanced",
	"testchmb_a_13_advanced",
	"testchmb_a_14_advanced",

	"escape_00",
	"escape_01",
	"escape_02",

	"", // END Marker
};


static const char *s_ep1Maps[] =
{
	"", // END Marker
};


static const char *s_ep2Maps[] =
{
	"", // END Marker
};


CDemezGameManager g_demezGameMgr;


CON_COMMAND_F( d_game, "set the game, leave blank for auto determining it", FCVAR_ARCHIVE | FCVAR_REPLICATED )
// CON_COMMAND( d_game, "set the game, leave blank for auto determining it" )
{
	if ( args.ArgC() != 2 )
	{
		Msg("Current game is \"%s\", options: auto, hl2, episodic, portal\n", g_demezGameMgr.GetGameName());
		return;
	}

	g_demezGameMgr.SetGame( args[1], true );
}


CDemezGameManager* DemezGameManager()
{
	return &g_demezGameMgr;
}


CDemezGameManager::CDemezGameManager(): CAutoGameSystem()
{
	m_mapName = "";
	m_gameType = EDemezGame::Invalid;
}

CDemezGameManager::~CDemezGameManager()
{
}


static bool FindInList( const char **pStrings, const char *pToFind )
{
	int i = 0;
	while ( pStrings[i][0] != 0 )
	{
		if ( Q_stricmp( pStrings[i], pToFind ) == 0 )
			return true;
		i++;
	}

	return false;
}


/*static const char* GetMapName()
{
#ifdef CLIENT_DLL
#if ENGINE_CSGO
	return GetClientModeHL2MPNormal()->GetMapNameChar();
#else
	return GetClientModeHL2MPNormal()->GetMapName();
#endif
#else
	return gpGlobals->mapname.ToCStr();
#endif
}*/


const char* CDemezGameManager::GetMapName()
{
	return m_mapName;
}


bool CDemezGameManager::Init()
{
	ParseMapListFiles();

	return true;
}


void CDemezGameManager::LevelInit( const char* mapName )
{
	m_mapName = mapName;
	LevelInitSetGame();
}


void CDemezGameManager::ParseMapListFiles()
{
	// TODO: read the keyvalues file "resource/demez/maplist.res" in all search paths and add them to the vectors
}


void CDemezGameManager::LevelInitSetGame()
{
	/*if ( V_strcmp(demez_game.GetString(), "") == 0 )
	{
		SetGame( demez_game.GetString(), true );
	}
	if ( V_strcmp(m_gameTypeStr, "") != 0 )
	{
		SetGame( m_gameTypeStr, false );
	}
	else*/

	// if ( m_gameType == EDemezGame::Invalid )
	{
		DetermineGameFromMap();
	}
}

static bool s_warn = true;

const char* CDemezGameManager::GetGameName( EDemezGame game )
{
	if (game == EDemezGame::Invalid)
		game = m_gameType;

	switch (game)
	{
		case EDemezGame::HalfLife2:		return "hl2";
		case EDemezGame::Portal:		return "portal";
		case EDemezGame::Episodic:		return "episodic";
		default:						return "";
	}
}


void CDemezGameManager::SetGame( const char* game, bool warn )
{
	if ( V_strcmp(game, "auto") == 0 || V_strcmp(game, "") == 0 )
	{
		Msg( "Game will be determined on map load\n" );
		m_gameType = EDemezGame::Invalid;
		return;
	}

	if ( V_strcmp(game, GetGameName()) == 0 )
	{
		return;
	}

	if ( V_strcmp(game, "hl2") == 0 )
	{
		m_gameType = EDemezGame::HalfLife2;
		hl2_episodic.SetValue(0);
	}
	else if ( V_strcmp(game, "portal") == 0 )
	{
		m_gameType = EDemezGame::Portal;
		hl2_episodic.SetValue(1);
	}
	else if ( V_strcmp(game, "episodic") == 0 )
	{
		m_gameType = EDemezGame::Episodic;
		hl2_episodic.SetValue(1);
	}
	else
	{
		Warning( "Invalid Game Type, Game will be determined on map load\n" );
		m_gameType = EDemezGame::Invalid;
		DetermineGameFromMap();
		return;
	}
	
	Msg( "Game set to \"%s\"\n", game );
}


void CDemezGameManager::SetGame( EDemezGame game )
{
	// TODO: add a bounds check here
	m_gameType = game;

	if ( game == EDemezGame::HalfLife2 )
	{
		hl2_episodic.SetValue(0);
	}
	else
	{
		hl2_episodic.SetValue(1);
	}
}


void CDemezGameManager::DetermineGameFromMap()
{
	// only need this if the game isn't set by the player
	if ( m_gameType != EDemezGame::Invalid )
		return;

	if ( V_strcmp(GetMapName(), "") == 0 )
		return;

	if ( IsHL2Map() )
	{
		SetGame( EDemezGame::HalfLife2 );
	}
	else if ( IsPortalMap() )
	{
		SetGame( EDemezGame::Portal );
	}
	else if ( IsEP1Map() || IsEP2Map() )
	{
		SetGame( EDemezGame::Episodic );
	}
	else
	{
		Warning( "Unknown Game Type, Defaulting to HL2 Episodic" );
		SetGame( EDemezGame::Episodic );
	}
}


bool CDemezGameManager::IsHL2Map()
{
	return FindInList( s_hl2Maps, GetMapName() );
}

bool CDemezGameManager::IsEP1Map()
{
	return FindInList( s_ep1Maps, GetMapName() );
}

bool CDemezGameManager::IsEP2Map()
{
	// funny lazy default
	//return FindInList( s_ep2Maps, GetMapName() );
	return true;
}

bool CDemezGameManager::IsPortalMap()
{
	return FindInList( s_portalMaps, GetMapName() );
}


EDemezGame CDemezGameManager::GetGameType()
{
	return m_gameType;
}


CDemezGameRules* CDemezGameManager::GetGameRules()
{
	return m_gameRules;
}


#ifdef GAME_DLL
void CDemezGameManager::CreateGameRules()
{
	// TODO: clean up the CHL2MPRules class, make a base CDemezGameRules class, and then have each game have their own gamerules

	if ( m_gameType == EDemezGame::Portal )
	{
		CreateGameRulesObject( "CPortalGameRules" );
	}
	else
	{
		CreateGameRulesObject( "CHL2MPRules" );
	}
}


void CDemezGameManager::ClientPutInServer( edict_t *pEdict, const char *playername )
{
	CBasePlayer* pPlayer = NULL;

	// DEMEZ TODO: this has to always be true right now due to CPortalGameMovement expecting this
	// gonna have to see if i can have that be determined here as well
#ifdef PORTAL_DLL
	if ( true ) // IsPortal() )
	{
		pPlayer = CHL2MP_Player::CreatePlayer( "player_portal", pEdict );
	}
#endif

	// fallback
	if ( pPlayer == NULL )
	{
		pPlayer = CHL2MP_Player::CreatePlayer( "player", pEdict );
	}

	pPlayer->SetClassname( "player" );
	pPlayer->SetPlayerName( playername );
}
#endif

//inline bool CDemezGameManager::IsHL2()        { return ( m_gameType == EDemezGame::HalfLife2 ) || IsEpisodic(); }
//inline bool CDemezGameManager::IsEpisodic()   { return ( m_gameType == EDemezGame::Episodic ); }
//inline bool CDemezGameManager::IsPortal()     { return ( m_gameType == EDemezGame::Portal ); }


