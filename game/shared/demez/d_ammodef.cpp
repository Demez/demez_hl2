#include "cbase.h"
#include "d_ammodef.h"
#include "d_gamemanager.h"


CDemezAmmoDef::CDemezAmmoDef(): CAmmoDef()
{
}

CDemezAmmoDef::~CDemezAmmoDef()
{
}


void CDemezAmmoDef::UpdateAmmoType( char const* name, int damageType, int tracerType, int plr_dmg, int npc_dmg, int carry, float physicsForceImpulse, int nFlags, int minSplashSize, int maxSplashSize )
{
	if ( UpdateAmmoType( name, damageType, tracerType, nFlags, minSplashSize, maxSplashSize ) == false )
		return;

	int index = Index( name );
	if ( index == -1 )
		index = m_nAmmoIndex;

	m_AmmoType[index].pPlrDmg = plr_dmg;
	m_AmmoType[index].pNPCDmg = npc_dmg;
	m_AmmoType[index].pMaxCarry = carry;

#if ENGINE_CSGO
	m_AmmoType[index].pPhysicsForceImpulse = physicsForceImpulse;
#else
	m_AmmoType[index].physicsForceImpulse = physicsForceImpulse;
#endif

	if ( index == m_nAmmoIndex )
		m_nAmmoIndex++;
}


void CDemezAmmoDef::UpdateAmmoType( char const* name, int damageType, int tracerType, char const* plr_cvar, char const* npc_cvar, char const* carry_cvar, float physicsForceImpulse, int nFlags, int minSplashSize, int maxSplashSize )
{
	if ( UpdateAmmoType( name, damageType, tracerType, nFlags, minSplashSize, maxSplashSize ) == false )
		return;

	int index = Index( name );
	if ( index == -1 )
		index = m_nAmmoIndex;

	if (plr_cvar)
	{
		m_AmmoType[index].pPlrDmgCVar	= cvar->FindVar(plr_cvar);
		if (!m_AmmoType[index].pPlrDmgCVar)
		{
			Msg("ERROR: Ammo (%s) found no CVar named (%s)\n",name,plr_cvar);
		}
		m_AmmoType[index].pPlrDmg = USE_CVAR;
	}
	if (npc_cvar)
	{
		m_AmmoType[index].pNPCDmgCVar	= cvar->FindVar(npc_cvar);
		if (!m_AmmoType[index].pNPCDmgCVar)
		{
			Msg("ERROR: Ammo (%s) found no CVar named (%s)\n",name,npc_cvar);
		}
		m_AmmoType[index].pNPCDmg = USE_CVAR;
	}
	if (carry_cvar)
	{
		m_AmmoType[index].pMaxCarryCVar= cvar->FindVar(carry_cvar);
		if (!m_AmmoType[index].pMaxCarryCVar)
		{
			Msg("ERROR: Ammo (%s) found no CVar named (%s)\n",name,carry_cvar);
		}
		m_AmmoType[index].pMaxCarry = USE_CVAR;
	}

#if ENGINE_CSGO
	m_AmmoType[index].pPhysicsForceImpulse = physicsForceImpulse;
#else
	m_AmmoType[index].physicsForceImpulse = physicsForceImpulse;
#endif

	if ( index == m_nAmmoIndex )
		m_nAmmoIndex++;
}


//-----------------------------------------------------------------------------
// Purpose: Create an Ammo type with the name, decal, and tracer.
// Does not increment m_nAmmoIndex because the functions below do so and 
//  are the only entry point.
//-----------------------------------------------------------------------------
bool CDemezAmmoDef::UpdateAmmoType(char const* name, int damageType, int tracerType, int nFlags, int minSplashSize, int maxSplashSize )
{
	if (m_nAmmoIndex == MAX_AMMO_TYPES)
		return false;

	int index = Index( name );
	if ( index == -1 )
	{
		// new ammo type
		index = m_nAmmoIndex;
		int len = strlen(name);
		m_AmmoType[index].pName = new char[len+1];
		Q_strncpy(m_AmmoType[index].pName, name,len+1);
	}

	m_AmmoType[index].nDamageType	= damageType;
	m_AmmoType[index].eTracerType	= tracerType;
	m_AmmoType[index].nMinSplashSize	= minSplashSize;
	m_AmmoType[index].nMaxSplashSize	= maxSplashSize;
	m_AmmoType[index].nFlags	= nFlags;

	return true;
}


void CDemezAmmoDef::ResetAmmoTypes()
{
	for ( int i = 1; i < m_nAmmoIndex; i++ )
	{
		delete[] m_AmmoType[ i ].pName;
	}

	m_nAmmoIndex = 1;
	memset( m_AmmoType, 0, sizeof( m_AmmoType ) );
}


