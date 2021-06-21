//********************************************************************************************
// gamerules.nut is called on newgame or transitions
//********************************************************************************************

printl( "==== calling gamerules.nut on map " + GetMapName() )



function DispatchOnPostSpawn()
{
	printl( "called DispatchOnPostSpawn" )
	
	// LoadCheckpoints()
	ApplyMapFixes()
}


function LevelInitPostEntity()
{
	printl( "called LevelInitPostEntity" )
	
	LoadCheckpoints()
}


function ApplyMapFixes()
{
	RestoreFOV();
}


function RestoreFOV()
{

}




index <- 0;

function CreateCheckpoint()
{
	local ent = CreateEntity( "info_checkpoint" );
	ent.SetIndex( index );
	index++;
	return ent;
}


function LoadCheckpoints()
{
	printl( "called LoadCheckpoints" )
	
	// local temp = Entities.FindByName( "" );
	// global_newgame_template_base_items,ForceSpawn,,0,1
	// temp.AddOutput( "global_newgame_template_base_items,ForceSpawn,,0,1" );
	// temp.AddOutput( "global_newgame_template_base_items", "ForceSpawn", "", 0, 1 );
	
	// move to a separate file?
	// ====================================================================
	// Half-Life 2 Checkpoints
	// ====================================================================
	
	// =======================================================
	// Trainstation
	// =======================================================
	
	if ( GetMapName() == "d1_trainstation_01" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetIndex( 0 );
		chk0.SetTriggerOrigin( Vector(-4318, -600, -25) );
		chk0.SetTriggerSize( Vector(196, 128, 96) );
		chk0.CalcSpawn();
		chk0.SetAngles( Vector(0, 0, 0) );
		
		local chk1 = CreateCheckpoint();
		chk1.SetIndex( 1 );
		chk1.SetTriggerOrigin( Vector(-4064, -550, -25) );
		chk1.SetTriggerSize( Vector(64, 64, 96) );
		chk1.CalcSpawn();
		chk1.SetAngles( Vector(0, 0, 0) );
		
		local chk2 = CreateCheckpoint();
		chk2.SetIndex( 2 );
		chk2.SetTriggerOrigin( Vector(-3600, -440, -25) );
		chk2.SetTriggerSize( Vector(128, 128, 96) );
		chk2.CalcSpawn();
		chk2.SetAngles( Vector(0, 0, 0) );
		
		local chk3 = CreateCheckpoint();
		chk3.SetIndex( 3 );
		chk3.SetTriggerOrigin( Vector(-3450, -110, -25) );
		chk3.SetTriggerSize( Vector(128, 64, 96) );
		chk3.SetOrigin( Vector(-3573, 0, -25) );
		chk3.SetAngles( Vector(0, 0, 0) );
	}
	
	else if ( GetMapName() == "d1_trainstation_03" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetIndex( 0 );
		chk0.SetTriggerOrigin( Vector(-5000, -4800, 516) );
		chk0.SetTriggerSize( Vector(96, 96, 96) );
		chk0.SetOrigin( Vector(-4933, -4800, 516) );
		chk0.SetAngles( Vector(0, 180, 0) );
	}
	
	else if ( GetMapName() == "d1_trainstation_04" )
	{
		/*local chk0 = CreateCheckpoint();
		chk0.SetIndex( 0 );
		chk0.SetTriggerOrigin( Vector(-7620, -4032, -250) );
		chk0.SetTriggerSize( Vector(64, 160, 96) );
		chk0.SetOrigin( Vector(-7460, -4030, -250) );
		chk0.SetAngles( Vector(0, 180, 0) );*/
		
		local chk0 = CreateCheckpoint();
		chk0.SetOrigin( Vector(-7765, -3961, 388) );
		chk0.SetName( "checkpoint_elevator" );
		chk0.FindAndSetParent( "tracktrain_elevator", 0 );
		
		local chk0Trigger = Entities.FindByName( null, "trigger_elevator_go_down" );
		chk0Trigger.AddOutput( "OnTrigger", "checkpoint_elevator,SetActive,,0,-1" );  // targetname, input, parameter, delay, times to fire
	}
	
	else if ( GetMapName() == "d1_trainstation_05" )
	{
		local chk0 = CreateCheckpoint();
		// chk0.SetTriggerOrigin( Vector(-6520, -1140, 4) );
		// chk0.SetTriggerSize( Vector(64, 32, 96) );
		chk0.SetOrigin( Vector(-6492, -1178, 0) );
		chk0.SetAngles( 0, -90, 0 );
		chk0.SetName( "checkpoint_1" );
		
		local chk0Trigger = Entities.FindByName( null, "Alyx_lab_entry_closedoor_1" );
		chk0Trigger.AddOutput( "OnTrigger", "checkpoint_1,SetActive,,0,-1" );  // targetname, input, parameter, delay, times to fire
		
		local chk1 = CreateCheckpoint();
		// chk1.SetTriggerOrigin( Vector(-7096, -1452, 4) );
		// chk1.SetTriggerSize( Vector(32, 160, 96) );
		chk1.SetOrigin( Vector(-7164, -1415, 0) );
		chk1.SetAngles( 0, 90, 0 );
		chk1.SetName( "checkpoint_2" );
		
		local chk1Trigger = Entities.FindByName( null, "lab_door" );
		chk1Trigger.AddOutput( "OnFullyClosed", "checkpoint_2,SetActive,,0,-1" );  // targetname, input, parameter, delay, times to fire
		
		// TODO: add a checkpoint for the teleporter here
		
		local chk2 = CreateCheckpoint();
		chk2.SetIndex( 2 );
		chk2.SetTriggerOrigin( Vector(-10445, -4750, 320) );
		chk2.SetTriggerSize( Vector(160, 64, 96) );
		chk2.SetOrigin( Vector(-10431, -4717, 320) );
		chk2.SetAngles( 15, -180, 0 );
	}
	
	// =======================================================
	// Canals
	// =======================================================
	
	else if ( GetMapName() == "d1_canals_08" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetIndex( 0 );
		chk0.SetTriggerOrigin( Vector(-1850, -4470, -638) );
		chk0.SetTriggerSize( Vector(128, 750, 160) );
		chk0.SetOrigin( Vector(-1947, -4272, -638) );
		chk0.SetAngles( 0, 180, 0 );
	}
	
	else if ( GetMapName() == "d1_canals_11" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetIndex( 0 );
		chk0.SetTriggerOrigin( Vector(6600, 4764, -983) );
		chk0.SetTriggerSize( Vector(64, 260, 128) );
		chk0.SetOrigin( Vector(6455, 4995, -953) );
		chk0.SetAngles( 0, 180, 0 );
		
		local chk1 = CreateCheckpoint();
		chk1.SetIndex( 1 );
		chk1.SetTriggerOrigin( Vector(5700, 4752, -965) );
		chk1.SetTriggerSize( Vector(64, 280, 128) );
		chk1.SetOrigin( Vector(5754, 4685, -895) );
		chk1.SetAngles( 0, 136, 0 );
	}
	
	// =======================================================
	// Eli
	// =======================================================
	
	else if ( GetMapName() == "d1_eli_01" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetIndex( 0 );
		chk0.SetTriggerOrigin( Vector(-150, 2636, -1279) );
		chk0.SetTriggerSize( Vector(160, 200, 96) );
		chk0.CalcSpawn();
		
		local chk1 = CreateCheckpoint();
		chk1.SetIndex( 1 );
		chk1.SetTriggerOrigin( Vector(224, 2016, -1279) );
		chk1.SetTriggerSize( Vector(64, 96, 96) );
		chk1.CalcSpawn();
		
		local chk2 = CreateCheckpoint();
		chk2.SetIndex( 2 );
		chk2.SetTriggerOrigin( Vector(376, 1776, -2736) );
		chk2.SetTriggerSize( Vector(160, 64, 96) );
		chk2.CalcSpawn();
		chk2.SetAngles( 0, 90, 0 );
		
		local chk3 = CreateCheckpoint();
		chk3.SetIndex( 3 );
		chk3.SetTriggerOrigin( Vector(196, 2080, -2736) );
		chk3.SetTriggerSize( Vector(32, 96, 96) );
		chk3.CalcSpawn();
		chk3.SetAngles( 0, 180, 0 );
		
		local chk4 = CreateCheckpoint();
		chk4.SetIndex( 4 );
		chk4.SetTriggerOrigin( Vector(-532, 2080, -2736) );
		chk4.SetTriggerSize( Vector(32, 96, 96) );
		chk4.CalcSpawn();
		chk4.SetAngles( 0, 180, 0 );
	}
	
	else if ( GetMapName() == "d1_eli_02" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetIndex( 0 );
		chk0.SetTriggerOrigin( Vector(-2000, 1840, -2735) );
		chk0.SetTriggerSize( Vector(96, 160, 96) );
		chk0.CalcSpawn();
		chk0.SetAngles( 0, 180, 0 );
	}
	
	// =======================================================
	// Town
	// =======================================================
	
	else if ( GetMapName() == "d1_town_02" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetIndex( 0 );
		chk0.SetTriggerOrigin( Vector(-3002, 862, -3332) );
		chk0.SetTriggerSize( Vector(96, 96, 64) );
		chk0.CalcSpawn();
		chk0.SetAngles( 0, -90, 0 );
		
		local chk1 = CreateCheckpoint();
		chk1.SetIndex( 1 );
		chk1.SetTriggerOrigin( Vector(-4400, 1420, -3007) );
		chk1.SetEndOrigin( Vector(-4300, 1480, 96) );  // z is added to origin, like a height value
		chk1.CalcSpawn();
		chk1.SetAngles( 0, -90, 0 );
		
		// TODO: probably add one here for when we change back to this level, idk
		// also crashed in vscript, odd
		
		// TODO: add a third one here for when we jump out of the crane?
	}
	
	else if ( GetMapName() == "d1_town_02a" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetIndex( 0 );
		chk0.SetTriggerOrigin( Vector(-7535, -295, -3407) );
		// chk0.SetTriggerSize( Vector(96, 96, 64) );
		chk0.SetEndOrigin( Vector(-7496, -163, 96) );  // z is added to origin, like a height value
		chk0.CalcSpawn();
		chk0.SetAngles( 0, 0, 0 );
	}
	
	else if ( GetMapName() == "d1_town_05" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetIndex( 0 );
		chk0.SetTriggerOrigin( Vector(-3426, 7375, 897) );
		chk0.SetTriggerSize( Vector(96, 350, 96) );
		// chk0.SetEndOrigin( Vector(-3223`, 7734, 96) );  // z is added to origin, like a height value
		chk0.CalcSpawn();
		chk0.SetAngles( 0, -27, 0 );
	}
	
	// =======================================================
	// Coast
	// =======================================================
	
	// bridge
	else if ( GetMapName() == "d2_coast_08" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetIndex( 0 );
		chk0.SetTriggerOrigin( Vector(3282, 1380, 1537) );
		chk0.SetTriggerSize( Vector(96, 64, 96) );
		chk0.CalcSpawn();
		chk0.SetAngles( 0, -90, 0 );
		chk0.SetTeleport( false );
		
		local chk1 = CreateCheckpoint();
		chk1.SetIndex( 1 );
		chk1.SetTriggerOrigin( Vector(2840, -7133, 1920) );
		chk1.SetTriggerSize( Vector(200, 192, 96) );
		chk1.CalcSpawn();
		chk1.SetAngles( 0, -90, 0 );
		chk1.SetTeleport( false );
	}
	
	else if ( GetMapName() == "d2_coast_11" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetIndex( 0 );
		chk0.SetTriggerOrigin( Vector(4200, 6986, 133) );
		chk0.SetTriggerSize( Vector(650, 256, 96) );
		chk0.SetOrigin( Vector(4587, 7162, 133) );
		chk0.SetAngles( 0, 90, 0 );
	}
	
	else if ( GetMapName() == "d2_coast_12" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetIndex( 0 );
		chk0.SetTriggerOrigin( Vector(2060, 1445, 672) );
		chk0.SetTriggerSize( Vector(180, 128, 96) );
		chk0.CalcSpawn();
		chk0.SetAngles( 0, 90, 0 );
	}
	
	// =======================================================
	// Prison
	// =======================================================
	
	else if ( GetMapName() == "d2_prison_03" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetIndex( 0 );
		chk0.SetTriggerOrigin( Vector(-3270, 1360, 0) );
		chk0.SetTriggerSize( Vector(450, 700, 120) );
		chk0.CalcSpawn();
		chk0.SetAngles( 0, 90, 0 );
	}
	
	else if ( GetMapName() == "d2_prison_04" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetIndex( 0 );
		chk0.SetTriggerOrigin( Vector(-250, 1424, 256) );
		chk0.SetTriggerSize( Vector(110, 160, 96) );
		chk0.CalcSpawn();
		chk0.SetAngles( 0, 180, 0 );
	}
	
	else if ( GetMapName() == "d2_prison_05" )
	{
		/*local chk0 = CreateCheckpoint();
		chk0.SetIndex( 0 );
		chk0.SetTriggerOrigin( Vector(-288, -50, 512) );
		chk0.SetTriggerSize( Vector(280, 290, 96) );
		chk0.CalcSpawn();
		chk0.SetAngles( 0, 180, 0 );*/
		
		local chk0 = CreateCheckpoint();
		chk0.SetTriggerOrigin( Vector(-288, -50, 512) );
		chk0.SetTriggerSize( Vector(280, 290, 96) );
		chk0.CalcSpawn();
		chk0.SetAngles( 0, 180, 0 );
	}
	
	else if ( GetMapName() == "d2_prison_06" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetOrigin( Vector(1572, 678, -706) );
		chk0.SetAngles( 0, 180, 0 );
		chk0.FindAndSetParent( "introom_elevator_1", 0 );
		chk0.SetName( "elevator_checkpoint" );
		
		local chkTrigger = Entities.FindByName( null, "introom_elevator_doors_1" );
		chkTrigger.AddOutput( "OnClose", "elevator_checkpoint,SetActive,,0,-1" );  // targetname, input, parameter, delay, times to fire
		
		local chk1 = CreateCheckpoint();
		chk1.SetTriggerOrigin( Vector(270, -210, 0) );
		chk1.SetTriggerSize( Vector(80, 80, 96) );
		chk1.SetOrigin( Vector(312, -357, -64) );
		chk1.SetAngles( 0, -90, 0 );
		
		local chk2 = CreateCheckpoint();
		chk2.SetTriggerOrigin( Vector(490, -1007, 0) );
		chk2.SetTriggerSize( Vector(96, 160, 96) );
		
		local chk3 = CreateCheckpoint();
		chk3.SetTriggerOrigin( Vector(1020, -2068, -240) );
		chk3.SetTriggerSize( Vector(340, 200, 72) );
	}
	
	else if ( GetMapName() == "d2_prison_07" )
	{
		// this work better with an entity output
		local chk0 = CreateCheckpoint();
		chk0.SetIndex( 0 );
		// chk0.SetTriggerOrigin( Vector(-520, -2950, -240) );
		// chk0.SetTriggerSize( Vector(160, 160, 96) );
		chk0.SetOrigin( Vector(-439, -2860, -239) );
		chk0.SetAngles( 0, -90, 0 );
		chk0.SetName( "door_exit_gate_checkpoint" );
		// chk0.Disable();
		
		local door = Entities.FindByName( null, "door_croom2_gate" );
		// targetname, input, parameter, delay, times to fire
		door.AddOutput( "OnClose", "door_exit_gate_checkpoint,SetActive,,0,-1" );
		// door.AddOutput( "OnClose", "door_exit_gate_checkpoint", "SetActive", ",,0,-1" );
		// door.ConnectOutput( "OnFullyOpen", "TestFunc" );
		
		local chk1 = CreateCheckpoint();
		chk1.SetIndex( 1 );
		chk1.SetTriggerOrigin( Vector(1620, -3475, -680) );
		chk1.SetTriggerSize( Vector(120, 80, 96) );
		chk1.SetOrigin( Vector(1697, -3459, -680) );
		chk1.SetAngles( 0, -90, 0 );
		
		local chk2 = CreateCheckpoint();
		chk2.SetIndex( 2 );
		chk2.SetTriggerOrigin( Vector(2940, -3472, -800) );
		chk2.SetTriggerSize( Vector(256, 140, 96) );
		// chk2.CalcSpawn();
		chk2.SetAngles( 0, 90, 0 );
		
		local chk3 = CreateCheckpoint();
		chk3.SetIndex( 3 );
		chk3.SetTriggerOrigin( Vector(4000, -4079, -544) );
		chk3.SetTriggerSize( Vector(320, 160, 96) );
		// chk3.CalcSpawn();
		chk3.SetAngles( 0, 90, 0 );
	}
	
	else if ( GetMapName() == "d2_prison_08" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetIndex( 0 );
		chk0.SetTriggerOrigin( Vector(-1044, 784, 960) );
		chk0.SetTriggerSize( Vector(280, 190, 96) );
		chk0.SetAngles( 0, -90, 0 );
		
		local chk1 = CreateCheckpoint();
		chk1.SetIndex( 1 );
		chk1.SetTriggerOrigin( Vector(-480, 600, 928) );
		chk1.SetTriggerSize( Vector(96, 48, 96) );
		chk1.SetAngles( 0, -90, 0 );
		
		local chk2 = CreateCheckpoint();
		chk2.SetIndex( 2 );
		chk2.SetTriggerOrigin( Vector(-220, 510, 928) );
		chk2.SetTriggerSize( Vector(50, 120, 96) );
		chk2.SetOrigin( Vector(-195, 770, 960) );
	}
	
	// =======================================================
	// City 17
	// =======================================================
	
	else if ( GetMapName() == "d3_c17_06b" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetIndex( 0 );
		chk0.SetTriggerOrigin( Vector(3296, 1624, 0) );
		chk0.SetTriggerSize( Vector(300, 200, 96) );
		chk0.SetOrigin( Vector(3357, 1717, 0) );
	}
	
	else if ( GetMapName() == "d3_c17_07" )
	{
		// idk if i should use this, just a quicker spawn point, and you don't get supply crates either, idk
		local chk0 = CreateCheckpoint();
		chk0.SetTriggerOrigin( Vector(4176, 744, 128) );
		chk0.SetTriggerSize( Vector(430, 200, 96) );
		chk0.SetTeleport( false );
		
		local chk1 = CreateCheckpoint();
		chk1.SetTriggerOrigin(	Vector(4791, 925, 0) );
		chk1.SetTriggerSize( Vector(200, 80, 96) );
		chk1.SetOrigin( Vector(5341, 1407. 0) );
		chk1.SetAngles( 0, 0, 0 );
		chk1.SetTeleport( false );
		
		local chk2 = CreateCheckpoint();
		chk2.SetTriggerOrigin( Vector(7300, 1290, 0) );
		chk2.SetTriggerSize( Vector(160, 480, 128) );
	}
	
	else if ( GetMapName() == "d3_c17_08" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetTriggerOrigin( Vector(-410, -1520, -207) );
		chk0.SetTriggerSize( Vector(160, 160, 96) );
		// chk0.SetAngles( 0, 180, 0 );
		chk0.SetTeleport( false );
		
		local chk1 = CreateCheckpoint();
		chk1.SetTriggerOrigin( Vector(1468, -940, 80) );
		chk1.SetTriggerSize( Vector(140, 240, 96) );
		chk1.CalcSpawn();
		chk1.SetAngles( 0, 180, 0 );
		chk1.SetTeleport( false );
		
		local chk2 = CreateCheckpoint();
		chk2.SetTriggerOrigin( Vector(880, -749, 400) );
		chk2.SetEndOrigin( Vector(1584, 624, 400) );
		// chk2.SetTriggerSize( Vector(140, 240, 96) );
		chk2.SetOrigin( Vector(1488, 577, 400) );
		chk2.SetAngles( 0, -180, 0 );
	}
	
	else if ( GetMapName() == "d3_c17_10a" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetTriggerOrigin( Vector(-2917, 8336, 128) );
		chk0.SetTriggerSize( Vector(160, 96, 96) );
	}
	
	else if ( GetMapName() == "d3_c17_10b" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetTriggerOrigin( Vector(2607, -982, 256) );
		chk0.SetTriggerSize( Vector(160, 160, 96) );
		chk0.SetAngles( 0, -180, 0 );
		
		local chk1 = CreateCheckpoint();
		chk1.SetTriggerOrigin( Vector(3454, 878, 512) );
		chk1.SetTriggerSize( Vector(96, 192, 96) );
		chk1.SetOrigin( Vector(3159, 886, 512) );
		chk1.SetAngles( 0, -180, 0 );
	}
	
	else if ( GetMapName() == "d3_c17_12b" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetTriggerOrigin( Vector(-8864, -1344, -256) );
		chk0.SetTriggerSize( Vector(96, 96, 96) );
		chk0.SetOrigin( Vector(-8933, -1320, -256) );
		chk0.SetAngles( 0, 90, 0 );
	}
	
	else if ( GetMapName() == "d3_c17_13" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetTriggerOrigin( Vector(5442, -352, -128) );
		chk0.SetTriggerSize( Vector(380, 580, 256) );
		chk0.SetOrigin( Vector(5717, 146, -128) );
		chk0.SetAngles( 0, -90, 0 );
	}
	
	// =======================================================
	// Citidel + Breen
	// =======================================================
	
	else if ( GetMapName() == "d3_citadel_03" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetTriggerOrigin( Vector(3440, 569, 2368) );
		chk0.SetTriggerSize( Vector(96, 96, 96) );
		chk0.CalcSpawn();
		chk0.SetAngles( 0, 180, 0 );
	}
	
	else if ( GetMapName() == "d3_citadel_04" )
	{
		// parent testing
		local chk0 = CreateCheckpoint();
		chk0.SetOrigin( Vector(254, 831, 6410) ); // spawn on the parent
		chk0.SetAngles( 0, -90, 0 );
		chk0.SetName( "elevator_checkpoint" );
		chk0.FindAndSetParent( "citadel_train_lift01_1", 0 );
		
		local elevUp = Entities.FindByName( null, "citadel_trigger_elevatorride_up" );
		elevUp.AddOutput( "OnTrigger", "elevator_checkpoint,SetActive,,0,-1" );  // targetname, input, parameter, delay, times to fire
		
		local chk1 = CreateCheckpoint();
		chk1.SetTriggerOrigin( Vector(-1800, -3375, 6144) );
		chk1.SetTriggerSize( Vector(300, 1300, 96) );
		chk1.CalcSpawn();
		chk1.SetAngles( 0, -90, 0 );
	}
	
	else if ( GetMapName() == "d3_breen_01" )
	{
		local chk0 = CreateCheckpoint();
		chk0.SetTriggerOrigin( Vector(-2016, -50, 580) );
		chk0.SetTriggerSize( Vector(96, 96, 96) );
		chk0.SetAngles( 0, 0, 0 );
		chk0.SetOrigin(-1970, 0, 1339);  // set on the parent
		chk0.GetSpawn().FindAndSetParent( "Train_lift", 0 );
		
		local chk1 = CreateCheckpoint();
		chk1.SetTriggerOrigin( Vector(-1140, 390, 1310) );
		chk1.SetTriggerSize( Vector(160, 140, 96) );
		chk1.SetAngles( 0, -90, 0 );
		chk1.CalcSpawn();
		chk1.GetSpawn().FindAndSetParent( "Train_lift_TP", 0 );
	}
	
	// =======================================================
	// Episode 1 - Citidel
	// =======================================================
	
	// =======================================================
	// Episode 1 - City 17
	// =======================================================
	
	// =======================================================
	// Episode 2
	// =======================================================
}


