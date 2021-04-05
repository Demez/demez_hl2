#pragma once


#include "weapon_parse.h"
#include "networkvar.h"

#include "baseentity.h"
#include "triggers.h"
#include "props.h"
#include "engine_defines.h"



// CPointEntity

class CBasePointTrigger : public CBaseTrigger
{
	DECLARE_CLASS( CBasePointTrigger, CBaseTrigger )

public:
	DECLARE_ENT_SCRIPTDESC();

	CBasePointTrigger();

	virtual void InitPointTrigger();

	virtual void CallSetSize();
	virtual void SetSize( const Vector& vec );
	virtual void SetEndOrigin( const Vector& vec );

	Vector m_vecMinSize;
	Vector m_vecMaxSize;
};


//--------------------------------------------------------------------------------------------------------
class CTriggerCheckpoint;

class CCheckpoint: public CBaseAnimating
{
	DECLARE_CLASS( CCheckpoint, CBaseAnimating )

public:
	// vscript stuff
	DECLARE_ENT_SCRIPTDESC();

	// sava data and hammer i/o
	DECLARE_DATADESC();

	CCheckpoint();

	void Spawn();

	Vector GetSpawnPos();

	void SetSpawnPos( Vector spawnPos );
	void SetSpawnAng( QAngle spawnAng );
	void CalcSpawn();

	void ScriptSetOriginOverride( const Vector &v );

	void CreateTrigger();
	void SetTriggerOrigin( const Vector& pos );  // dumb
	void SetTriggerSize( const Vector& size );  // dumb

	void CheckpointFire();

	void TeleportPlayer( CBasePlayer *pPlayer );
	void TeleportAllPlayers();
	
#if ENGINE_NEW
	HSCRIPT ScriptGetTrigger();
#endif

	void SetIndex( int index );
	void SetTeleport( bool teleport );
	void SetTeleportActivator( bool teleport );

	void InputTeleport( inputdata_t &data );
	void InputSetActive( inputdata_t &data );

	int m_nIndex;  // checkpoint index
	bool m_bTeleport = true;
	bool m_bTeleportActivator = false;

	bool m_bActive;  // is this the currently active checkpoint?


	CTriggerCheckpoint* m_pTrigger;  // why
	CBaseEntity* m_pActivator;

	bool m_bSpawnPosSet;  // dumb check for if we should call CalcSpawn() automatically or not
	Vector m_spawnPos;
	// QAngle m_spawnAng;
};

//--------------------------------------------------------------------------------------------------------

/*class CPointTriggerOnce : public CBasePointTrigger
{
	DECLARE_CLASS( CPointTriggerOnce, CBasePointTrigger )

public:
	// vscript stuff
	DECLARE_ENT_SCRIPTDESC();

	// data description
	DECLARE_DATADESC();

	CPointTriggerOnce();

	void Spawn();
	void CallSetSize();
};*/


//--------------------------------------------------------------------------------------------------------
// Old
class CTriggerCheckpoint : public CBasePointTrigger
{
	DECLARE_CLASS( CTriggerCheckpoint, CBasePointTrigger )

public:
	CTriggerCheckpoint();

	void Spawn();
	void CallSetSize();

	void CheckpointTouch( CBaseEntity *pOther );

	CBaseEntity* m_pActivator;
	CCheckpoint* m_pCheckpoint;
};