// shared ammo definition
// JAY: Trying to make a more physical bullet response
#define BULLET_MASS_GRAINS_TO_LB(grains)	(0.002285*(grains)/16.0f)
#define BULLET_MASS_GRAINS_TO_KG(grains)	lbs2kg(BULLET_MASS_GRAINS_TO_LB(grains))

// exaggerate all of the forces, but use real numbers to keep them consistent
#define BULLET_IMPULSE_EXAGGERATION			3.5
// convert a velocity in ft/sec and a mass in grains to an impulse in kg in/s
#define BULLET_IMPULSE(grains, ftpersec)	((ftpersec)*12*BULLET_MASS_GRAINS_TO_KG(grains)*BULLET_IMPULSE_EXAGGERATION)


CDemezAmmoDef* g_ammoDef = NULL;


CAmmoDef *GetAmmoDef()
{
	static bool bInitted = false;

	if ( g_ammoDef == NULL )
	{
		bInitted = false;
		g_ammoDef = new CDemezAmmoDef;
	}

	if ( !bInitted )
	{
		bInitted = true;
		UpdateAmmoDef();
	}

	return g_ammoDef;
}


void UpdateAmmoDef()
{
	if ( g_ammoDef == NULL )
	{
		// create the ammo definitions instance
		GetAmmoDef();
	}

	g_ammoDef->ResetAmmoTypes();

	// hmm
	if ( DemezGameManager()->IsHL2() )
	{
		g_ammoDef->UpdateAmmoType("AR2",				DMG_BULLET,					TRACER_LINE_AND_WHIZ,	"sk_plr_dmg_ar2",			"sk_npc_dmg_ar2",			"sk_max_ar2",			BULLET_IMPULSE(200, 1225),	0 );
		g_ammoDef->UpdateAmmoType("AlyxGun",			DMG_BULLET,					TRACER_LINE,			"sk_plr_dmg_alyxgun",		"sk_npc_dmg_alyxgun",		"sk_max_alyxgun",		BULLET_IMPULSE(200, 1225),	0 );
		g_ammoDef->UpdateAmmoType("Pistol",			DMG_BULLET,					TRACER_LINE_AND_WHIZ,	"sk_plr_dmg_pistol",		"sk_npc_dmg_pistol",		"sk_max_pistol",		BULLET_IMPULSE(200, 1225),	0 );
		g_ammoDef->UpdateAmmoType("SMG1",				DMG_BULLET,					TRACER_LINE_AND_WHIZ,	"sk_plr_dmg_smg1",			"sk_npc_dmg_smg1",			"sk_max_smg1",			BULLET_IMPULSE(200, 1225),	0 );
		g_ammoDef->UpdateAmmoType("357",				DMG_BULLET,					TRACER_LINE_AND_WHIZ,	"sk_plr_dmg_357",			"sk_npc_dmg_357",			"sk_max_357",			BULLET_IMPULSE(800, 5000),	0 );
		g_ammoDef->UpdateAmmoType("XBowBolt",			DMG_BULLET,					TRACER_LINE,			"sk_plr_dmg_crossbow",		"sk_npc_dmg_crossbow",		"sk_max_crossbow",		BULLET_IMPULSE(800, 8000),	0 );
		g_ammoDef->UpdateAmmoType("FlareRound",		DMG_BURN,					TRACER_LINE,			"sk_plr_dmg_flare_round","sk_npc_dmg_flare_round",		"sk_max_flare_round",	BULLET_IMPULSE(1500, 600),	0 );
		g_ammoDef->UpdateAmmoType("Buckshot",			DMG_BULLET | DMG_BUCKSHOT,	TRACER_LINE,			"sk_plr_dmg_buckshot",		"sk_npc_dmg_buckshot",		"sk_max_buckshot",		BULLET_IMPULSE(400, 1200),	0 );
		g_ammoDef->UpdateAmmoType("RPG_Round",		DMG_BURN,					TRACER_NONE,			"sk_plr_dmg_rpg_round",		"sk_npc_dmg_rpg_round",		"sk_max_rpg_round",		0.0,						0 );
		g_ammoDef->UpdateAmmoType("SMG1_Grenade",		DMG_BURN,					TRACER_NONE,			"sk_plr_dmg_smg1_grenade",	"sk_npc_dmg_smg1_grenade",	"sk_max_smg1_grenade",	0.0,						0 );
		g_ammoDef->UpdateAmmoType("AR2_Grenade",		DMG_BURN,					TRACER_NONE,			"sk_plr_dmg_ar2_grenade",	"sk_npc_dmg_ar2_grenade",	"sk_max_ar2_grenade",	0.0,						0 );
		g_ammoDef->UpdateAmmoType("ML_Grenade",		DMG_BURN,					TRACER_NONE,			"sk_plr_dmg_ml_grenade",	"sk_npc_dmg_ml_grenade",	"sk_max_ml_grenade",	0.0,						0 );
		g_ammoDef->UpdateAmmoType("AR2AltFire",		DMG_DISSOLVE,				TRACER_NONE,			0,							0,							"sk_max_ar2_altfire",	0.0,						0 );
		g_ammoDef->UpdateAmmoType("SniperRound",		DMG_BULLET | DMG_SNIPER,	TRACER_NONE,			"sk_plr_dmg_sniper_round",	"sk_npc_dmg_sniper_round",	"sk_max_sniper_round",	BULLET_IMPULSE(650, 6000),	0 );
		g_ammoDef->UpdateAmmoType("SniperPenetratedRound",DMG_BULLET | DMG_SNIPER,TRACER_NONE,			"sk_dmg_sniper_penetrate_plr","sk_dmg_sniper_penetrate_npc","sk_max_sniper_round",BULLET_IMPULSE(150, 6000),0 );
		g_ammoDef->UpdateAmmoType("Slam",				DMG_BURN,					TRACER_NONE,			NULL,						NULL,						"sk_max_slam",			0.0,						0 );
		g_ammoDef->UpdateAmmoType("Tripwire",			DMG_BURN,					TRACER_NONE,			NULL,						NULL,						"sk_max_tripwire",		0.0,						0 );
		g_ammoDef->UpdateAmmoType("SmallRound",		DMG_BULLET,					TRACER_LINE,			"sk_plr_dmg_small_round",	"sk_npc_dmg_small_round",	"sk_max_small_round",	BULLET_IMPULSE(125, 1325),	0 );
		g_ammoDef->UpdateAmmoType("MediumRound",		DMG_BULLET,					TRACER_LINE,			"sk_plr_dmg_medium_round",	"sk_npc_dmg_medium_round",	"sk_max_medium_round",	BULLET_IMPULSE(200, 1225),	0 );
		g_ammoDef->UpdateAmmoType("LargeRound",		DMG_BULLET,					TRACER_LINE,			"sk_plr_dmg_large_round",	"sk_npc_dmg_large_round",	"sk_max_large_round",	BULLET_IMPULSE(250, 1180),	0 );
		g_ammoDef->UpdateAmmoType("Molotov",			DMG_BURN,					TRACER_NONE,			"sk_plr_dmg_molotov",		"sk_npc_dmg_molotov",		"sk_max_molotov", 		0.0,						0 );
		g_ammoDef->UpdateAmmoType("Grenade",			DMG_BURN,					TRACER_NONE,			"sk_plr_dmg_grenade",		"sk_npc_dmg_grenade",		"sk_max_grenade",		0.0,						0 );
		g_ammoDef->UpdateAmmoType("Brickbat",			DMG_CLUB,					TRACER_NONE,			"sk_plr_dmg_brickbat",		"sk_npc_dmg_brickbat",		"sk_max_brickbat",		0.0,						0 );
		g_ammoDef->UpdateAmmoType("Rock",				DMG_CLUB,					TRACER_NONE,			"sk_plr_dmg_brickbat",		"sk_npc_dmg_brickbat",		"sk_max_brickbat",		0.0,						0 );
		g_ammoDef->UpdateAmmoType("Thumper",			DMG_SONIC,					TRACER_NONE,			10, 10, 2, 0, 0 );
		g_ammoDef->UpdateAmmoType("Gravity",			DMG_CLUB,					TRACER_NONE,			0,	0, 8, 0, 0 );
		g_ammoDef->UpdateAmmoType("Extinguisher",		DMG_BURN,					TRACER_NONE,			0,	0, 100, 0, 0 );
		g_ammoDef->UpdateAmmoType("Battery",			DMG_CLUB,					TRACER_NONE,			NULL, NULL, NULL, 0, 0 );
		g_ammoDef->UpdateAmmoType("GaussEnergy",		DMG_SHOCK,					TRACER_NONE,			"sk_jeep_gauss_damage",		"sk_jeep_gauss_damage", "sk_max_gauss_round", BULLET_IMPULSE(650, 8000), 0 ); // hit like a 10kg weight at 400 in/s
		g_ammoDef->UpdateAmmoType("CombineCannon",	DMG_BULLET,					TRACER_LINE,			"sk_npc_dmg_gunship_to_plr", "sk_npc_dmg_gunship", NULL, 1.5 * 750 * 12, 0 ); // hit like a 1.5kg weight at 750 ft/s
		g_ammoDef->UpdateAmmoType("AirboatGun",		DMG_AIRBOAT,				TRACER_LINE,			"sk_plr_dmg_airboat",		"sk_npc_dmg_airboat",		NULL,					BULLET_IMPULSE(10, 600), 0 );
		g_ammoDef->UpdateAmmoType("HelicopterGun",	DMG_BULLET,					TRACER_LINE_AND_WHIZ,	"sk_npc_dmg_helicopter_to_plr", "sk_npc_dmg_helicopter",	"sk_max_smg1",	BULLET_IMPULSE(400, 1225), AMMO_FORCE_DROP_IF_CARRIED | AMMO_INTERPRET_PLRDAMAGE_AS_DAMAGE_TO_PLAYER );

		if ( DemezGameManager()->IsEpisodic() )
		{
			g_ammoDef->UpdateAmmoType("Hopwire",			DMG_BLAST,					TRACER_NONE,			"sk_plr_dmg_grenade",		"sk_npc_dmg_grenade",		"sk_max_hopwire",		0.0, 0);
			g_ammoDef->UpdateAmmoType("CombineHeavyCannon",	DMG_BULLET,				TRACER_LINE,			40,	40, NULL, 10 * 750 * 12, AMMO_FORCE_DROP_IF_CARRIED ); // hit like a 10 kg weight at 750 ft/s
			g_ammoDef->UpdateAmmoType("ammo_proto1",			DMG_BULLET,				TRACER_LINE,			0, 0, 10, 0, 0 );
		}

		g_ammoDef->UpdateAmmoType("StriderMinigunDirect",	DMG_BULLET,				TRACER_LINE,			2, 2, 15, 1.0 * 750 * 12, AMMO_FORCE_DROP_IF_CARRIED ); // hit like a 1.0kg weight at 750 ft/s

		//=====================================================================
		// STRIDER MINIGUN DAMAGE - Pull up a chair and I'll tell you a tale.
		//
		// When we shipped Half-Life 2 in 2004, we were unaware of a bug in
		// CAmmoDef::NPCDamage() which was returning the MaxCarry field of
		// an ammotype as the amount of damage that should be done to a NPC
		// by that type of ammo. Thankfully, the bug only affected Ammo Types 
		// that DO NOT use ConVars to specify their parameters. As you can see,
		// all of the important ammotypes use ConVars, so the effect of the bug
		// was limited. The Strider Minigun was affected, though.
		//
		// According to my perforce Archeology, we intended to ship the Strider
		// Minigun ammo type to do 15 points of damage per shot, and we did. 
		// To achieve this we, unaware of the bug, set the Strider Minigun ammo 
		// type to have a maxcarry of 15, since our observation was that the 
		// number that was there before (8) was indeed the amount of damage being
		// done to NPC's at the time. So we changed the field that was incorrectly
		// being used as the NPC Damage field.
		//
		// The bug was fixed during Episode 1's development. The result of the 
		// bug fix was that the Strider was reduced to doing 5 points of damage
		// to NPC's, since 5 is the value that was being assigned as NPC damage
		// even though the code was returning 15 up to that point.
		//
		// Now as we go to ship Orange Box, we discover that the Striders in 
		// Half-Life 2 are hugely ineffective against citizens, causing big
		// problems in maps 12 and 13. 
		//
		// In order to restore balance to HL2 without upsetting the delicate 
		// balance of ep2_outland_12, I have chosen to build Episodic binaries
		// with 5 as the Strider->NPC damage, since that's the value that has
		// been in place for all of Episode 2's development. Half-Life 2 will
		// build with 15 as the Strider->NPC damage, which is how HL2 shipped
		// originally, only this time the 15 is located in the correct field
		// now that the AmmoDef code is behaving correctly.
		//
		//=====================================================================

		if ( DemezGameManager()->IsEpisodic() )
			g_ammoDef->UpdateAmmoType("StriderMinigun",	DMG_BULLET,					TRACER_LINE,			5, 5, 15, 1.0 * 750 * 12, AMMO_FORCE_DROP_IF_CARRIED ); // hit like a 1.0kg weight at 750 ft/s
		else
			g_ammoDef->UpdateAmmoType("StriderMinigun",	DMG_BULLET,					TRACER_LINE,			5, 15,15, 1.0 * 750 * 12, AMMO_FORCE_DROP_IF_CARRIED ); // hit like a 1.0kg weight at 750 ft/s
	}
}

