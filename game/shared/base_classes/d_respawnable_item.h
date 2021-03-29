//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#pragma once


#include "weapon_parse.h"
#include "networkvar.h"


#ifdef CLIENT_DLL
	#include "c_baseentity.h"

	#define CRespawnableItem C_RespawnableItem
	#define CBaseEntity C_BaseEntity
#else
	
#endif


//--------------------------------------------------------------------------------------------------------
class CRespawnableItem: public CBaseEntity
{
	DECLARE_CLASS( CRespawnableItem, CBaseEntity )

public:
	
	CRespawnableItem();

#ifdef GAME_DLL

	DECLARE_DATADESC()

	void    FallThink( void );

	virtual CBaseEntity* Respawn( void );
	// void ItemTouch( CBaseEntity *pOther );
	virtual void Materialize( void );

	void    Spawn( void );

	bool	CanRespawn( void );

#endif

	float   m_flNextResetCheckTime;
	bool    m_bCanRespawn;

	Vector		m_vOriginalSpawnOrigin;
	QAngle		m_vOriginalSpawnAngles;
};



