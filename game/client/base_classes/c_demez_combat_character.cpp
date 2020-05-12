//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose:		Base NPC character with AI
//
//=============================================================================//

#include "cbase.h"
#include "c_demez_combat_character.h"

C_DemezCombatCharacter::C_DemezCombatCharacter()
{
}

C_DemezCombatCharacter::~C_DemezCombatCharacter()
{
}

int C_DemezCombatCharacter::DrawModel( int flags )
{
	RenderableInstance_t instance;
	return DrawModel(flags, instance);
}

int C_DemezCombatCharacter::DrawModel( int flags, const RenderableInstance_t &instance )
{
#if ENGINE_NEW
	return BaseClass::DrawModel(flags, instance);
#else
	return BaseClass::DrawModel(flags);
#endif
}

IMPLEMENT_CLIENTCLASS(C_DemezCombatCharacter, DT_DemezCombatCharacter, CDemezCombatCharacter);

// Only send active weapon index to local player
BEGIN_RECV_TABLE_NOBASE( C_DemezCombatCharacter, DT_DemezBCCLocalPlayerExclusive )
// RecvPropTime( RECVINFO( m_flNextAttack ) ),
END_RECV_TABLE();


BEGIN_RECV_TABLE(C_DemezCombatCharacter, DT_DemezCombatCharacter)
/*RecvPropDataTable( "dmz_bcc_localdata", 0, 0, &REFERENCE_RECV_TABLE(DT_DemezBCCLocalPlayerExclusive) ),
RecvPropEHandle( RECVINFO( m_hActiveWeapon ) ),
RecvPropArray3( RECVINFO_ARRAY(m_hMyWeapons), RecvPropEHandle( RECVINFO( m_hMyWeapons[0] ) ) ),
#ifdef GLOWS_ENABLE
RecvPropBool( RECVINFO( m_bGlowEnabled ) ),
#endif // GLOWS_ENABLE*/
END_RECV_TABLE()


BEGIN_PREDICTION_DATA( C_DemezCombatCharacter )
	/*DEFINE_PRED_ARRAY( m_iAmmo, FIELD_INTEGER,  MAX_AMMO_TYPES, FTYPEDESC_INSENDTABLE ),
	DEFINE_PRED_FIELD( m_flNextAttack, FIELD_FLOAT, FTYPEDESC_INSENDTABLE ),
	DEFINE_PRED_FIELD( m_hActiveWeapon, FIELD_EHANDLE, FTYPEDESC_INSENDTABLE ),
	DEFINE_PRED_ARRAY( m_hMyWeapons, FIELD_EHANDLE, MAX_WEAPONS, FTYPEDESC_INSENDTABLE ),*/
END_PREDICTION_DATA()
