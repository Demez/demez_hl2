//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Misc utility code.
//
// $NoKeywords: $
//=============================================================================//

#ifndef DEMEZ_UTIL_H
#define DEMEZ_UTIL_H
#ifdef _WIN32
#pragma once
#endif

#include "util.h"
#include "engine_defines.h"

#if ENGINE_NEW


CBasePlayer* UTIL_Demez_GetLocalPlayer( void );

// multiplayer use
CBasePlayer* UTIL_GetNearestPlayer(const Vector& origin);
CBasePlayer* UTIL_GetNearestPlayerPreferVisible(CBaseEntity* pLooker, int mask = MASK_SOLID_BRUSHONLY);
CBasePlayer* UTIL_GetNearestVisiblePlayer(CBaseEntity* pLooker, int mask = MASK_SOLID_BRUSHONLY);

// Return true if the any player is looking at the entity.
bool UTIL_IsAnyPlayerLookingAtEntity(CBaseEntity* pEntity);
#endif


#endif // DEMEZ_UTIL_H
