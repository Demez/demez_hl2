//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#include "cbase.h"
#include "ivmodemanager.h"
#include "clientmode_hl2mpnormal.h"
#include "panelmetaclassmgr.h"
#include "vr_clientmode.h"

#if ENGINE_NEW
#include "c_gameinstructor.h"
#include "c_baselesson.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar default_fov("default_fov", "90", FCVAR_ARCHIVE | FCVAR_USERINFO, "Sets the base field-of-view.");
ConVar demez_fov("demez_fov", "90", FCVAR_ARCHIVE | FCVAR_USERINFO, "Sets the base field-of-view.");

// The current client mode. Always ClientModeNormal in HL.
#if ENGINE_NEW
	static IClientMode *g_pClientMode[ MAX_SPLITSCREEN_PLAYERS ];
	IClientMode *GetClientMode()
	{
		ASSERT_LOCAL_PLAYER_RESOLVABLE();
		return g_pClientMode[ GET_ACTIVE_SPLITSCREEN_SLOT() ];
	}

#else
	IClientMode *g_pClientMode = NULL;
#endif

#define SCREEN_FILE		"scripts/vgui_screens.txt"

class CHLModeManager : public CVRModeManager
{
public:
				CHLModeManager( void );
	virtual		~CHLModeManager( void );

	virtual void	Init( void );
	virtual void	SwitchMode( bool commander, bool force );
	virtual void	OverrideView( CViewSetup *pSetup );
	virtual void	CreateMove( float flInputSampleTime, CUserCmd *cmd );
	virtual void	LevelInit( const char *newmap );
	virtual void	LevelShutdown( void );

#if ENGINE_NEW
	virtual void	DoPostScreenSpaceEffects( const CViewSetup *pSetup );
#endif
};

CHLModeManager::CHLModeManager( void )
{
}

CHLModeManager::~CHLModeManager( void )
{
}

void CHLModeManager::Init( void )
{
#if ENGINE_NEW
	for (int i = 0; i < MAX_SPLITSCREEN_PLAYERS; ++i)
	{
		ACTIVE_SPLITSCREEN_PLAYER_GUARD(i);
		g_pClientMode[i] = GetClientModeNormal();
	}
#else
	g_pClientMode = GetClientModeNormal();
#endif
	PanelMetaClassMgr()->LoadMetaClassDefinitionFile( SCREEN_FILE );
}

void CHLModeManager::SwitchMode( bool commander, bool force )
{
}

void CHLModeManager::OverrideView( CViewSetup *pSetup )
{
}

void CHLModeManager::CreateMove( float flInputSampleTime, CUserCmd *cmd )
{
}

void CHLModeManager::LevelInit( const char *newmap )
{
	GetClientMode()->LevelInit( newmap );
}

void CHLModeManager::LevelShutdown( void )
{
	GetClientMode()->LevelShutdown();
}

#if ENGINE_NEW
void CHLModeManager::DoPostScreenSpaceEffects( const CViewSetup *pSetup )
{
}

// just gonna shove this here
void CScriptedIconLesson::Mod_PreReadLessonsFromFile( void )
{
}

bool CScriptedIconLesson::Mod_ProcessElementAction( int iAction, bool bNot, const char *pchVarName, EHANDLE &hVar, const CGameInstructorSymbol *pchParamName, float fParam, C_BaseEntity *pParam, const char *pchParam, bool &bModHandled )
{
	return false;
}

bool C_GameInstructor::Mod_HiddenByOtherElements( void )
{
	return false;
}
#endif


static CHLModeManager g_HLModeManager;
IVModeManager *modemanager = &g_HLModeManager;
