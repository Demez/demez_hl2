
#ifndef AI_DEMEZ_ACTOR_H
#define AI_DEMEZ_ACTOR_H

#ifdef _WIN32
#pragma once
#endif

#include "engine_defines.h"
#include "ai_baseactor.h"

class CAI_DemezActor : public CAI_BaseActor
{
	DECLARE_CLASS( CAI_DemezActor, CAI_BaseActor );

public:
	CAI_DemezActor();
	~CAI_DemezActor();
};

#endif // AI_DEMEZ_ACTOR_H
