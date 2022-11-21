//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//=============================================================================//
#if !defined( CLIENTMODE_HLNORMAL_H )
#define CLIENTMODE_HLNORMAL_H
#ifdef _WIN32
#pragma once
#endif

#include "clientmode_shared.h"
#include <vgui_controls/EditablePanel.h>
#include <vgui/Cursor.h>
#include "engine_defines.h"

class CHudViewport;

namespace vgui
{
	typedef unsigned long HScheme;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class ClientModeHL2MPNormal : public ClientModeShared
{
public:
	DECLARE_CLASS( ClientModeHL2MPNormal, ClientModeShared );

	ClientModeHL2MPNormal();
	~ClientModeHL2MPNormal();

	virtual void        Init();
	virtual int         GetDeathMessageStartHeight( void );
	virtual void        InitViewport();

	virtual void        LevelInit( const char *newmap );

#if ENGINE_NEW
	virtual void        DoPostScreenSpaceEffects( const CViewSetup *pSetup ) {}
	virtual void        FireGameEvent( IGameEvent *event );
#endif

#if ENGINE_CSGO
	virtual void OnColorCorrectionWeightsReset() {}
	virtual float GetColorCorrectionScale() const { return 0.0f; }
#endif

#if ENGINE_CSGO
	inline const char* GetMapNameChar() override { return mapName; }
#elif ENGINE_ASW
	inline const char* GetMapName() override { return mapName; }
#else
	inline wchar_t* GetMapName() override { return mapName; }
#endif

#if ENGINE_NEW
	const char* mapName;
#else
	wchar_t* mapName;
#endif
};

extern IClientMode *GetClientModeNormal();
extern vgui::HScheme g_hVGuiCombineScheme;

extern ClientModeHL2MPNormal* GetClientModeHL2MPNormal();

#endif // CLIENTMODE_HLNORMAL_H
