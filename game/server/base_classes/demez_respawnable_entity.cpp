//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "demez_respawnable_entity.h"
#include "hl2mp_gamerules.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


CRespawnableEntity::CRespawnableEntity()
{
	m_bCanRespawn = true;
}


BEGIN_DATADESC_NO_BASE( CRespawnableEntity )

	DEFINE_THINKFUNC( FallThink ),

	// DEFINE_FIELD( m_bActivateWhenAtRest,	 FIELD_BOOLEAN ),
	DEFINE_FIELD( m_vOriginalSpawnOrigin, FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( m_vOriginalSpawnAngles, FIELD_VECTOR ),
	// DEFINE_PHYSPTR( m_pConstraint ),

	// Function Pointers
	// DEFINE_ENTITYFUNC( ItemTouch ),
	// DEFINE_THINKFUNC( Materialize ),
	// DEFINE_THINKFUNC( ComeToRest ),

	// Outputs
	// DEFINE_OUTPUT( m_OnPlayerTouch, "OnPlayerTouch" ),
	// DEFINE_OUTPUT( m_OnCacheInteraction, "OnCacheInteraction" ),

END_DATADESC()


void CRespawnableEntity::Spawn()
{
	SetNeedsRespawn( false );

	// m_vOriginalSpawnOrigin = GetAbsOrigin();
	// m_vOriginalSpawnAngles = GetAbsAngles();

	// HL2MPRules()->AddRespawnableEntity( this );
}


bool CRespawnableEntity::CanRespawn()
{
	return m_bCanRespawn;
}


void CRespawnableEntity::SetNeedsRespawn( bool enabled )
{
	m_bNeedsRespawn = enabled;

	if ( enabled )
	{
		m_flNextRespawnTime = gpGlobals->curtime + 15.0f;
	}
	else
	{
		m_flNextRespawnTime = 0.0f;
	}
}


//-----------------------------------------------------------------------------
// Purpose: Items that have just spawned run this think to catch them when 
//			they hit the ground. Once we're sure that the object is grounded, 
//			we change its solid type to trigger and set it in a large box that 
//			helps the player get it.
//-----------------------------------------------------------------------------
void CRespawnableEntity::FallThink()
{
	/*SetNextThink( gpGlobals->curtime + 0.1f );

	bool shouldMaterialize = false;
	IPhysicsObject *pPhysics = VPhysicsGetObject();

	if ( pPhysics )
	{
		shouldMaterialize = pPhysics->IsAsleep();
	}
	else
	{
		shouldMaterialize = (GetFlags() & FL_ONGROUND) ? true : false;
	}

	if ( shouldMaterialize )
	{
		SetThink ( NULL );

		m_vOriginalSpawnOrigin = GetAbsOrigin();
		m_vOriginalSpawnAngles = GetAbsAngles();

		HL2MPRules()->AddLevelDesignerPlacedObject( this );
	}

#if defined( TF_DLL )
	// We only come here if ActivateWhenAtRest() is never called,
	// which is the case when creating currencypacks in MvM
	if ( !( GetFlags() & FL_ONGROUND ) )
	{
		if ( !GetAbsVelocity().Length() && GetMoveType() == MOVETYPE_FLYGRAVITY )
		{
			// Mr. Game, meet Mr. Hammer.  Mr. Hammer, meet the uncooperative Mr. Physics.
			// Mr. Physics really doesn't want to give our friend the FL_ONGROUND flag.
			// This means our wonderfully helpful radius currency collection code will be sad.
			// So in the name of justice, we ask that this flag be delivered unto him.

			SetMoveType( MOVETYPE_NONE );
			SetGroundEntity( GetWorldEntity() );
		}
	}
	else
	{
		SetThink( &CItem::ComeToRest );
	}
#endif // TF*/
}

CBaseEntity* CRespawnableEntity::Respawn( void )
{
	/*SetTouch( NULL );
	AddEffects( EF_NODRAW );

	// VPhysicsDestroyObject();

	SetMoveType( MOVETYPE_NONE );
	SetSolid( SOLID_BBOX );
	AddSolidFlags( FSOLID_TRIGGER );

	// UTIL_SetOrigin( this, g_pGameRules->VecItemRespawnSpot( this ) );// blip to whereever you should respawn.
	// SetAbsAngles( g_pGameRules->VecItemRespawnAngles( this ) );// set the angles.

	UTIL_SetOrigin( this, m_vOriginalSpawnOrigin );// blip to whereever you should respawn.
	SetAbsAngles( m_vOriginalSpawnAngles );// set the angles.

	UTIL_DropToFloor( this, MASK_SOLID );

	RemoveAllDecals(); //remove any decals

	// hmmm
	SetThink ( &CRespawnableEntity::Materialize );
	// SetNextThink( gpGlobals->curtime + g_pGameRules->FlItemRespawnTime( this ) );
	SetNextThink( gpGlobals->curtime + 15 );
	return this;*/
	return NULL;
}

void CRespawnableEntity::Materialize( void )
{
	// CreateItemVPhysicsObject();

	/*if ( IsEffectActive( EF_NODRAW ) )
	{
		// changing from invisible state to visible.

		EmitSound( "Item.Materialize" );
		RemoveEffects( EF_NODRAW );
		// DoMuzzleFlash();
	}*/

	// SetTouch( &CRespawnableEntity::ItemTouch );
}

