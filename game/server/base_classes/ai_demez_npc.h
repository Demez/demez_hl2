
#ifndef AI_DEMEZ_NPC_H
#define AI_DEMEZ_NPC_H

#ifdef _WIN32
#pragma once
#endif

#include "ai_basenpc.h"
#include "demez_combat_character.h"
#include "engine_defines.h"

class CAI_DemezNPC: public CDemezCombatCharacter,
	public CAI_BaseNPC
{
	DECLARE_CLASS( CAI_DemezNPC, CDemezCombatCharacter );

public:
	CAI_DemezNPC();
	~CAI_DemezNPC();
	
	// DECLARE_DATADESC();
	// DECLARE_SERVERCLASS();
};


#endif // AI_DEMEZ_NPC_H
