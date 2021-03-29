//********************************************************************************************
//MAPSPAWN.nut is called on newgame or transitions
//********************************************************************************************
printl( "==== calling pablo.nut on map " + GetMapName() )


mapsNoSuit <- [
	"d1_trainstation_01",
	"d1_trainstation_02",
	"d1_trainstation_03",
	"d1_trainstation_04",
	"d1_trainstation_05",
]


weaponDefaultAmmo <- {
	weapon_crowbar =	[],
	weapon_bugbait =	[],
	weapon_stunstick =	[],
	weapon_physcannon =	[],
	weapon_crossbow =	[],
	
	weapon_pistol =		[255,	"Pistol"],
	weapon_shotgun =	[255,	"Buckshot"],
	weapon_357 =		[32,	"357"],
	weapon_rpg =		[3,		"rpg_round"],
	weapon_frag =		[1,		"grenade"],
	
	weapon_ar2 =		[255,	"AR2",		5, "AR2AltFire"],
	weapon_smg1 =		[255,	"SMG1",		1, "smg1_grenade"],
}


allWeaponsBugBait <- [
	"weapon_physcannon",
	"weapon_crowbar",
	"weapon_pistol",
	"weapon_smg1",
	"weapon_ar2",
	"weapon_shotgun",
	"weapon_357",
	"weapon_frag",
	"weapon_rpg",
	"weapon_crossbow",
	"weapon_bugbait",
]

allWeapons <- [
	"weapon_physcannon",
	"weapon_crowbar",
	"weapon_pistol",
	"weapon_smg1",
	"weapon_ar2",
	"weapon_shotgun",
	"weapon_357",
	"weapon_frag",
	"weapon_rpg",
	"weapon_crossbow",
]


