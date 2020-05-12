
#ifndef AI_DEMEZ_NPC_H
#define AI_DEMEZ_NPC_H

#ifdef _WIN32
#pragma once
#endif

#include "c_demez_combat_character.h"

// full smile
#define private public
// #define C_BaseCombatCharacter C_DemezCombatCharacter
#include "c_ai_basenpc.h"
#undef private
// #undef C_BaseCombatCharacter

#include "engine_defines.h"

// class C_AI_DemezNPC: public C_DemezCombatCharacter, public C_AI_BaseNPC
class C_AI_DemezNPC: public C_AI_BaseNPC
{
	DECLARE_CLASS( C_AI_DemezNPC, C_AI_BaseNPC );

public:
	DECLARE_CLIENTCLASS();

	C_AI_DemezNPC();
	~C_AI_DemezNPC();

	void ClientThink();

	int DrawModel( int flags );
	int DrawModel( int flags, const RenderableInstance_t &instance );

private:
	C_AI_DemezNPC( const C_AI_DemezNPC & ); // not defined, not accessible
};


#endif // AI_DEMEZ_NPC_H
