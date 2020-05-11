#ifndef ENGINE_DEFINES_H
#define ENGINE_DEFINES_H
#ifdef _WIN32
#pragma once
#endif

// yes, this is evil
// but tell me, do you want to place about 100 #if ENGINE_ASW statements just so it's not "evil"?
// it would be way messier as well, this is actually a very clean way to do it
#if ENGINE_ASW || ENGINE_CSGO

	#define ENGINE_NEW 1
	#define ENGINE_OLD 0

	#if CLIENT_DLL

		#define SteamUser() steamapicontext->SteamUser()

	#elif GAME_DLL
		#ifdef UTIL_H
			#include "demez_util.h"
			#define UTIL_GetLocalPlayer UTIL_Demez_GetLocalPlayer
		#endif

		#define GetAnimEvent( pEvent ) pEvent->Event()

	#endif

#elif ENGINE_QUIVER || ENGINE_2013

	#define ENGINE_NEW 0
	#define ENGINE_OLD 1

	#if CLIENT_DLL

		#define GetHud() gHUD
		#define GetClientMode() g_pClientMode
		#define GetMapOverView() g_pMapOverview
		#define GetViewPortInterface() gViewPortInterface

		#define GetHudList() m_HudList

	#elif GAME_DLL
		#define LAST_SHARED_ENTITY_CLASS NUM_AI_CLASSES

		#define GetRenderAlpha() GetRenderColor().a
		#define SetRenderAlpha   SetRenderColorA

		#define GetAnimEvent( pEvent ) pEvent->event
	#endif

#endif // ENGINE_QUIVER || ENGINE_2013

#endif // ENGINE_DEFINES_H