mapSpawnWeapons <- {
	d1_trainstation_01 = 	[],
	d1_trainstation_02 = 	[],
	d1_trainstation_03 = 	[],
	d1_trainstation_04 = 	[],
	d1_trainstation_05 = 	[],
	
	d1_trainstation_06 = 	["weapon_crowbar"],
	
	d1_canals_01 = 			["weapon_crowbar", "weapon_pistol"],
	d1_canals_01a = 		["weapon_crowbar", "weapon_pistol"],
	d1_canals_02 = 			["weapon_crowbar", "weapon_pistol"],
	d1_canals_03 = 			["weapon_crowbar", "weapon_pistol"], // smg1 acquired
	
	d1_canals_05 = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1"],
	d1_canals_06 = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1"],
	d1_canals_07 = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1"],  // frag acquired
	
	d1_canals_08 = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1", "weapon_frag", "weapon_357"],  // 357 acquired
	d1_canals_09 = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1", "weapon_frag", "weapon_357"],
	d1_canals_10 = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1", "weapon_frag", "weapon_357"],
	d1_canals_11 = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1", "weapon_frag", "weapon_357"],
	d1_canals_12 = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1", "weapon_frag", "weapon_357"],
	d1_canals_13 = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1", "weapon_frag", "weapon_357"],
	
	d1_eli_01 = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1", "weapon_frag", "weapon_357"],
	d1_eli_02 = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1", "weapon_frag", "weapon_357", "weapon_physcannon"],
	
	d1_town_01 = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1", "weapon_frag", "weapon_357", "weapon_physcannon"],
	d1_town_01a = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1", "weapon_frag", "weapon_357", "weapon_physcannon"],
	d1_town_02 = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1", "weapon_frag", "weapon_357", "weapon_physcannon", "weapon_shotgun"],  // shotgun acquired
	d1_town_03 = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1", "weapon_frag", "weapon_357", "weapon_physcannon", "weapon_shotgun"], 
	d1_town_02a = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1", "weapon_frag", "weapon_357", "weapon_physcannon", "weapon_shotgun"], 
	d1_town_04 = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1", "weapon_frag", "weapon_357", "weapon_physcannon", "weapon_shotgun"], 
	d1_town_05 = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1", "weapon_frag", "weapon_357", "weapon_physcannon", "weapon_shotgun"], 
	
	d2_coast_01 = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1", "weapon_frag", "weapon_357", "weapon_physcannon", "weapon_shotgun", "weapon_ar2"], 
	d2_coast_03 = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1", "weapon_frag", "weapon_357", "weapon_physcannon", "weapon_shotgun", "weapon_ar2"], 
	d2_coast_04 = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1", "weapon_frag", "weapon_357", "weapon_physcannon", "weapon_shotgun", "weapon_ar2"], 
	d2_coast_05 = 			["weapon_crowbar", "weapon_pistol", "weapon_smg1", "weapon_frag", "weapon_357", "weapon_physcannon", "weapon_shotgun", "weapon_ar2"], 
	
	d2_coast_07 = 			allWeapons,
	d2_coast_08 = 			allWeapons,
	d2_coast_09 = 			allWeapons,
	d2_coast_10 = 			allWeapons,
	d2_coast_11 = 			allWeapons,
	
	d2_coast_11 =			allWeaponsBugBait,
	d2_coast_12 = 			allWeaponsBugBait,
	
	d2_prison_01 = 			allWeaponsBugBait,
	d2_prison_02 = 			allWeaponsBugBait,
	d2_prison_03 = 			allWeaponsBugBait,
	d2_prison_04 = 			allWeaponsBugBait,
	d2_prison_05 =			allWeaponsBugBait,
	d2_prison_06 = 			allWeaponsBugBait,
	d2_prison_07 = 			allWeaponsBugBait,
	d2_prison_08 =			allWeaponsBugBait,
	
	d3_c17_01 = 			allWeaponsBugBait,
	d3_c17_02 = 			allWeaponsBugBait,
	d3_c17_03 = 			allWeaponsBugBait,
	d3_c17_04 = 			allWeaponsBugBait,
	d3_c17_05 = 			allWeaponsBugBait,
	d3_c17_06a = 			allWeaponsBugBait,
	d3_c17_06b =			allWeaponsBugBait,
	d3_c17_07 = 			allWeaponsBugBait,
	d3_c17_08 = 			allWeaponsBugBait,
	d3_c17_09 = 			allWeaponsBugBait,
	d3_c17_10a =			allWeaponsBugBait,
	d3_c17_10b =			allWeaponsBugBait,
	d3_c17_11 = 			allWeaponsBugBait,
	d3_c17_12 = 			allWeaponsBugBait,
	d3_c17_12b =			allWeaponsBugBait,
	d3_c17_13 = 			allWeaponsBugBait,
	
	d3_citadel_01 = 		allWeaponsBugBait,
	d3_citadel_02 = 		allWeaponsBugBait,
	d3_citadel_03 = 		["weapon_physcannon"],
	d3_citadel_04 = 		["weapon_physcannon"],
	d3_citadel_05 = 		["weapon_physcannon"],
	
	d3_breen_01 = 			["weapon_physcannon"],
	
	// ===========================================================
	// episode 1
	
	ep1_citadel_01 = 		["weapon_physcannon"],
	ep1_citadel_02 = 		["weapon_physcannon"],
	ep1_citadel_02b = 		["weapon_physcannon"],
	ep1_citadel_03 = 		["weapon_physcannon"],
	ep1_citadel_04 = 		["weapon_physcannon"],
	
	ep1_c17_00 = 			["weapon_physcannon", "weapon_pistol", "weapon_shotgun"],
	ep1_c17_00a = 			["weapon_physcannon", "weapon_pistol", "weapon_shotgun"],
	
	ep1_c17_01 = 			["weapon_physcannon", "weapon_pistol", "weapon_shotgun", "weapon_smg1"],
	ep1_c17_01a = 			["weapon_physcannon", "weapon_pistol", "weapon_shotgun", "weapon_smg1"],
	
	ep1_c17_02 = 			["weapon_physcannon", "weapon_pistol", "weapon_shotgun", "weapon_smg1", "weapon_frag", "weapon_ar2"],
	ep1_c17_02b = 			["weapon_physcannon", "weapon_pistol", "weapon_shotgun", "weapon_smg1", "weapon_frag", "weapon_ar2", "weapon_crossbow"],
	ep1_c17_02a = 			["weapon_physcannon", "weapon_pistol", "weapon_shotgun", "weapon_smg1", "weapon_frag", "weapon_ar2", "weapon_crossbow", "weapon_crowbar", "weapon_rpg"],
	ep1_c17_05 = 			["weapon_physcannon", "weapon_pistol", "weapon_shotgun", "weapon_smg1", "weapon_frag", "weapon_ar2", "weapon_crossbow", "weapon_crowbar", "weapon_rpg", "weapon_357"],
	ep1_c17_06 = 			["weapon_physcannon", "weapon_pistol", "weapon_shotgun", "weapon_smg1", "weapon_frag", "weapon_ar2", "weapon_crossbow", "weapon_crowbar", "weapon_rpg", "weapon_357"],
	
	// ===========================================================
	// episode 2
	
	ep2_outland_01 = 		["weapon_physcannon"],
	ep2_outland_01a = 		["weapon_physcannon", "weapon_crowbar", "weapon_pistol", "weapon_357", "weapon_shotgun"],  // also gets 357 and shotgun
	ep2_outland_02 = 		["weapon_physcannon", "weapon_crowbar", "weapon_pistol", "weapon_357", "weapon_shotgun", "weapon_smg1"],
	ep2_outland_03 = 		["weapon_physcannon", "weapon_crowbar", "weapon_pistol", "weapon_357", "weapon_shotgun", "weapon_smg1", "weapon_frag"],
	ep2_outland_04 = 		["weapon_physcannon", "weapon_crowbar", "weapon_pistol", "weapon_357", "weapon_shotgun", "weapon_smg1", "weapon_frag"],
	ep2_outland_05 = 		["weapon_physcannon", "weapon_crowbar", "weapon_pistol", "weapon_357", "weapon_shotgun", "weapon_smg1", "weapon_frag"],
	ep2_outland_06 = 		["weapon_physcannon", "weapon_crowbar", "weapon_pistol", "weapon_357", "weapon_shotgun", "weapon_smg1", "weapon_frag", "weapon_ar2"],
	ep2_outland_06a = 		["weapon_physcannon", "weapon_crowbar", "weapon_pistol", "weapon_357", "weapon_shotgun", "weapon_smg1", "weapon_frag", "weapon_ar2"],
	ep2_outland_07 = 		["weapon_physcannon", "weapon_crowbar", "weapon_pistol", "weapon_357", "weapon_shotgun", "weapon_smg1", "weapon_frag", "weapon_ar2", "weapon_crossbow"],
	ep2_outland_08 = 		["weapon_physcannon", "weapon_crowbar", "weapon_pistol", "weapon_357", "weapon_shotgun", "weapon_smg1", "weapon_frag", "weapon_ar2", "weapon_crossbow"],
	ep2_outland_09 = 		["weapon_physcannon", "weapon_crowbar", "weapon_pistol", "weapon_357", "weapon_shotgun", "weapon_smg1", "weapon_frag", "weapon_ar2", "weapon_crossbow"],
	ep2_outland_10 = 		["weapon_physcannon", "weapon_crowbar", "weapon_pistol", "weapon_357", "weapon_shotgun", "weapon_smg1", "weapon_frag", "weapon_ar2", "weapon_crossbow"],
	ep2_outland_10a = 		["weapon_physcannon", "weapon_crowbar", "weapon_pistol", "weapon_357", "weapon_shotgun", "weapon_smg1", "weapon_frag", "weapon_ar2", "weapon_crossbow", "weapon_rpg"],
	ep2_outland_11 = 		["weapon_physcannon", "weapon_crowbar", "weapon_pistol", "weapon_357", "weapon_shotgun", "weapon_smg1", "weapon_frag", "weapon_ar2", "weapon_crossbow", "weapon_rpg"],
	ep2_outland_11a = 		["weapon_physcannon", "weapon_crowbar", "weapon_pistol", "weapon_357", "weapon_shotgun", "weapon_smg1", "weapon_frag", "weapon_ar2", "weapon_crossbow", "weapon_rpg"],
	ep2_outland_11b = 		["weapon_physcannon", "weapon_crowbar", "weapon_pistol", "weapon_357", "weapon_shotgun", "weapon_smg1", "weapon_frag", "weapon_ar2", "weapon_crossbow", "weapon_rpg"],
	ep2_outland_12 = 		["weapon_physcannon", "weapon_crowbar", "weapon_pistol", "weapon_357", "weapon_shotgun", "weapon_smg1", "weapon_frag", "weapon_ar2", "weapon_crossbow", "weapon_rpg"],
	ep2_outland_12a = 		["weapon_physcannon", "weapon_crowbar", "weapon_pistol", "weapon_357", "weapon_shotgun", "weapon_smg1", "weapon_frag", "weapon_ar2", "weapon_crossbow", "weapon_rpg"],
}


