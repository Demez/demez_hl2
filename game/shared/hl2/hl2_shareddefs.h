//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef HL2_SHAREDDEFS_H
#define HL2_SHAREDDEFS_H

#ifdef _WIN32
#pragma once
#endif

#include "const.h"
#include "engine_defines.h"


#if ENGINE_CSGO && defined(CLIENT_DLL)
	#define LINK_ENTITY_TO_CLASS_DUMB			LINK_ENTITY_TO_CLASS_CLIENTONLY
#else
	#define LINK_ENTITY_TO_CLASS_DUMB			LINK_ENTITY_TO_CLASS
#endif

#if ENGINE_ASW
	#define LINK_ENTITY_TO_CLASS_ALIASED		LINK_ENTITY_TO_CLASS
	#define LINK_ENTITY_TO_CLASS_CLIENTONLY		LINK_ENTITY_TO_CLASS
#endif


//--------------------------------------------------------------------------
// Collision groups
//--------------------------------------------------------------------------

enum
{
	HL2COLLISION_GROUP_PLASMANODE = LAST_SHARED_COLLISION_GROUP,
	HL2COLLISION_GROUP_SPIT,
	HL2COLLISION_GROUP_HOMING_MISSILE,
	HL2COLLISION_GROUP_COMBINE_BALL,

	HL2COLLISION_GROUP_FIRST_NPC,
	HL2COLLISION_GROUP_HOUNDEYE,
	HL2COLLISION_GROUP_CROW,
	HL2COLLISION_GROUP_HEADCRAB,
	HL2COLLISION_GROUP_STRIDER,
	HL2COLLISION_GROUP_GUNSHIP,
	HL2COLLISION_GROUP_ANTLION,
	HL2COLLISION_GROUP_LAST_NPC,
	HL2COLLISION_GROUP_COMBINE_BALL_NPC,
};


//--------------
// HL2 SPECIFIC
//--------------
#define DMG_SNIPER			(DMG_LASTGENERICFLAG<<1)	// This is sniper damage
#define DMG_MISSILEDEFENSE	(DMG_LASTGENERICFLAG<<2)	// The only kind of damage missiles take. (special missile defense)



#endif // HL2_SHAREDDEFS_H
