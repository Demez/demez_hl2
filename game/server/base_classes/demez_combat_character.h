#ifndef DEMEZ_COMBAT_CHARACTER_H
#define DEMEZ_COMBAT_CHARACTER_H

#ifdef _WIN32
#pragma once
#endif

#include "engine_defines.h"
#include "basecombatcharacter.h"


class CDemezCombatCharacter : public CBaseCombatCharacter
{
	DECLARE_CLASS( CDemezCombatCharacter, CBaseCombatCharacter );
	
public:
	CDemezCombatCharacter();
	~CDemezCombatCharacter();
	
	// DECLARE_DATADESC();
	// DECLARE_SERVERCLASS();
};

#endif // DEMEZ_COMBAT_CHARACTER_H
