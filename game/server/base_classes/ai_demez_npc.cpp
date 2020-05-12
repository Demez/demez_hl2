//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose:		Base NPC character with AI
//
//=============================================================================//

#include "cbase.h"
#include "ai_demez_npc.h"

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

