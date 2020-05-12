#ifndef DEMEZ_COMBAT_CHARACTER_H
#define DEMEZ_COMBAT_CHARACTER_H

#ifdef _WIN32
#pragma once
#endif

#include "engine_defines.h"
#include "c_basecombatcharacter.h"


class C_DemezCombatCharacter : public C_BaseCombatCharacter
{
	DECLARE_CLASS( C_DemezCombatCharacter, C_BaseCombatCharacter );
	
public:
	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();

	C_DemezCombatCharacter();
	~C_DemezCombatCharacter();

	int DrawModel( int flags );
	int DrawModel( int flags, const RenderableInstance_t &instance );
};

#endif // DEMEZ_COMBAT_CHARACTER_H