function HandleSuit()
{
	local equipSuit = true;
	for (local i = 0; i < mapsNoSuit.len(); i++)
	{
		if (GetMapName() == mapsNoSuit[i])
		{
			equipSuit = false;
			break;
		}
	}
	
	if (equipSuit)
	{
		self.EquipSuit( false )
	}
}


function HandleSpawnWeapons()
{
	local spawnWeapons = []
	
	try
	{
		spawnWeapons = mapSpawnWeapons[GetMapName()];
	}
	catch(exception)
	{
		printl( "Error: no map in mapSpawnWeapons table" );
		return;
	}
		
	printl( "giving items" )
	
	for (local wpn = 0; wpn < spawnWeapons.len(); wpn++)
	{
		self.GiveNamedItem(spawnWeapons[wpn]);
		
		// give default ammo
		for (local j = 0; j < weaponDefaultAmmo[spawnWeapons[wpn]].len(); j += 2)
		{
			self.GiveAmmo( weaponDefaultAmmo[spawnWeapons[wpn]][j], weaponDefaultAmmo[spawnWeapons[wpn]][j + 1], false );
		}
	}
}


function HandleSpawnPoint()
{
	// TODO: do this better with checkpoints
	/*if (GetMapName() == "d2_coast_08")
	{
		local spawnOrigin = Vector(3332.13, 1429.25, 1602.0);
		// local spawnAngles = Vector(3.0, -90.0, 0.0);
		local spawnVelocity = Vector(0.0, 0.0, 0.0);
		
		self.SetOrigin(spawnOrigin);
		self.SetAngles(3.0, -90.0, 0.0);
		self.SetVelocity(spawnVelocity);
	}*/
}


function PlayerSpawn()
{
	printl( "we are farmers" )
	
	// TODO: auto checkpoints with autosave positions?
	
	HandleSuit();
	HandleSpawnWeapons();
	HandleSpawnPoint();
}


ent <- 0;


function DispatchOnPostSpawn()
{
	printl("called DispatchOnPostSpawn")
	
	if (self.IsPlayer())
	{
		PlayerSpawn()
		
		// TEST
		/*if ( ent == 0 )
		{
			ent = CreateEntity( "npc_alyx" );
		}
		
		local precache = ent.IsPrecacheAllowed();
		ent.SetAllowPrecache( true );
		ent.PrecacheModel( "models/alyx.mdl" );
		ent.SetAllowPrecache( precache );
		
		ent.SetModel("models/alyx.mdl");*/
	}
}

