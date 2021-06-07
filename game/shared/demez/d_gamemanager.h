#pragma once

#include "igamesystem.h"
#include "hl2mp_gamerules.h"
#include "engine_defines.h"


enum class EDemezGame
{
	// HL1,
	// PORTAL, // maybe?
	// SANDBOX, // maybe?
	HL2 = 0,
	EPISODIC,
	MAX = EPISODIC
};


/*class CDemezGame
{
public:
	CDemezGame();
	~CDemezGame();

	const char* name;
};*/


class CDemezGameManager: public CAutoGameSystem
{
public:
	CDemezGameManager();
	~CDemezGameManager();

	// -------------------------------------------------------------

	virtual void            LevelInit( const char* mapName );

	// void                    RegisterGame( CDemezGame* game );
	void                    DetermineGame();

	EDemezGame              GetGameType();
	CDemezGameRules*        GetGameRules();

#ifdef GAME_DLL
	void                    CreateGameRules();
#endif

	const char*             GetMapName();
	bool                    IsHL2Map();
	bool                    IsEP1Map();
	bool                    IsEP2Map();

	bool                    IsHL2();
	// bool                    IsEP1();
	// bool                    IsEP2();
	bool                    IsEpisodic();
	// bool                    IsSandbox();

	// -------------------------------------------------------------
	
	EDemezGame              m_gameType;
	CDemezGameRules*        m_gameRules;
	const char*             m_mapName;
};


extern CDemezGameManager g_demezGameMgr;

const char* GetMapName();
CDemezGameManager* DemezGameManager();


