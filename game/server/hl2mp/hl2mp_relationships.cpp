//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "hl2mp_gamerules.h"
/*#include "viewport_panel_names.h"
#include "gameeventdefs.h"
#include <KeyValues.h>
#include "ammodef.h"
#include "hl2_shareddefs.h"*/

/*
#ifdef CLIENT_DLL
	#include "c_hl2mp_player.h"
#else
	#include "globalstate.h"
	#include "eventqueue.h"
	#include "player.h"
	#include "gamerules.h"
	#include "game.h"
	#include "items.h"
	#include "entitylist.h"
	#include "mapentities.h"
	#include "in_buttons.h"
	#include <ctype.h>
	#include "voice_gamemgr.h"
	#include "iscorer.h"
	#include "hl2mp_player.h"
	#include "basehlcombatweapon.h"
	#include "team.h"
	#include "voice_gamemgr.h"
	#include "hl2mp_gameinterface.h"
	#include "hl2mp_cvars.h"

	#include "demez_items.h"
#endif
*/


#ifdef GAME_DLL

// keep this at the end of the file because it's ugly
// this is an awful function, should just be in a keyvalues file
void CHL2MPRules::InitDefaultAIRelationships(void)
{
	int i, j;

	//  Allocate memory for default relationships
	CBaseCombatCharacter::AllocateDefaultRelationships();

	// --------------------------------------------------------------
	// First initialize table so we can report missing relationships
	// --------------------------------------------------------------
	for (i = 0; i < LAST_SHARED_ENTITY_CLASS; i++)
	{
		for (j = 0; j < LAST_SHARED_ENTITY_CLASS; j++)
		{
			// By default all relationships are neutral of priority zero
			CBaseCombatCharacter::SetDefaultRelationship((Class_T) i, (Class_T) j, D_NU, 0);
		}
	}

	// ------------------------------------------------------------
	//	> CLASS_ANTLION
	// ------------------------------------------------------------
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_PLAYER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_BARNACLE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_BULLSQUID, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_CITIZEN_PASSIVE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_CITIZEN_REBEL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_COMBINE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_COMBINE_HUNTER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_CONSCRIPT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_HEADCRAB, D_HT, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_HOUNDEYE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_MANHACK, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_METROPOLICE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_MILITARY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_SCANNER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_STALKER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_VORTIGAUNT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_ZOMBIE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_PROTOSNIPER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_ANTLION, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_PLAYER_ALLY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_PLAYER_ALLY_VITAL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ANTLION, CLASS_HACKED_ROLLERMINE, D_HT, 0);

	// ------------------------------------------------------------
	//	> CLASS_BARNACLE
	//
	//  In this case, the relationship D_HT indicates which characters
	//  the barnacle will try to eat.
	// ------------------------------------------------------------
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_PLAYER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_BARNACLE, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_BULLSQUID, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_CITIZEN_PASSIVE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_CITIZEN_REBEL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_COMBINE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_COMBINE_HUNTER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_CONSCRIPT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_HEADCRAB, D_HT, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_HOUNDEYE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_MANHACK, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_METROPOLICE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_MILITARY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_SCANNER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_STALKER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_VORTIGAUNT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_ZOMBIE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_EARTH_FAUNA, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_PLAYER_ALLY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_PLAYER_ALLY_VITAL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BARNACLE, CLASS_HACKED_ROLLERMINE, D_HT, 0);

	// ------------------------------------------------------------
	//	> CLASS_BULLSEYE
	// ------------------------------------------------------------
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_PLAYER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_ANTLION, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_BARNACLE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_BULLSQUID, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_CITIZEN_PASSIVE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_CITIZEN_REBEL, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_COMBINE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_COMBINE_HUNTER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_CONSCRIPT, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_HEADCRAB, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_HOUNDEYE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_MANHACK, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_METROPOLICE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_MILITARY, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_SCANNER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_STALKER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_VORTIGAUNT, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_ZOMBIE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_PLAYER_ALLY, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_PLAYER_ALLY_VITAL, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSEYE, CLASS_HACKED_ROLLERMINE, D_NU, 0);

	// ------------------------------------------------------------
	//	> CLASS_BULLSQUID
	// ------------------------------------------------------------
	/*
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_PLAYER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_BARNACLE, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_BULLSEYE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_BULLSQUID, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_CITIZEN_PASSIVE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_CITIZEN_REBEL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_COMBINE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_COMBINE_HUNTER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_CONSCRIPT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_HEADCRAB, D_HT, 1);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_HOUNDEYE, D_HT, 1);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_MANHACK, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_METROPOLICE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_MILITARY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_SCANNER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_STALKER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_VORTIGAUNT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_ZOMBIE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_PLAYER_ALLY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_PLAYER_ALLY_VITAL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_BULLSQUID, CLASS_HACKED_ROLLERMINE, D_HT, 0);
	*/

	// ------------------------------------------------------------
	//	> CLASS_CITIZEN_PASSIVE
	// ------------------------------------------------------------
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_PLAYER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_BARNACLE, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_BULLSQUID, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_CITIZEN_PASSIVE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_CITIZEN_REBEL, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_COMBINE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_COMBINE_HUNTER, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_CONSCRIPT, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_HEADCRAB, D_FR, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_HOUNDEYE, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_MANHACK, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_METROPOLICE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_MILITARY, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_MISSILE, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_SCANNER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_STALKER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_VORTIGAUNT, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_ZOMBIE, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_PLAYER_ALLY, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_PLAYER_ALLY_VITAL, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_PASSIVE, CLASS_HACKED_ROLLERMINE, D_NU, 0);

	// ------------------------------------------------------------
	//	> CLASS_CITIZEN_REBEL
	// ------------------------------------------------------------
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_PLAYER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_BARNACLE, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_BULLSQUID, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_CITIZEN_PASSIVE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_CITIZEN_REBEL, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_COMBINE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_COMBINE_HUNTER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_CONSCRIPT, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_HEADCRAB, D_HT, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_HOUNDEYE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_MANHACK, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_METROPOLICE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_MILITARY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_MISSILE, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_SCANNER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_STALKER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_VORTIGAUNT, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_ZOMBIE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_PLAYER_ALLY, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_PLAYER_ALLY_VITAL, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CITIZEN_REBEL, CLASS_HACKED_ROLLERMINE, D_NU, 0);

	// ------------------------------------------------------------
	//	> CLASS_COMBINE
	// ------------------------------------------------------------
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_PLAYER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_BARNACLE, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_BULLSQUID, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_CITIZEN_PASSIVE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_CITIZEN_REBEL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_COMBINE, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_COMBINE_GUNSHIP, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_COMBINE_HUNTER, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_CONSCRIPT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_HEADCRAB, D_HT, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_HOUNDEYE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_MANHACK, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_METROPOLICE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_MILITARY, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_SCANNER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_STALKER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_VORTIGAUNT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_ZOMBIE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_PLAYER_ALLY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_PLAYER_ALLY_VITAL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE, CLASS_HACKED_ROLLERMINE, D_HT, 0);

	// ------------------------------------------------------------
	//	> CLASS_COMBINE_GUNSHIP
	// ------------------------------------------------------------
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_PLAYER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_BARNACLE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_BULLSQUID, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_CITIZEN_PASSIVE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_CITIZEN_REBEL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_COMBINE, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_COMBINE_GUNSHIP, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_COMBINE_HUNTER, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_CONSCRIPT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_HEADCRAB, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_HOUNDEYE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_MANHACK, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_METROPOLICE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_MILITARY, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_MISSILE, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_SCANNER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_STALKER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_VORTIGAUNT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_ZOMBIE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_PLAYER_ALLY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_PLAYER_ALLY_VITAL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_GUNSHIP, CLASS_HACKED_ROLLERMINE, D_HT, 0);

	// ------------------------------------------------------------
	//	> CLASS_COMBINE_HUNTER
	// ------------------------------------------------------------
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_PLAYER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_BARNACLE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_BULLSQUID, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_CITIZEN_PASSIVE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_CITIZEN_REBEL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_COMBINE, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_COMBINE_GUNSHIP, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_COMBINE_HUNTER, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_CONSCRIPT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_HEADCRAB, D_HT, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_HOUNDEYE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_MANHACK, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_METROPOLICE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_MILITARY, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_SCANNER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_STALKER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_VORTIGAUNT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_ZOMBIE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_PLAYER_ALLY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_PLAYER_ALLY_VITAL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_COMBINE_HUNTER, CLASS_HACKED_ROLLERMINE, D_HT, 0);

	// ------------------------------------------------------------
	//	> CLASS_CONSCRIPT
	// ------------------------------------------------------------
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_PLAYER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_BARNACLE, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_BULLSQUID, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_CITIZEN_PASSIVE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_CITIZEN_REBEL, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_COMBINE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_COMBINE_HUNTER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_CONSCRIPT, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_HEADCRAB, D_HT, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_HOUNDEYE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_MANHACK, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_METROPOLICE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_MILITARY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_SCANNER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_STALKER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_VORTIGAUNT, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_ZOMBIE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_PLAYER_ALLY, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_PLAYER_ALLY_VITAL, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_CONSCRIPT, CLASS_HACKED_ROLLERMINE, D_NU, 0);

	// ------------------------------------------------------------
	//	> CLASS_FLARE
	// ------------------------------------------------------------
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_PLAYER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_ANTLION, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_BARNACLE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_BULLSQUID, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_CITIZEN_PASSIVE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_CITIZEN_REBEL, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_COMBINE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_COMBINE_HUNTER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_CONSCRIPT, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_HEADCRAB, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_HOUNDEYE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_MANHACK, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_METROPOLICE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_MILITARY, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_SCANNER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_STALKER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_VORTIGAUNT, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_ZOMBIE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_PLAYER_ALLY, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_PLAYER_ALLY_VITAL, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_FLARE, CLASS_HACKED_ROLLERMINE, D_NU, 0);

	// ------------------------------------------------------------
	//	> CLASS_HEADCRAB
	// ------------------------------------------------------------
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_PLAYER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_BARNACLE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_BULLSQUID, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_CITIZEN_PASSIVE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_CITIZEN_REBEL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_COMBINE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_COMBINE_HUNTER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_CONSCRIPT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_HEADCRAB, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_HOUNDEYE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_MANHACK, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_METROPOLICE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_MILITARY, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_SCANNER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_STALKER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_VORTIGAUNT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_ZOMBIE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_PLAYER_ALLY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_PLAYER_ALLY_VITAL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HEADCRAB, CLASS_HACKED_ROLLERMINE, D_FR, 0);

	// ------------------------------------------------------------
	//	> CLASS_HOUNDEYE
	// ------------------------------------------------------------
	/*
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_PLAYER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_BARNACLE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_BULLSEYE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_BULLSQUID, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_CITIZEN_PASSIVE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_CITIZEN_REBEL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_COMBINE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_COMBINE_HUNTER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_CONSCRIPT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_HEADCRAB, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_HOUNDEYE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_MANHACK, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_METROPOLICE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_MILITARY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_SCANNER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_STALKER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_VORTIGAUNT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_ZOMBIE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_PLAYER_ALLY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_PLAYER_ALLY_VITAL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HOUNDEYE, CLASS_HACKED_ROLLERMINE, D_HT, 0);
	*/

	// ------------------------------------------------------------
	//	> CLASS_MANHACK
	// ------------------------------------------------------------
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_PLAYER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_BARNACLE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_BULLSQUID, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_CITIZEN_PASSIVE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_CITIZEN_REBEL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_COMBINE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_COMBINE_HUNTER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_CONSCRIPT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_HEADCRAB, D_HT, -1);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_HOUNDEYE, D_HT, -1);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_MANHACK, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_METROPOLICE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_MILITARY, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_SCANNER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_STALKER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_VORTIGAUNT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_ZOMBIE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_PLAYER_ALLY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_PLAYER_ALLY_VITAL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MANHACK, CLASS_HACKED_ROLLERMINE, D_HT, 0);

	// ------------------------------------------------------------
	//	> CLASS_METROPOLICE
	// ------------------------------------------------------------
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_PLAYER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_BARNACLE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_BULLSQUID, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_CITIZEN_PASSIVE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_CITIZEN_REBEL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_COMBINE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_COMBINE_HUNTER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_CONSCRIPT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_HEADCRAB, D_HT, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_HOUNDEYE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_MANHACK, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_METROPOLICE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_MILITARY, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_SCANNER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_STALKER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_VORTIGAUNT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_ZOMBIE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_PLAYER_ALLY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_PLAYER_ALLY_VITAL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_METROPOLICE, CLASS_HACKED_ROLLERMINE, D_HT, 0);

	// ------------------------------------------------------------
	//	> CLASS_MILITARY
	// ------------------------------------------------------------
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_PLAYER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_BARNACLE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_BULLSQUID, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_CITIZEN_PASSIVE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_CITIZEN_REBEL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_COMBINE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_COMBINE_HUNTER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_CONSCRIPT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_HEADCRAB, D_HT, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_HOUNDEYE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_MANHACK, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_METROPOLICE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_MILITARY, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_SCANNER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_STALKER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_VORTIGAUNT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_ZOMBIE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_PLAYER_ALLY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_PLAYER_ALLY_VITAL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MILITARY, CLASS_HACKED_ROLLERMINE, D_HT, 0);

	// ------------------------------------------------------------
	//	> CLASS_MISSILE
	// ------------------------------------------------------------
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_PLAYER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_BARNACLE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_BULLSQUID, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_CITIZEN_PASSIVE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_CITIZEN_REBEL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_COMBINE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_COMBINE_HUNTER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_CONSCRIPT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_HEADCRAB, D_HT, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_HOUNDEYE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_MANHACK, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_METROPOLICE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_MILITARY, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_SCANNER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_STALKER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_VORTIGAUNT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_ZOMBIE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_PLAYER_ALLY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_PLAYER_ALLY_VITAL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_MISSILE, CLASS_HACKED_ROLLERMINE, D_HT, 0);

	// ------------------------------------------------------------
	//	> CLASS_NONE
	// ------------------------------------------------------------
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_PLAYER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_ANTLION, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_BARNACLE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_BULLSQUID, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_CITIZEN_PASSIVE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_CITIZEN_REBEL, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_COMBINE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_COMBINE_HUNTER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_CONSCRIPT, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_HEADCRAB, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_HOUNDEYE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_MANHACK, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_METROPOLICE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_MILITARY, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_SCANNER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_STALKER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_VORTIGAUNT, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_ZOMBIE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_PLAYER_ALLY, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_PLAYER_ALLY_VITAL, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_NONE, CLASS_HACKED_ROLLERMINE, D_NU, 0);

	// ------------------------------------------------------------
	//	> CLASS_PLAYER
	// ------------------------------------------------------------
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_PLAYER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_BARNACLE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_BULLSEYE, D_HT, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_BULLSQUID, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_CITIZEN_PASSIVE, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_CITIZEN_REBEL, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_COMBINE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_COMBINE_GUNSHIP, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_COMBINE_HUNTER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_CONSCRIPT, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_HEADCRAB, D_HT, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_HOUNDEYE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_MANHACK, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_METROPOLICE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_MILITARY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_SCANNER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_STALKER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_VORTIGAUNT, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_ZOMBIE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_PROTOSNIPER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_PLAYER_ALLY, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_PLAYER_ALLY_VITAL, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER, CLASS_HACKED_ROLLERMINE, D_LI, 0);

	// ------------------------------------------------------------
	//	> CLASS_PLAYER_ALLY
	// ------------------------------------------------------------
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_PLAYER, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_BARNACLE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_BULLSQUID, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_CITIZEN_PASSIVE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_CITIZEN_REBEL, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_COMBINE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_COMBINE_HUNTER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_CONSCRIPT, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_HEADCRAB, D_FR, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_HOUNDEYE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_MANHACK, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_METROPOLICE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_MILITARY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_SCANNER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_STALKER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_VORTIGAUNT, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_ZOMBIE, D_FR, 1);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_PROTOSNIPER, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_PLAYER_ALLY, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_PLAYER_ALLY_VITAL, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY, CLASS_HACKED_ROLLERMINE, D_LI, 0);

	// ------------------------------------------------------------
	//	> CLASS_PLAYER_ALLY_VITAL
	// ------------------------------------------------------------
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_PLAYER, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_BARNACLE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_BULLSQUID, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_CITIZEN_PASSIVE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_CITIZEN_REBEL, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_COMBINE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_COMBINE_HUNTER, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_CONSCRIPT, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_HEADCRAB, D_HT, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_HOUNDEYE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_MANHACK, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_METROPOLICE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_MILITARY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_SCANNER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_STALKER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_VORTIGAUNT, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_ZOMBIE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_PROTOSNIPER, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_PLAYER_ALLY, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_PLAYER_ALLY_VITAL, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PLAYER_ALLY_VITAL, CLASS_HACKED_ROLLERMINE, D_LI, 0);

	// ------------------------------------------------------------
	//	> CLASS_SCANNER
	// ------------------------------------------------------------	
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_PLAYER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_BARNACLE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_BULLSQUID, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_CITIZEN_PASSIVE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_CITIZEN_REBEL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_COMBINE, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_COMBINE_GUNSHIP, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_COMBINE_HUNTER, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_CONSCRIPT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_HEADCRAB, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_HOUNDEYE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_MANHACK, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_METROPOLICE, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_MILITARY, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_SCANNER, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_STALKER, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_VORTIGAUNT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_ZOMBIE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_PROTOSNIPER, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_PLAYER_ALLY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_PLAYER_ALLY_VITAL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_SCANNER, CLASS_HACKED_ROLLERMINE, D_HT, 0);

	// ------------------------------------------------------------
	//	> CLASS_STALKER
	// ------------------------------------------------------------	
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_PLAYER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_BARNACLE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_BULLSQUID, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_CITIZEN_PASSIVE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_CITIZEN_REBEL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_COMBINE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_COMBINE_HUNTER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_CONSCRIPT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_HEADCRAB, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_HOUNDEYE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_MANHACK, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_METROPOLICE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_MILITARY, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_SCANNER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_STALKER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_VORTIGAUNT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_ZOMBIE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_PLAYER_ALLY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_PLAYER_ALLY_VITAL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_STALKER, CLASS_HACKED_ROLLERMINE, D_HT, 0);

	// ------------------------------------------------------------
	//	> CLASS_VORTIGAUNT
	// ------------------------------------------------------------	
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_PLAYER, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_BARNACLE, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_BULLSQUID, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_CITIZEN_PASSIVE, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_CITIZEN_REBEL, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_COMBINE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_COMBINE_HUNTER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_CONSCRIPT, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_HEADCRAB, D_HT, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_HOUNDEYE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_MANHACK, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_METROPOLICE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_MILITARY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_SCANNER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_STALKER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_VORTIGAUNT, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_ZOMBIE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_PLAYER_ALLY, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_PLAYER_ALLY_VITAL, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_VORTIGAUNT, CLASS_HACKED_ROLLERMINE, D_LI, 0);

	// ------------------------------------------------------------
	//	> CLASS_ZOMBIE
	// ------------------------------------------------------------	
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_PLAYER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_BARNACLE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_BULLSQUID, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_CITIZEN_PASSIVE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_CITIZEN_REBEL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_COMBINE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_COMBINE_HUNTER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_CONSCRIPT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_HEADCRAB, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_HOUNDEYE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_MANHACK, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_METROPOLICE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_MILITARY, D_FR, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_SCANNER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_STALKER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_VORTIGAUNT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_ZOMBIE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_PLAYER_ALLY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_PLAYER_ALLY_VITAL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_ZOMBIE, CLASS_HACKED_ROLLERMINE, D_HT, 0);

	// ------------------------------------------------------------
	//	> CLASS_PROTOSNIPER
	// ------------------------------------------------------------	
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_PLAYER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_BARNACLE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_BULLSQUID, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_CITIZEN_PASSIVE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_CITIZEN_REBEL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_COMBINE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_COMBINE_HUNTER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_CONSCRIPT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_HEADCRAB, D_HT, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_HOUNDEYE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_MANHACK, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_METROPOLICE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_MILITARY, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_MISSILE, D_NU, 5);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_SCANNER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_STALKER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_VORTIGAUNT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_ZOMBIE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_PLAYER_ALLY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_PLAYER_ALLY_VITAL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_PROTOSNIPER, CLASS_HACKED_ROLLERMINE, D_HT, 0);

	// ------------------------------------------------------------
	//	> CLASS_EARTH_FAUNA
	//
	// Hates pretty much everything equally except other earth fauna.
	// This will make the critter choose the nearest thing as its enemy.
	// ------------------------------------------------------------	
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_PLAYER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_BARNACLE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_BULLSQUID, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_CITIZEN_PASSIVE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_CITIZEN_REBEL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_COMBINE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_COMBINE_GUNSHIP, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_COMBINE_HUNTER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_CONSCRIPT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_FLARE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_HEADCRAB, D_HT, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_HOUNDEYE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_MANHACK, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_METROPOLICE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_MILITARY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_MISSILE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_SCANNER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_STALKER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_VORTIGAUNT, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_ZOMBIE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_PROTOSNIPER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_EARTH_FAUNA, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_PLAYER_ALLY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_PLAYER_ALLY_VITAL, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_EARTH_FAUNA, CLASS_HACKED_ROLLERMINE, D_NU, 0);

	// ------------------------------------------------------------
	//	> CLASS_HACKED_ROLLERMINE
	// ------------------------------------------------------------
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_NONE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_PLAYER, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_ANTLION, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_BARNACLE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_BULLSEYE, D_NU, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_BULLSQUID, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_CITIZEN_PASSIVE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_CITIZEN_REBEL, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_COMBINE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_COMBINE_GUNSHIP, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_COMBINE_HUNTER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_CONSCRIPT, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_FLARE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_HEADCRAB, D_HT, 0);
	// CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_HOUNDEYE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_MANHACK, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_METROPOLICE, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_MILITARY, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_MISSILE, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_SCANNER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_STALKER, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_VORTIGAUNT, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_ZOMBIE, D_HT, 1);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_PROTOSNIPER, D_NU, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_EARTH_FAUNA, D_HT, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_PLAYER_ALLY, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_PLAYER_ALLY_VITAL, D_LI, 0);
	CBaseCombatCharacter::SetDefaultRelationship(CLASS_HACKED_ROLLERMINE, CLASS_HACKED_ROLLERMINE, D_LI, 0);
}

#endif