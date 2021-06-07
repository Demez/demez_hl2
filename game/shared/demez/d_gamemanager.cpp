#include "cbase.h"
#include "d_gamemanager.h"
#include "gamerules_register.h"

#ifdef CLIENT_DLL
	#include "clientmode_hl2mpnormal.h"
#endif


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


static const char *s_ep1Maps[] =
{
	"", // END Marker
};


static const char *s_ep2Maps[] =
{
	"", // END Marker
};


ConVar demez_game("d_game", "hl2", FCVAR_GAMEDLL | FCVAR_ARCHIVE | FCVAR_REPLICATED | FCVAR_NOT_CONNECTED,
				  "options: hl2, episodic" );


CDemezGameManager g_demezGameMgr;


CDemezGameManager* DemezGameManager()
{
	return &g_demezGameMgr;
}


CDemezGameManager::CDemezGameManager(): CAutoGameSystem()
{
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


void CDemezGameManager::LevelInit( const char* mapName )
{
	m_mapName = mapName;
	DetermineGame();
}


void CDemezGameManager::DetermineGame()
{
	if ( IsHL2Map() )
	{
		m_gameType = EDemezGame::HL2;
		demez_game.SetValue( "hl2" );
	}
	else if ( IsEP1Map() || IsEP2Map() )
	{
		m_gameType = EDemezGame::EPISODIC;
		demez_game.SetValue( "episodic" );
	}
	else
	{
		// maybe?
		// Warning( "Invalid Game Type, defaulting to HL2 Episodic" );
		m_gameType = EDemezGame::EPISODIC;
		demez_game.SetValue( "episodic" );
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


extern ConVar demez_game;


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
	CreateGameRulesObject( "CHL2MPRules" );
}
#endif


bool CDemezGameManager::IsHL2()
{
	// return ( V_strcmp( demez_game.GetString(), "hl2" ) == 0 );
	// return IsHL2Map() || IsEP1() || IsEP2();
	return ( m_gameType == EDemezGame::HL2 ) || IsEpisodic();
}

/*bool CDemezGameManager::IsEP1()
{
	// return ( V_strcmp( demez_game.GetString(), "ep1" ) == 0 );
	return IsEP1Map();
}

bool CDemezGameManager::IsEP2()
{
	// return ( V_strcmp( demez_game.GetString(), "ep2" ) == 0 );
	return IsEP2Map();
}*/

bool CDemezGameManager::IsEpisodic()
{
	// return IsEP1() || IsEP2();
	return ( m_gameType == EDemezGame::EPISODIC );
}

/*bool CDemezGameManager::IsSandbox()
{
	// return ( V_strcmp( demez_game.GetString(), "sandbox" ) == 0 );
	return ( m_gameType == EDemezGame::SANDBOX );
}*/


