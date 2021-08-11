#pragma once

#include "igamesystem.h"
#include "hl2mp_gamerules.h"
#include "engine_defines.h"


enum class EDemezGame
{
	Invalid,
	// HalfLife,  // will come later
	HalfLife2,
	Episodic,
	Portal
	// Sandbox // maybe?
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

	virtual bool            Init();

	virtual void            LevelInit( const char* mapName );

	// well, this is a mess
	// void                    RegisterGame( CDemezGame* game );
	void                    LevelInitSetGame();
	void                    DetermineGameFromMap();
	void                    SetGame( const char* game, bool warn = false );
	void                    SetGame( EDemezGame game );
	const char*             GetGameName( EDemezGame game = EDemezGame::Invalid );

	void                    ParseMapListFiles();

	EDemezGame              GetGameType();
	CDemezGameRules*        GetGameRules();

#ifdef GAME_DLL
	void                    CreateGameRules();

	void                    ClientPutInServer( edict_t *pEdict, const char *playername );
#endif

	const char*             GetMapName();
	bool                    IsHL2Map();
	bool                    IsEP1Map();
	bool                    IsEP2Map();
	bool                    IsPortalMap();

	bool                    IsHL2();
	// bool                    IsEP1();
	// bool                    IsEP2();
	bool                    IsEpisodic();
	bool                    IsPortal();
	// bool                    IsSandbox();

	// -------------------------------------------------------------
	
	EDemezGame              m_gameType;
	CDemezGameRules*        m_gameRules;
	const char*             m_mapName;

	// -------------------------------------------------------------
	// map lists - maybe actually make that CDemezGame class and have this be a part of it?
	// though ep1 and ep2 maps would be grouped together, thoguh that's not too important
	CUtlVector< const char* > m_mapsHalfLife2;
	CUtlVector< const char* > m_mapsEpisodic;
	CUtlVector< const char* > m_mapsPortal;
};


extern CDemezGameManager g_demezGameMgr;

const char* GetMapName();
CDemezGameManager* DemezGameManager();


