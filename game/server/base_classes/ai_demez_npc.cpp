//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose:		Base NPC character with AI
//
//=============================================================================//

#include "cbase.h"
#include "ai_demez_npc.h"

// #define private public
#include "ai_basenpc.h"
// #undef private

CAI_DemezNPC::CAI_DemezNPC()
{
}

CAI_DemezNPC::~CAI_DemezNPC()
{
}


BEGIN_DATADESC( CAI_DemezNPC )
END_DATADESC()

IMPLEMENT_SERVERCLASS_ST( CAI_DemezNPC, DT_AI_DemezNPC )
END_SEND_TABLE()

/*#if ENGINE_NEW
CAI_ClassScheduleIdSpace	CAI_DemezNPC::gm_ClassScheduleIdSpace( true );
CAI_GlobalScheduleNamespace CAI_DemezNPC::gm_SchedulingSymbols;
#endif*/