//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "trigger_checkpoint.h"
#include "hl2mp_gamerules.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


ConVar demez_show_triggers("d_showtriggers", "1");
ConVar demez_chk_player_preview("d_chk_player_preview", "1");

// ConVar demez_chk_player_preview_model("d_chk_player_preview_model", "models/editor/playerstart.mdl");


CBasePointTrigger::CBasePointTrigger():
	m_vecMinSize(0, 0, 0),
	m_vecMaxSize(16, 16, 16)
{
}


void CBasePointTrigger::InitPointTrigger()
{
	SetSolid( SOLID_BBOX );	
	AddSolidFlags( FSOLID_NOT_SOLID );

	if (m_bDisabled)
	{
		RemoveSolidFlags( FSOLID_TRIGGER );	
	}
	else
	{
		AddSolidFlags( FSOLID_TRIGGER );	
	}

	SetMoveType( MOVETYPE_NONE );

	// debugging size (add a callback here?)
	if ( demez_show_triggers.GetBool() )
	{
		m_debugOverlays |= OVERLAY_BBOX_BIT;
	}
	else
	{
		if (m_debugOverlays & OVERLAY_BBOX_BIT)
		{
			m_debugOverlays &= ~OVERLAY_BBOX_BIT;
		}
	}

	m_hTouchingEntities.Purge();

	if ( HasSpawnFlags( SF_TRIG_TOUCH_DEBRIS ) )
	{
		CollisionProp()->AddSolidFlags( FSOLID_TRIGGER_TOUCH_DEBRIS );
	}

	CallSetSize();
}


void CBasePointTrigger::CallSetSize()
{
	UTIL_SetSize( this, m_vecMinSize, m_vecMaxSize );
}


void CBasePointTrigger::SetSize( const Vector& vec )
{
	m_vecMaxSize = vec;
	CallSetSize();
}


void CBasePointTrigger::SetEndOrigin( const Vector& vec )
{
	m_vecMaxSize = GetAbsOrigin() - vec;
	m_vecMaxSize.z = vec.z;  // lazy hack

	if ( m_vecMaxSize.x < 0 )
		m_vecMaxSize.x = m_vecMaxSize.x * -1.0f;

	if ( m_vecMaxSize.y < 0 )
		m_vecMaxSize.y = m_vecMaxSize.y * -1.0f;

	CallSetSize();
}


/*void CBasePointTrigger::SetEndOriginAndHeight( const Vector& vec )
{
	m_vecMaxSize = GetAbsOrigin() - vec;
	m_vecMaxSize.z = vec.z;
	CallSetSize();
}*/


#if ENGINE_NEW
BEGIN_ENT_SCRIPTDESC( CBasePointTrigger, CBaseTrigger, "Point Entity Trigger" )
	// DEFINE_SCRIPTFUNC( SetMinSize, "" )
	DEFINE_SCRIPTFUNC( SetSize, "" )
	DEFINE_SCRIPTFUNC( SetEndOrigin, "" )
	// DEFINE_SCRIPTFUNC( SetEndOriginAndHeight, "" )
END_SCRIPTDESC();
#endif


// ===================================

// Demez: idea: maybe have all checkpoints register in the gamerules so it can disable (or delete) previous checkpoints
// Demez: TODO: move precaching of the spawn model preview to a dedicated precache function in this entity, though then the convar will be useless


// this could all be done in vscript probably lol
CCheckpoint::CCheckpoint()
{
	m_spawnPos.Init();
	// m_spawnAng.Init();

	m_pTrigger = NULL;
	m_pActivator = NULL;

	m_bSpawnPosSet = false;
	m_bTeleportActivator = true;  // temp so i can test the teleports
}


void CCheckpoint::Spawn()
{
	SetSolid( SOLID_NONE );	
	AddSolidFlags( FSOLID_NOT_SOLID );

	if ( demez_chk_player_preview.GetBool() )
	{
		bool precache = IsPrecacheAllowed();
		SetAllowPrecache( true );

		PrecacheModel( "models/editor/playerstart.mdl" );

		SetModel( "models/editor/playerstart.mdl" );
		SetRenderMode( kRenderTransAlpha );
		SetRenderAlpha( 128 );

		SetAllowPrecache( precache );
	}
	else
	{
		AddEffects( EF_NODRAW );
	}

	SetAbsOrigin( m_spawnPos );
	// SetAbsAngles( m_spawnAng );

	HL2MPRules()->RegisterCheckpoint( this );
}


void CCheckpoint::SetSpawnPos( Vector spawnPos )
{
	m_spawnPos = spawnPos;

	if ( GetParent() == NULL )
	{
		SetAbsOrigin( m_spawnPos );
	}
}


void CCheckpoint::ScriptSetOriginOverride( const Vector &v )
{
	Teleport( &v, NULL, NULL );
	m_bSpawnPosSet = true;
}


void CCheckpoint::SetSpawnAng( QAngle spawnAng )
{
	// m_spawnAng = spawnAng;
	SetAbsAngles( spawnAng );
}


Vector CCheckpoint::GetSpawnPos()
{
	m_spawnPos = GetAbsOrigin();
	m_spawnPos.z += 4;
	return m_spawnPos;
}


void CCheckpoint::CreateTrigger()
{
	if ( m_pTrigger != NULL )
		return;

	m_pTrigger = (CTriggerCheckpoint*)CreateEntityByName( "trigger_checkpoint" );
	m_pTrigger->m_pCheckpoint = this;
	DispatchSpawn( m_pTrigger );
}


