//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Demez Base Items
//
//=============================================================================//

#include "cbase.h"
#include "demez_items.h"
#include "hl2mp_gamerules.h"

CDemezItem::CDemezItem()
{
}

CDemezItem::~CDemezItem()
{
}

BEGIN_DATADESC( CDemezItem )
	DEFINE_FIELD( m_flNextResetCheckTime, FIELD_TIME ),
	DEFINE_THINKFUNC( FallThink ),
END_DATADESC()

void CDemezItem::Spawn()
{
	BaseClass::Spawn();

	SetThink( &CDemezItem::FallThink );
	SetNextThink( gpGlobals->curtime + 0.1f );
}


//-----------------------------------------------------------------------------
// Purpose: Items that have just spawned run this think to catch them when 
//			they hit the ground. Once we're sure that the object is grounded, 
//			we change its solid type to trigger and set it in a large box that 
//			helps the player get it.
//-----------------------------------------------------------------------------
void CDemezItem::FallThink()
{
	SetNextThink( gpGlobals->curtime + 0.1f );

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
#endif // TF
}


