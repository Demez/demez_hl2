
#ifndef AI_DEMEZ_NPC_H
#define AI_DEMEZ_NPC_H

#ifdef _WIN32
#pragma once
#endif

#include "demez_combat_character.h"

// full smile
// #define private public
#include "ai_basenpc.h"
// #undef private

#include "engine_defines.h"

class CAI_DemezNPC: public CAI_BaseNPC
{
	DECLARE_CLASS( CAI_DemezNPC, CAI_BaseNPC );

public:
	CAI_DemezNPC();
	~CAI_DemezNPC();
	
	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();
};


#endif // AI_DEMEZ_NPC_H
