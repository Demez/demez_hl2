#include "cbase.h"
#include "d_gamemanager.h"
#include "vphysics_interface.h"
#include "vphysics/performance.h"


void UpdatePhysEnvironment();

static void PhysCallback( IConVar *var, const char *pOldValue, float flOldValue )
{
	UpdatePhysEnvironment();
}

/*
* 		maxCollisionsPerObjectPerTimestep = 6;
		maxCollisionChecksPerTimestep = 250;
		maxVelocity = 2000.0f;
		maxAngularVelocity = 360.0f * 10.0f;
		lookAheadTimeObjectsVsWorld = 1.0f;
		lookAheadTimeObjectsVsObject = 0.5f;
		minFrictionMass = DEFAULT_MIN_FRICTION_MASS;
		maxFrictionMass = DEFAULT_MAX_FRICTION_MASS;
*/


ConVar d_phys_maxCollisionsPerObjectPerTimestep( "d_phys_maxCollisionsPerObjectPerTimestep", "6", 0,    "object will be frozen after this many collisions (visual hitching vs. CPU cost)", PhysCallback );
ConVar d_phys_maxCollisionChecksPerTimestep( "d_phys_maxCollisionChecksPerTimestep", "250", 0,          "objects may penetrate after this many collision checks (can be extended in AdditionalCollisionChecksThisTick)", PhysCallback );
ConVar d_phys_maxVelocity( "d_phys_maxVelocity", "2000.0", 0,                                           "limit world space linear velocity to this (in / s)", PhysCallback );
ConVar d_phys_maxAngularVelocity( "d_phys_maxAngularVelocity", "3600.0", 0,                             "limit world space angular velocity to this (degrees / s)", PhysCallback );
ConVar d_phys_lookAheadTimeObjectsVsWorld( "d_phys_lookAheadTimeObjectsVsWorld", "1.0", 0,              "predict collisions this far (seconds) into the future", PhysCallback );
ConVar d_phys_lookAheadTimeObjectsVsObject( "d_phys_lookAheadTimeObjectsVsObject", "0.5", 0,            "predict collisions this far (seconds) into the future", PhysCallback );
ConVar d_phys_minFrictionMass( "d_phys_minFrictionMass", "10.0", 0,                                     "min mass for friction solves (constrains dynamic range of mass to improve stability)", PhysCallback );
ConVar d_phys_maxFrictionMass( "d_phys_maxFrictionMass", "2500.0", 0,                                   "mas mass for friction solves", PhysCallback );


void UpdatePhysEnvironment()
{
	physics_performanceparams_t params;
	// physenv->GetPerformanceSettings( &params );
	params.Defaults();

	if ( d_phys_maxCollisionsPerObjectPerTimestep.GetInt() > -1 )
		params.maxCollisionsPerObjectPerTimestep = d_phys_maxCollisionsPerObjectPerTimestep.GetInt();

	if ( d_phys_maxCollisionChecksPerTimestep.GetInt() > -1 )
		params.maxCollisionChecksPerTimestep = d_phys_maxCollisionChecksPerTimestep.GetInt();

	if ( d_phys_maxVelocity.GetFloat() > -1.0 )
		params.maxVelocity = d_phys_maxVelocity.GetFloat();

	if ( d_phys_maxAngularVelocity.GetFloat() > -1.0 )
		params.maxAngularVelocity = d_phys_maxAngularVelocity.GetFloat();

	if ( d_phys_lookAheadTimeObjectsVsWorld.GetFloat() > -1.0 )
		params.lookAheadTimeObjectsVsWorld = d_phys_lookAheadTimeObjectsVsWorld.GetFloat();

	if ( d_phys_lookAheadTimeObjectsVsObject.GetFloat() > -1.0 )
		params.lookAheadTimeObjectsVsObject = d_phys_lookAheadTimeObjectsVsObject.GetFloat();

	if ( d_phys_minFrictionMass.GetFloat() > -1.0 )
		params.minFrictionMass = d_phys_minFrictionMass.GetFloat();

	if ( d_phys_maxFrictionMass.GetFloat() > -1.0 )
		params.maxFrictionMass = d_phys_maxFrictionMass.GetFloat();

	physenv->SetPerformanceSettings( &params );
}


