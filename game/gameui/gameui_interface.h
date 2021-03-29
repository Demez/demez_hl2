//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Defines the interface that the GameUI dll exports
//
// $NoKeywords: $
//=============================================================================//

#ifndef GAMEUI_INTERFACE_H
#define GAMEUI_INTERFACE_H
#pragma once

#include "GameUI/IGameUI.h"

#include "vgui_controls/Panel.h"
#include "vgui_controls/PHandle.h"
#include "convar.h"

class IGameClientExports;
class CCommand;

//-----------------------------------------------------------------------------
// Purpose: Implementation of GameUI's exposed interface 
//-----------------------------------------------------------------------------
class CGameUI : public IGameUI
{
public:
	CGameUI();
	~CGameUI();

	virtual void Initialize( CreateInterfaceFn appFactory );
	virtual void Connect( CreateInterfaceFn gameFactory );
	virtual void Start();
	virtual void Shutdown();
	virtual void RunFrame();
	virtual void PostInit();

	// plays the startup mp3 when GameUI starts
	void PlayGameStartupSound();

	// Engine wrappers for activating / hiding the gameUI
	void ActivateGameUI();
	void HideGameUI();

	// Toggle allowing the engine to hide the game UI with the escape key
	void PreventEngineHideGameUI();
	void AllowEngineHideGameUI();

	virtual void SetLoadingBackgroundDialog( vgui::VPANEL panel );

	// notifications
	virtual void OnGameUIActivated();
	virtual void OnGameUIHidden();
	virtual void OLD_OnConnectToServer( const char *game, int IP, int port );	// OLD: use OnConnectToServer2
	virtual void OnConnectToServer2( const char *game, int IP, int connectionPort, int queryPort );
	virtual void OnDisconnectFromServer( uint8 eSteamLoginFailure );
	virtual void OnLevelLoadingStarted( const char *levelName, bool bShowProgressDialog );
	virtual void OnLevelLoadingFinished( bool bError, const char *failureReason, const char *extendedReason );
	virtual void OnDisconnectFromServer_OLD( uint8 eSteamLoginFailure, const char *username ) { OnDisconnectFromServer( eSteamLoginFailure ); }

	// progress
	virtual bool UpdateProgressBar(float progress, const char *statusText);
	virtual bool UpdateProgressBar(float progress, const char *statusText, bool showDialog) { return UpdateProgressBar(progress, statusText); }
	// Shows progress desc, returns previous setting... (used with custom progress bars )
	virtual bool SetShowProgressText( bool show );

	// Allows the level loading progress to show map-specific info
	virtual void SetProgressLevelName( const char *levelName );

 	virtual void NeedConnectionProblemWaitScreen();

	virtual void ShowPasswordUI( char const *pchCurrentPW );

 	virtual void SetProgressOnStart();
 
#if defined( _X360 ) && defined( _DEMO )
	virtual void OnDemoTimeout();
#endif

 	// state
 	bool IsInLevel();
 	bool IsInBackgroundLevel();
 	bool IsInMultiplayer();
 	bool IsConsoleUI();
 	bool HasSavedThisMenuSession();
 	void SetSavedThisMenuSession( bool bState );
 
 	void ShowLoadingBackgroundDialog();
	void HideLoadingBackgroundDialog();
	bool HasLoadingBackgroundDialog();

#if ENGINE_CSGO

	virtual void StartLoadingScreenForCommand( const char* command ) {}
	virtual void StartLoadingScreenForKeyValues( KeyValues* keyValues ) {}
	virtual bool UpdateSecondaryProgressBar(float progress, const wchar_t *desc ) { return false; }  // download bar
												
	 // Xbox 360
	virtual void ShowMessageDialog( const uint nType, vgui::Panel *pOwner = NULL ) {}
	virtual void ShowMessageDialog( const char* messageID, const char* titleID ) {}

	virtual void CreateCommandMsgBox( const char* pszTitle, const char* pszMessage, bool showOk = true, bool showCancel = false, const char* okCommand = NULL, const char* cancelCommand = NULL, const char* closedCommand = NULL, const char* pszLegend = NULL ) {}
	virtual void CreateCommandMsgBoxInSlot( ECommandMsgBoxSlot slot, const char* pszTitle, const char* pszMessage, bool showOk = true, bool showCancel = false, const char* okCommand = NULL, const char* cancelCommand = NULL, const char* closedCommand = NULL, const char* pszLegend = NULL ) {}


	virtual bool LoadingProgressWantsIsolatedRender( bool bContextValid ) { return false; }
	virtual void RestoreTopLevelMenu() {}

	virtual bool IsPlayingFullScreenVideo() { return false; }
	virtual bool IsTransitionEffectEnabled() { return false; }

#else

#endif

private:
	void SendConnectedToGameMessage();

	virtual void StartProgressBar();
	virtual bool ContinueProgressBar(float progressFraction);
	virtual void StopProgressBar(bool bError, const char *failureReason, const char *extendedReason = NULL);
	virtual bool SetProgressBarStatusText(const char *statusText);

	//!! these functions currently not implemented
	virtual void SetSecondaryProgressBar(float progress /* range [0..1] */);
	virtual void SetSecondaryProgressBarText(const char *statusText);

	bool FindPlatformDirectory(char *platformDir, int bufferSize);
	void GetUpdateVersion( char *pszProd, char *pszVer);
	void ValidateCDKey();

	CreateInterfaceFn m_GameFactory;

	bool m_bTryingToLoadFriends : 1;
	bool m_bActivatedUI : 1;
	bool m_bIsConsoleUI : 1;
	bool m_bHasSavedThisMenuSession : 1;
	bool m_bOpenProgressOnStart : 1;

	int m_iGameIP;
	int m_iGameConnectionPort;
	int m_iGameQueryPort;
	
	int m_iFriendsLoadPauseFrames;
	int m_iPlayGameStartupSound;

	char m_szPreviousStatusText[128];
	char m_szPlatformDir[MAX_PATH];

	vgui::DHANDLE<class CCDKeyEntryDialog> m_hCDKeyEntryDialog;
};

// Purpose: singleton accessor
extern CGameUI &GameUI();

// expose client interface
extern IGameClientExports *GameClientExports();

#endif // GAMEUI_INTERFACE_H
