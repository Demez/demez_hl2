#pragma once


#include "weapon_parse.h"
#include "networkvar.h"

#include "baseentity.h"


//--------------------------------------------------------------------------------------------------------
class CRespawnableEntity // : public CBaseEntity
{
	// DECLARE_CLASS( CRespawnableEntity, CBaseEntity )
	DECLARE_CLASS_NOBASE( CRespawnableEntity )

public:
	
	CRespawnableEntity();

	DECLARE_DATADESC()

	void    FallThink( void );

	virtual CBaseEntity* Respawn( void );
	// void ItemTouch( CBaseEntity *pOther );
	virtual void Materialize( void );

	Vector	GetOriginalSpawnOrigin( void ) { return m_vOriginalSpawnOrigin;	}
	QAngle	GetOriginalSpawnAngles( void ) { return m_vOriginalSpawnAngles;	}
	void	SetOriginalSpawnOrigin( const Vector& origin ) { m_vOriginalSpawnOrigin = origin; }
	void	SetOriginalSpawnAngles( const QAngle& angles ) { m_vOriginalSpawnAngles = angles; }

	void    Spawn( void );

	bool	CanRespawn( void );

	void	SetNeedsRespawn( bool enabled = true );

	float   m_flNextResetCheckTime;
	bool    m_bCanRespawn;
	bool    m_bNeedsRespawn;

	Vector		m_vOriginalSpawnOrigin;
	QAngle		m_vOriginalSpawnAngles;

	float   m_flNextRespawnTime;
};



