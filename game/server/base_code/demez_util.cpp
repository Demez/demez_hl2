//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: New Utility code for Demez HL2
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "util.h"
#include "demez_util.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// these are already defined in your moms house, so whatever
#if ENGINE_NEW

//
// Return any player.
// 
CBasePlayer *UTIL_Demez_GetLocalPlayer( void )
{
	CBasePlayer* pHost = UTIL_GetListenServerHost();
	if (pHost)
		return pHost;

	for (int i = 1; i <= gpGlobals->maxClients; i++)
	{
		CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);
		if (pPlayer)
			return pPlayer;
	}

	return NULL;
}

//
// Return the nearest available player.
// source: https://github.com/whoozzem/SecobMod
// pPlayer = UTIL_GetNearestPlayer(GetAbsOrigin());
// 
CBasePlayer* UTIL_GetNearestPlayer(const Vector& origin)
{
	float distToNearest = 9999999999999999999.0f;
	CBasePlayer* pNearest = NULL;

	for (int i = 1; i <= gpGlobals->maxClients; i++)
	{
		CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);
		if (!pPlayer)
			continue;

		float flDist = (pPlayer->GetAbsOrigin() - origin).LengthSqr();
		if (flDist < distToNearest)
		{
			pNearest = pPlayer;
			distToNearest = flDist;
		}
	}

	return pNearest;
}

//
// Return the nearest visible player if we found one, otherwise return the nearest player
// 
CBasePlayer* UTIL_GetNearestPlayerPreferVisible(CBaseEntity* pLooker, int mask)
{
	float distToNearest = 9999999999999999999.0f;
	float distToNearestNonVisible = 9999999999999999999.0f;
	CBasePlayer* pNearest = NULL;
	CBasePlayer* pNearestNonVisible = NULL;

	for (int i = 1; i <= gpGlobals->maxClients; i++)
	{
		CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);
		if (!pPlayer)
			continue;

		float flDist = (pPlayer->GetAbsOrigin() - pLooker->GetAbsOrigin()).LengthSqr();
		if (flDist < distToNearest && pLooker->FVisible(pPlayer, mask))
		{
			pNearest = pPlayer;
			distToNearest = flDist;
		}
		else if (flDist < distToNearestNonVisible)
		{
			pNearestNonVisible = pPlayer;
			distToNearestNonVisible = flDist;
		}
	}

	if (pNearest)
		return pNearest;
	return pNearestNonVisible;
}

//
// Return the nearest visible player.
// 
CBasePlayer* UTIL_GetNearestVisiblePlayer(CBaseEntity* pLooker, int mask)
{
	float distToNearest = 9999999999999999999.0f;
	CBasePlayer* pNearest = NULL;

	for (int i = 1; i <= gpGlobals->maxClients; i++)
	{
		CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);
		if (!pPlayer)
			continue;

		float flDist = (pPlayer->GetAbsOrigin() - pLooker->GetAbsOrigin()).LengthSqr();
		if (flDist < distToNearest && pLooker->FVisible(pPlayer, mask))  // only difference
		{
			pNearest = pPlayer;
			distToNearest = flDist;
		}
	}

	return pNearest;
}

//
// Return true if the any player is looking at the entity.
// 
bool UTIL_IsAnyPlayerLookingAtEntity(CBaseEntity* pEntity)
{
	for (int i = 1; i <= gpGlobals->maxClients; i++)
	{
		CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);
		if (pPlayer && pPlayer->FInViewCone(pEntity))
			return true;
	}

	return false;
}

#endif // ENGINE_NEW