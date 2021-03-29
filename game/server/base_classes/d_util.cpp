#include "cbase.h"
#include "util.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//
// Return the nearest available player.
// source: https://github.com/whoozzem/SecobMod
// pPlayer = UTIL_GetNearestPlayer(GetAbsOrigin());
// 
CBasePlayer* UTIL_GetNearestPlayer(const Vector& origin)
{
	// No point in doing distance tests in singleplayer, when it will always return the same player
	if ( gpGlobals->maxClients == 1 )
	{
		return UTIL_GetLocalPlayer();
	}

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
	// No point in doing visibilty tests in singleplayer, when it will always return the same player
	if ( gpGlobals->maxClients == 1 )
	{
		return UTIL_GetLocalPlayer();
	}

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
// This can return NULL if the looker can't see any players
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
bool UTIL_IsAnyPlayerLookingAtEntity( CBaseEntity* pEntity )
{
	return UTIL_IsAnyPlayerLookingAtEntity( pEntity->WorldSpaceCenter() );
}


bool UTIL_IsAnyPlayerLookingAtEntity( const Vector &vecSpot )
{
	for (int i = 1; i <= gpGlobals->maxClients; i++)
	{
		CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);
		if (pPlayer && pPlayer->FInViewCone(vecSpot))
			return true;
	}

	return false;
}


bool UTIL_IsAnyPlayerVisible( CBaseEntity* pEntity )
{
	return UTIL_IsAnyPlayerLookingAtEntity( pEntity->WorldSpaceCenter() );
}


bool UTIL_IsAnyPlayerVisible( const Vector &vecSpot )
{
	for (int i = 1; i <= gpGlobals->maxClients; i++)
	{
		CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);
		if (pPlayer && pPlayer->FVisible(vecSpot))
			return true;
	}

	return false;
}


bool UTIL_AnyPlayerHasFlashlightOn()
{
	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);
		if ( !pPlayer )
			continue;

		if ( pPlayer->FlashlightIsOn() )
			return true;
	}

	return false;
}


// checks if any player is illuminating this entity
bool UTIL_IsIlluminatedByFlashlight( CBaseEntity *pEntity, float *flReturnDot )
{
	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);
		if ( !pPlayer )
			continue;

		if ( !pPlayer->FlashlightIsOn() )
			continue;

		if ( pPlayer->IsIlluminatedByFlashlight( pEntity, flReturnDot ) )
			return true;
	}

	return false;
}


//
// Get the nearest player to a player that called the command
//
CBasePlayer* UTIL_GetOtherNearestPlayer(const Vector& origin)
{
	float distToOtherNearest = 128.0f; //4WH - Information: We don't want the OtherNearest player to be the player that called this function.
	CBasePlayer* pOtherNearest = NULL;

	for (int i = 1; i <= gpGlobals->maxClients; i++)
	{
		CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);
		if (!pPlayer)
			continue;

		float flDist = (pPlayer->GetAbsOrigin() - origin).LengthSqr();
		if (flDist >= distToOtherNearest)

		{
			pOtherNearest = pPlayer;
			distToOtherNearest = flDist;

		}
	}

	return pOtherNearest;
}