void CCheckpoint::CheckpointFire()
{
	if ( m_nIndex == -1 )
		return;

	// Notify gamerules that this was passed, so new players spawn here
	if ( !HL2MPRules()->SetCheckpoint( this ) )
		return;

	// should we teleport everyone here?
	if ( m_bTeleport )
	{
		UTIL_FOREACHPLAYER(i)
		{
			UTIL_GETNEXTPLAYER(i);

			TeleportPlayer( pPlayer );
		}
	}
}


void CCheckpoint::TeleportAllPlayers()
{
	if ( !m_bActive )
		return;

	if ( m_bTeleport )
	{
		UTIL_FOREACHPLAYER(i)
		{
			UTIL_GETNEXTPLAYER(i);

			TeleportPlayer( pPlayer );
		}
	}
}


void CCheckpoint::TeleportPlayer( CBasePlayer* pPlayer )
{
	if ( pPlayer == m_pActivator && !m_bTeleportActivator )
		return;

	if ( m_bActive && pPlayer && pPlayer->IsAlive() )
	{
		pPlayer->Teleport( &GetSpawnPos(), &GetAbsAngles(), &Vector(0, 0, 0) );
		pPlayer->SetAbsOrigin( GetSpawnPos() );  // just in case
	}
}


#if ENGINE_NEW
HSCRIPT CCheckpoint::ScriptGetTrigger()
{
	CreateTrigger();
	return ToHScript( m_pTrigger );
}
#endif


void CCheckpoint::SetIndex( int index )
{
	m_nIndex = index;

	// make sure we don't overwrite an existing name
	// if ( GetEntityName().ToCStr() )

	/*if ( m_pTrigger )
	{
		char buf[128];
		V_snprintf( buf, 128, "trigger_checkpoint_%d", m_nIndex );
		m_pTrigger->SetName( MAKE_STRING(buf) );
	}

	char buf[128];
	V_snprintf( buf, 128, "trigger_checkpoint_%d", m_nIndex );
	SetName( MAKE_STRING(buf) );*/
}


void CCheckpoint::SetTeleport( bool teleport )
{
	m_bTeleport = teleport;
}


void CCheckpoint::SetTeleportActivator( bool teleport )
{
	m_bTeleportActivator = teleport;
}


void CCheckpoint::SetTriggerOrigin( const Vector& pos )
{
	CreateTrigger();
	m_pTrigger->SetAbsOrigin( pos );

	if ( !m_bSpawnPosSet )
		CalcSpawn();
}


void CCheckpoint::SetTriggerSize( const Vector& size )
{
	CreateTrigger();
	m_pTrigger->SetSize( size );

	if ( !m_bSpawnPosSet )
		CalcSpawn();
}


void CCheckpoint::CalcSpawn()
{
	if ( m_pTrigger == NULL )
		return;

	Vector spawnPos;
	spawnPos.x = m_pTrigger->GetAbsOrigin().x + (m_pTrigger->m_vecMaxSize.x / 2);
	spawnPos.y = m_pTrigger->GetAbsOrigin().y + (m_pTrigger->m_vecMaxSize.y / 2);
	spawnPos.z = m_pTrigger->GetAbsOrigin().z + 4;

	SetSpawnPos( spawnPos );

	// m_bPosSet = true;
}

void CCheckpoint::InputTeleport( inputdata_t &data )
{
	TeleportAllPlayers();
}

void CCheckpoint::InputSetActive( inputdata_t &data )
{
	CheckpointFire();
}


#if ENGINE_NEW
BEGIN_ENT_SCRIPTDESC( CCheckpoint, CBaseAnimating, "Checkpoint for coop" )
	DEFINE_SCRIPTFUNC( SetIndex, "Set checkpoint number" )
	DEFINE_SCRIPTFUNC( CalcSpawn, "" )
	DEFINE_SCRIPTFUNC( SetTeleport, "" )
	DEFINE_SCRIPTFUNC( SetTeleportActivator, "" )
	DEFINE_SCRIPTFUNC( SetTriggerOrigin, "" )
	DEFINE_SCRIPTFUNC( SetTriggerSize, "" )

	DEFINE_SCRIPTFUNC_NAMED( ScriptGetTrigger, "GetTrigger", "" )
	DEFINE_SCRIPTFUNC_NAMED( ScriptSetOriginOverride, "SetOrigin", "" )
END_SCRIPTDESC();
#endif


BEGIN_DATADESC( CCheckpoint )
	// DEFINE_KEYFIELD( m_bTeleport, FIELD_BOOL, "teleport" ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Teleport", InputTeleport ),
	DEFINE_INPUTFUNC( FIELD_VOID, "SetActive", InputSetActive ),
END_DATADESC()


LINK_ENTITY_TO_CLASS( info_checkpoint, CCheckpoint )


// ==============================================


CTriggerCheckpoint::CTriggerCheckpoint()
{
	m_pCheckpoint = NULL;
	m_pActivator = NULL;
}


void CTriggerCheckpoint::Spawn( void )
{
	BaseClass::Spawn();
	InitPointTrigger();
	SetTouch( &CTriggerCheckpoint::CheckpointTouch );
}

void CTriggerCheckpoint::CallSetSize( void )
{
	BaseClass::CallSetSize();
	// CalcPlayerSpawn();
}


void CTriggerCheckpoint::CheckpointTouch( CBaseEntity *pOther )
{
	if ( m_pCheckpoint == NULL )
	{
		// uhh
		SetTouch( NULL );
		return;
	}

	if ( pOther && !pOther->IsPlayer() )
		return;

	m_pActivator = pOther;
	m_pCheckpoint->m_pActivator = pOther;
	m_pCheckpoint->CheckpointFire();

	SetTouch( NULL );
}


LINK_ENTITY_TO_CLASS( trigger_checkpoint, CTriggerCheckpoint )



