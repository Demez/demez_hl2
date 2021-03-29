//********************************************************************************************
//MAPSPAWN.nut is called on newgame or transitions
//********************************************************************************************
printl( "==== calling mapspawn.nut on map " + GetMapName() )


// ent <- ScriptCreateEntity( "npc_alyx" );



function DispatchOnPostSpawn()
{
	printl( "called DispatchOnPostSpawn" )
	
	LoadCheckpoints()
}

function LoadCheckpoints()
{
	printl( "called LoadCheckpoints" )
	
	if ( GetMapName() == "d1_trainstation_01" )
	{
		local chk1 = CreateEntity( "trigger_checkpoint" );
		chk1.SetIndex( 0 );
		
		// chk1.SetOrigin( Vector(-4211, -492, 28) );
		chk1.SetOrigin( Vector(-4318, -432, 68) );
		chk1.SetMaxSize( Vector(32, 32, 16) );
		
		chk1.SetPlayerSpawnOrigin( Vector(32, 32, 16) );
		chk1.SetPlayerSpawnAngles( Vector(32, 32, 16) );
	}
}


