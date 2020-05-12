
#ifndef DEMEZ_AMMODEF_H
#define DEMEZ_AMMODEF_H

#ifdef _WIN32
#pragma once
#endif

#include "ammodef.h"
#include "engine_defines.h"

class CDemezAmmoDef: public CAmmoDef
{
	DECLARE_CLASS( CDemezAmmoDef, CAmmoDef );

public:
	CDemezAmmoDef();
	~CDemezAmmoDef();
	
	int	MaxCarry(int nAmmoIndex, const CBaseCombatCharacter *owner);
};


#endif // DEMEZ_AMMODEF_H
