//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Demez Ammo Def (cross engine only thing tbh)
//
//=============================================================================//

#include "cbase.h"
#include "demez_ammodef.h"

CDemezAmmoDef::CDemezAmmoDef()
{
}

CDemezAmmoDef::~CDemezAmmoDef()
{
}

int	CDemezAmmoDef::MaxCarry(int nAmmoIndex, const CBaseCombatCharacter *owner)
{
#if ENGINE_NEW
	return BaseClass::MaxCarry(nAmmoIndex, owner);
#else
	return BaseClass::MaxCarry(nAmmoIndex);
#endif
}

