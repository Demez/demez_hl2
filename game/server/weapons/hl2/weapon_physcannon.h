//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#pragma once

class CGrabController;

//-----------------------------------------------------------------------------
// Do we have the super-phys gun?
//-----------------------------------------------------------------------------
bool PlayerHasMegaPhysCannon();

// force the physcannon to drop an object (if carried)
void PhysCannonForceDrop( CBaseCombatWeapon *pActiveWeapon, CBaseEntity *pOnlyIfHoldingThis );
void PhysCannonBeginUpgrade( CBaseAnimating *pAnim );

bool PlayerPickupControllerIsHoldingEntity( CBaseEntity *pPickupController, CBaseEntity *pHeldEntity );
float PlayerPickupGetHeldObjectMass( CBaseEntity *pPickupControllerEntity, IPhysicsObject *pHeldObject );
float PhysCannonGetHeldObjectMass( CBaseCombatWeapon *pActiveWeapon, IPhysicsObject *pHeldObject );

CBaseEntity *PhysCannonGetHeldEntity( CBaseCombatWeapon *pActiveWeapon );
CBaseEntity *GetPlayerHeldEntity( CBasePlayer *pPlayer );

bool PhysCannonAccountableForObject( CBaseCombatWeapon *pPhysCannon, CBaseEntity *pObject );

// new portal shit
void ShutdownPickupController( CBaseEntity *pPickupControllerEntity );
CBasePlayer *GetPlayerHoldingEntity( CBaseEntity *pEntity );
CGrabController *GetGrabControllerForPlayer( CBasePlayer *pPlayer );
CGrabController *GetGrabControllerForPhysCannon( CBaseCombatWeapon *pActiveWeapon );
void GetSavedParamsForCarriedPhysObject( CGrabController *pGrabController, IPhysicsObject *pObject, float *pSavedMassOut, float *pSavedRotationalDampingOut );
void UpdateGrabControllerTargetPosition( CBasePlayer *pPlayer, Vector *vPosition, QAngle *qAngles );
void GrabController_SetPortalPenetratingEntity( CGrabController *pController, CBaseEntity *pPenetrated );


