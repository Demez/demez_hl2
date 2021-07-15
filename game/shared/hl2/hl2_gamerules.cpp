//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ====
//
// Purpose: The Half-Life 2 game rules, such as the relationship tables and ammo
//			damage cvars.
//
//=============================================================================

#include "cbase.h"
#include "hl2_gamerules.h"
#include "ammodef.h"
#include "hl2_shareddefs.h"
#include "d_ammodef.h"

#ifdef CLIENT_DLL

#else
	#include "player.h"
	#include "game.h"
	#include "gamerules.h"
	#include "teamplay_gamerules.h"
	#include "hl2_player.h"
	#include "voice_gamemgr.h"
	#include "globalstate.h"
	#include "ai_basenpc.h"
	#include "weapon_physcannon.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


REGISTER_GAMERULES_CLASS( CHalfLife2 );

BEGIN_NETWORK_TABLE_NOBASE( CHalfLife2, DT_HL2GameRules )
	#ifdef CLIENT_DLL
		RecvPropBool( RECVINFO( m_bMegaPhysgun ) ),
	#else
		SendPropBool( SENDINFO( m_bMegaPhysgun ) ),
	#endif
END_NETWORK_TABLE()


LINK_ENTITY_TO_CLASS_DUMB( hl2_gamerules, CHalfLife2Proxy );
IMPLEMENT_NETWORKCLASS_ALIASED( HalfLife2Proxy, DT_HalfLife2Proxy )


#ifdef CLIENT_DLL
	void RecvProxy_HL2GameRules( const RecvProp *pProp, void **pOut, void *pData, int objectID )
	{
		CHalfLife2 *pRules = ActualHL2GameRules();
		Assert( pRules );
		*pOut = pRules;
	}

	BEGIN_RECV_TABLE( CHalfLife2Proxy, DT_HalfLife2Proxy )
		RecvPropDataTable( "hl2_gamerules_data", 0, 0, &REFERENCE_RECV_TABLE( DT_HL2GameRules ), RecvProxy_HL2GameRules )
	END_RECV_TABLE()
#else
	void* SendProxy_HL2GameRules( const SendProp *pProp, const void *pStructBase, const void *pData, CSendProxyRecipients *pRecipients, int objectID )
	{
		CHalfLife2 *pRules = ActualHL2GameRules();
		Assert( pRules );
		pRecipients->SetAllRecipients();
		return pRules;
	}

	BEGIN_SEND_TABLE( CHalfLife2Proxy, DT_HalfLife2Proxy )
		SendPropDataTable( "hl2_gamerules_data", 0, &REFERENCE_SEND_TABLE( DT_HL2GameRules ), SendProxy_HL2GameRules )
	END_SEND_TABLE()
#endif

// ConVar  physcannon_mega_enabled( "physcannon_mega_enabled", "0", FCVAR_CHEAT | FCVAR_REPLICATED );

// Controls the application of the robus radius damage model.
ConVar	sv_robust_explosions( "sv_robust_explosions","1", FCVAR_REPLICATED );

// Damage scale for damage inflicted by the player on each skill level.
ConVar	sk_dmg_inflict_scale1( "sk_dmg_inflict_scale1", "1.50", FCVAR_REPLICATED );
ConVar	sk_dmg_inflict_scale2( "sk_dmg_inflict_scale2", "1.00", FCVAR_REPLICATED );
ConVar	sk_dmg_inflict_scale3( "sk_dmg_inflict_scale3", "0.75", FCVAR_REPLICATED );

// Damage scale for damage taken by the player on each skill level.
ConVar	sk_dmg_take_scale1( "sk_dmg_take_scale1", "0.50", FCVAR_REPLICATED );
ConVar	sk_dmg_take_scale2( "sk_dmg_take_scale2", "1.00", FCVAR_REPLICATED );
#ifdef HL2_EPISODIC
	ConVar	sk_dmg_take_scale3( "sk_dmg_take_scale3", "2.0", FCVAR_REPLICATED );
#else
	ConVar	sk_dmg_take_scale3( "sk_dmg_take_scale3", "1.50", FCVAR_REPLICATED );
#endif//HL2_EPISODIC

ConVar	sk_allow_autoaim( "sk_allow_autoaim", "1", FCVAR_REPLICATED );

// Autoaim scale
ConVar	sk_autoaim_scale1( "sk_autoaim_scale1", "1.0", FCVAR_REPLICATED );
ConVar	sk_autoaim_scale2( "sk_autoaim_scale2", "0.5", FCVAR_REPLICATED );
ConVar	sk_autoaim_scale3( "sk_autoaim_scale3", "0.0", FCVAR_REPLICATED ); //NOT CURRENTLY OFFERED ON SKILL 3

// Quantity scale for ammo received by the player.
ConVar	sk_ammo_qty_scale1 ( "sk_ammo_qty_scale1", "1.20", FCVAR_REPLICATED );
ConVar	sk_ammo_qty_scale2 ( "sk_ammo_qty_scale2", "1.00", FCVAR_REPLICATED );
ConVar	sk_ammo_qty_scale3 ( "sk_ammo_qty_scale3", "0.60", FCVAR_REPLICATED );

ConVar	sk_plr_health_drop_time		( "sk_plr_health_drop_time", "30", FCVAR_REPLICATED );
ConVar	sk_plr_grenade_drop_time	( "sk_plr_grenade_drop_time", "30", FCVAR_REPLICATED );

ConVar	sk_plr_dmg_small_round	( "sk_plr_dmg_small_round", "0", FCVAR_REPLICATED );
ConVar	sk_npc_dmg_small_round	( "sk_npc_dmg_small_round", "0", FCVAR_REPLICATED );
ConVar	sk_max_small_round		( "sk_max_small_round", "0", FCVAR_REPLICATED );

ConVar	sk_plr_dmg_medium_round	( "sk_plr_dmg_medium_round", "0", FCVAR_REPLICATED );
ConVar	sk_npc_dmg_medium_round	( "sk_npc_dmg_medium_round", "0", FCVAR_REPLICATED );
ConVar	sk_max_medium_round		( "sk_max_medium_round", "0", FCVAR_REPLICATED );

ConVar	sk_plr_dmg_large_round	( "sk_plr_dmg_large_round", "0", FCVAR_REPLICATED );
ConVar	sk_npc_dmg_large_round	( "sk_npc_dmg_large_round", "0", FCVAR_REPLICATED );
ConVar	sk_max_large_round		( "sk_max_large_round", "0", FCVAR_REPLICATED );

ConVar	sk_plr_dmg_ar2			( "sk_plr_dmg_ar2","0", FCVAR_REPLICATED );
ConVar	sk_npc_dmg_ar2			( "sk_npc_dmg_ar2","0", FCVAR_REPLICATED);
ConVar	sk_max_ar2				( "sk_max_ar2","0", FCVAR_REPLICATED);
ConVar	sk_max_ar2_altfire		( "sk_max_ar2_altfire","0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_alyxgun		( "sk_plr_dmg_alyxgun","0", FCVAR_REPLICATED );
ConVar	sk_npc_dmg_alyxgun		( "sk_npc_dmg_alyxgun","0", FCVAR_REPLICATED);
ConVar	sk_max_alyxgun			( "sk_max_alyxgun","0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_pistol		( "sk_plr_dmg_pistol","0", FCVAR_REPLICATED );
ConVar	sk_npc_dmg_pistol		( "sk_npc_dmg_pistol","0", FCVAR_REPLICATED);
ConVar	sk_max_pistol			( "sk_max_pistol","0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_smg1			( "sk_plr_dmg_smg1","0", FCVAR_REPLICATED );
ConVar	sk_npc_dmg_smg1			( "sk_npc_dmg_smg1","0", FCVAR_REPLICATED);
ConVar	sk_max_smg1				( "sk_max_smg1","0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_flare_round	( "sk_plr_dmg_flare_round","0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_flare_round	( "sk_npc_dmg_flare_round","0", FCVAR_REPLICATED);
ConVar	sk_max_flare_round		( "sk_max_flare_round","0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_buckshot		( "sk_plr_dmg_buckshot","0", FCVAR_REPLICATED);	
ConVar	sk_npc_dmg_buckshot		( "sk_npc_dmg_buckshot","0", FCVAR_REPLICATED);
ConVar	sk_max_buckshot			( "sk_max_buckshot","0", FCVAR_REPLICATED);
ConVar	sk_plr_num_shotgun_pellets( "sk_plr_num_shotgun_pellets","7", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_rpg_round	( "sk_plr_dmg_rpg_round","0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_rpg_round	( "sk_npc_dmg_rpg_round","0", FCVAR_REPLICATED);
ConVar	sk_max_rpg_round		( "sk_max_rpg_round","0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_sniper_round	( "sk_plr_dmg_sniper_round","0", FCVAR_REPLICATED);	
ConVar	sk_npc_dmg_sniper_round	( "sk_npc_dmg_sniper_round","0", FCVAR_REPLICATED);
ConVar	sk_max_sniper_round		( "sk_max_sniper_round","0", FCVAR_REPLICATED);

ConVar	sk_max_slam				( "sk_max_slam","0", FCVAR_REPLICATED);
ConVar	sk_max_tripwire			( "sk_max_tripwire","0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_molotov		( "sk_plr_dmg_molotov","0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_molotov		( "sk_npc_dmg_molotov","0", FCVAR_REPLICATED);
ConVar	sk_max_molotov			( "sk_max_molotov","0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_grenade		( "sk_plr_dmg_grenade","0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_grenade		( "sk_npc_dmg_grenade","0", FCVAR_REPLICATED);
ConVar	sk_max_grenade			( "sk_max_grenade","0", FCVAR_REPLICATED);

#ifdef HL2_EPISODIC
ConVar	sk_max_hopwire			( "sk_max_hopwire", "3", FCVAR_REPLICATED);
ConVar	sk_max_striderbuster	( "sk_max_striderbuster", "3", FCVAR_REPLICATED);
#endif

ConVar sk_plr_dmg_brickbat	( "sk_plr_dmg_brickbat","0", FCVAR_REPLICATED);
ConVar sk_npc_dmg_brickbat	( "sk_npc_dmg_brickbat","0", FCVAR_REPLICATED);
ConVar sk_max_brickbat		( "sk_max_brickbat","0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_smg1_grenade	( "sk_plr_dmg_smg1_grenade","0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_smg1_grenade	( "sk_npc_dmg_smg1_grenade","0", FCVAR_REPLICATED);
ConVar	sk_max_smg1_grenade		( "sk_max_smg1_grenade","0", FCVAR_REPLICATED );

ConVar	sk_plr_dmg_ml_grenade		( "sk_plr_dmg_ml_grenade","0", FCVAR_REPLICATED);
ConVar	sk_npc_dmg_ml_grenade		( "sk_npc_dmg_ml_grenade","0", FCVAR_REPLICATED);
ConVar	sk_max_ml_grenade			( "sk_max_ml_grenade","0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_ar2_grenade	( "sk_plr_dmg_ar2_grenade", "0", FCVAR_REPLICATED );
ConVar	sk_npc_dmg_ar2_grenade	( "sk_npc_dmg_ar2_grenade", "0", FCVAR_REPLICATED );
ConVar	sk_max_ar2_grenade		( "sk_max_ar2_grenade", "0", FCVAR_REPLICATED );

ConVar	sk_plr_dmg_357			( "sk_plr_dmg_357", "0", FCVAR_REPLICATED );
ConVar	sk_npc_dmg_357			( "sk_npc_dmg_357", "0", FCVAR_REPLICATED );
ConVar	sk_max_357				( "sk_max_357", "0", FCVAR_REPLICATED );

ConVar	sk_plr_dmg_crossbow		( "sk_plr_dmg_crossbow", "0", FCVAR_REPLICATED );
ConVar	sk_npc_dmg_crossbow		( "sk_npc_dmg_crossbow", "0", FCVAR_REPLICATED );
ConVar	sk_max_crossbow			( "sk_max_crossbow", "0", FCVAR_REPLICATED );

ConVar	sk_dmg_sniper_penetrate_plr( "sk_dmg_sniper_penetrate_plr","0", FCVAR_REPLICATED);
ConVar	sk_dmg_sniper_penetrate_npc( "sk_dmg_sniper_penetrate_npc","0", FCVAR_REPLICATED);

ConVar	sk_plr_dmg_airboat		( "sk_plr_dmg_airboat", "0", FCVAR_REPLICATED );
ConVar	sk_npc_dmg_airboat		( "sk_npc_dmg_airboat", "0", FCVAR_REPLICATED );

ConVar	sk_max_gauss_round		( "sk_max_gauss_round", "0", FCVAR_REPLICATED );

// Gunship & Dropship cannons
ConVar	sk_npc_dmg_gunship			( "sk_npc_dmg_gunship", "0", FCVAR_REPLICATED );
ConVar	sk_npc_dmg_gunship_to_plr	( "sk_npc_dmg_gunship_to_plr", "0", FCVAR_REPLICATED );

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iDmgType - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
int CHalfLife2::Damage_GetTimeBased( void )
{
#ifdef HL2_EPISODIC
	int iDamage = ( DMG_PARALYZE | DMG_NERVEGAS | DMG_POISON | DMG_RADIATION | DMG_DROWNRECOVER | DMG_ACID | DMG_SLOWBURN );
	return iDamage;
#else
	return BaseClass::Damage_GetTimeBased();
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iDmgType - 
// Output :		bool
//-----------------------------------------------------------------------------
bool CHalfLife2::Damage_IsTimeBased( int iDmgType )
{
	// Damage types that are time-based.
#ifdef HL2_EPISODIC
	// This makes me think EP2 should have its own rules, but they are #ifdef all over in here.
	return ( ( iDmgType & ( DMG_PARALYZE | DMG_NERVEGAS | DMG_POISON | DMG_RADIATION | DMG_DROWNRECOVER | DMG_SLOWBURN ) ) != 0 );
#else
	return BaseClass::Damage_IsTimeBased( iDmgType );
#endif
}


#ifdef CLIENT_DLL //{


#else //}{

	extern bool		g_fGameOver;

#if !(defined( HL2MP ) || defined( PORTAL_MP ))
	class CVoiceGameMgrHelper : public IVoiceGameMgrHelper
	{
	public:
		virtual bool		CanPlayerHearPlayer( CBasePlayer *pListener, CBasePlayer *pTalker, bool &bProximity )
		{
			return true;
		}
	};
	CVoiceGameMgrHelper g_VoiceGameMgrHelper;
	IVoiceGameMgrHelper *g_pVoiceGameMgrHelper = &g_VoiceGameMgrHelper;
#endif
	
	//-----------------------------------------------------------------------------
	// Purpose:
	// Input  :
	// Output :
	//-----------------------------------------------------------------------------
	CHalfLife2::CHalfLife2()
	{
		m_bMegaPhysgun = false;
		
		m_flLastHealthDropTime = 0.0f;
		m_flLastGrenadeDropTime = 0.0f;
	}

	//-----------------------------------------------------------------------------
	// Purpose: called each time a player uses a "cmd" command
	// Input  : *pEdict - the player who issued the command
	//			Use engine.Cmd_Argv,  engine.Cmd_Argv, and engine.Cmd_Argc to get 
	//			pointers the character string command.
	//-----------------------------------------------------------------------------
	bool CHalfLife2::ClientCommand( CBaseEntity *pEdict, const CCommand &args )
	{
		if( BaseClass::ClientCommand( pEdict, args ) )
			return true;

		CHL2_Player *pPlayer = (CHL2_Player *) pEdict;

		if ( pPlayer->ClientCommand( args ) )
			return true;

		return false;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Player has just spawned. Equip them.
	//-----------------------------------------------------------------------------
	void CHalfLife2::PlayerSpawn( CBasePlayer *pPlayer )
	{
	}

	//-----------------------------------------------------------------------------
	// Purpose: MULTIPLAYER BODY QUE HANDLING
	//-----------------------------------------------------------------------------
	class CCorpse : public CBaseAnimating
	{
	public:
		DECLARE_CLASS( CCorpse, CBaseAnimating );
		DECLARE_SERVERCLASS();

		virtual int ObjectCaps( void ) { return FCAP_DONT_SAVE; }	

	public:
		CNetworkVar( int, m_nReferencePlayer );
	};

	IMPLEMENT_SERVERCLASS_ST(CCorpse, DT_Corpse)
		SendPropInt( SENDINFO(m_nReferencePlayer), 10, SPROP_UNSIGNED )
	END_SEND_TABLE()

	LINK_ENTITY_TO_CLASS( bodyque, CCorpse );


	CCorpse		*g_pBodyQueueHead;

	void InitBodyQue(void)
	{
		CCorpse *pEntity = ( CCorpse * )CreateEntityByName( "bodyque" );
		pEntity->AddEFlags( EFL_KEEP_ON_RECREATE_ENTITIES );
		g_pBodyQueueHead = pEntity;
		CCorpse *p = g_pBodyQueueHead;
		
		// Reserve 3 more slots for dead bodies
		for ( int i = 0; i < 3; i++ )
		{
			CCorpse *next = ( CCorpse * )CreateEntityByName( "bodyque" );
			next->AddEFlags( EFL_KEEP_ON_RECREATE_ENTITIES );
			p->SetOwnerEntity( next );
			p = next;
		}
		
		p->SetOwnerEntity( g_pBodyQueueHead );
	}

	//-----------------------------------------------------------------------------
	// Purpose: make a body que entry for the given ent so the ent can be respawned elsewhere
	// GLOBALS ASSUMED SET:  g_eoBodyQueueHead
	//-----------------------------------------------------------------------------
	void CopyToBodyQue( CBaseAnimating *pCorpse ) 
	{
		if ( pCorpse->IsEffectActive( EF_NODRAW ) )
			return;

		CCorpse *pHead	= g_pBodyQueueHead;

		pHead->CopyAnimationDataFrom( pCorpse );

		pHead->SetMoveType( MOVETYPE_FLYGRAVITY );
		pHead->SetAbsVelocity( pCorpse->GetAbsVelocity() );
		pHead->ClearFlags();
		pHead->m_nReferencePlayer	= ENTINDEX( pCorpse );

		pHead->SetLocalAngles( pCorpse->GetAbsAngles() );
		UTIL_SetOrigin(pHead, pCorpse->GetAbsOrigin());

		UTIL_SetSize(pHead, pCorpse->WorldAlignMins(), pCorpse->WorldAlignMaxs());
		g_pBodyQueueHead = (CCorpse *)pHead->GetOwnerEntity();
	}

	//------------------------------------------------------------------------------
	// Purpose : Return classify text for classify type
	// Input   :
	// Output  :
	//------------------------------------------------------------------------------
	const char* CHalfLife2::AIClassText(int classType)
	{
		switch (classType)
		{
			case CLASS_NONE:			return "CLASS_NONE";
			case CLASS_PLAYER:			return "CLASS_PLAYER";
			case CLASS_ANTLION:			return "CLASS_ANTLION";
			case CLASS_BARNACLE:		return "CLASS_BARNACLE";
			case CLASS_BULLSEYE:		return "CLASS_BULLSEYE";
			// case CLASS_BULLSQUID:		return "CLASS_BULLSQUID";	
			case CLASS_CITIZEN_PASSIVE: return "CLASS_CITIZEN_PASSIVE";		
			case CLASS_CITIZEN_REBEL:	return "CLASS_CITIZEN_REBEL";
			case CLASS_COMBINE:			return "CLASS_COMBINE";
			case CLASS_COMBINE_GUNSHIP:	return "CLASS_COMBINE_GUNSHIP";
			case CLASS_COMBINE_HUNTER:	return "CLASS_COMBINE_HUNTER";
			case CLASS_CONSCRIPT:		return "CLASS_CONSCRIPT";
			case CLASS_HEADCRAB:		return "CLASS_HEADCRAB";
			// case CLASS_HOUNDEYE:		return "CLASS_HOUNDEYE";
			case CLASS_MANHACK:			return "CLASS_MANHACK";
			case CLASS_METROPOLICE:		return "CLASS_METROPOLICE";
			case CLASS_MILITARY:		return "CLASS_MILITARY";	
			case CLASS_SCANNER:			return "CLASS_SCANNER";		
			case CLASS_STALKER:			return "CLASS_STALKER";		
			case CLASS_VORTIGAUNT:		return "CLASS_VORTIGAUNT";
			case CLASS_ZOMBIE:			return "CLASS_ZOMBIE";
			case CLASS_PROTOSNIPER:		return "CLASS_PROTOSNIPER";
			case CLASS_MISSILE:			return "CLASS_MISSILE";
			case CLASS_FLARE:			return "CLASS_FLARE";
			case CLASS_EARTH_FAUNA:		return "CLASS_EARTH_FAUNA";

			default:					return "MISSING CLASS in ClassifyText()";
		}
	}

	void CHalfLife2::PlayerThink( CBasePlayer *pPlayer )
	{
	}

	void CHalfLife2::Think( void )
	{
		BaseClass::Think();

		/*if( physcannon_mega_enabled.GetBool() == true )
		{
			m_bMegaPhysgun = true;
		}
		else*/
		{
			// FIXME: Is there a better place for this?
			m_bMegaPhysgun = ( GlobalEntity_GetState("super_phys_gun") == GLOBAL_ON );
		}
	}

	//-----------------------------------------------------------------------------
	// Purpose: Returns how much damage the given ammo type should do to the victim
	//			when fired by the attacker.
	// Input  : pAttacker - Dude what shot the gun.
	//			pVictim - Dude what done got shot.
	//			nAmmoType - What been shot out.
	// Output : How much hurt to put on dude what done got shot (pVictim).
	//-----------------------------------------------------------------------------
	float CHalfLife2::GetAmmoDamage( CBaseEntity *pAttacker, CBaseEntity *pVictim, int nAmmoType )
	{
		float flDamage = 0.0f;
		CAmmoDef *pAmmoDef = GetAmmoDef();

		if ( pAmmoDef->DamageType( nAmmoType ) & DMG_SNIPER )
		{
			// If this damage is from a SNIPER, we do damage based on what the bullet
			// HITS, not who fired it. All other bullets have their damage values
			// arranged according to the owner of the bullet, not the recipient.
			if ( pVictim->IsPlayer() )
			{
				// Player
				flDamage = pAmmoDef->PlrDamage( nAmmoType );
			}
			else
			{
				// NPC or breakable
				flDamage = pAmmoDef->NPCDamage( nAmmoType );
			}
		}
		else
		{
			flDamage = BaseClass::GetAmmoDamage( pAttacker, pVictim, nAmmoType );
		}

		if( pAttacker->IsPlayer() && pVictim->IsNPC() )
		{
			if( pVictim->MyCombatCharacterPointer() )
			{
				// Player is shooting an NPC. Adjust the damage! This protects breakables
				// and other 'non-living' entities from being easier/harder to break
				// in different skill levels.
				flDamage = pAmmoDef->PlrDamage( nAmmoType );
				flDamage = AdjustPlayerDamageInflicted( flDamage );
			}
		}

		return flDamage;
	}

   	//-----------------------------------------------------------------------------
  	//-----------------------------------------------------------------------------
 	bool CHalfLife2::AllowDamage( CBaseEntity *pVictim, const CTakeDamageInfo &info )
  	{
#ifndef CLIENT_DLL
	if( (info.GetDamageType() & DMG_CRUSH) && info.GetInflictor() && pVictim->MyNPCPointer() )
	{
		if( pVictim->MyNPCPointer()->IsPlayerAlly() )
		{
			// A physics object has struck a player ally. Don't allow damage if it
			// came from the player's physcannon. 
			CBasePlayer *pPlayer = UTIL_PlayerByIndex(1);

			if( pPlayer )
			{
				CBaseEntity *pWeapon = pPlayer->HasNamedPlayerItem("weapon_physcannon");

				if( pWeapon )
				{
					CBaseCombatWeapon *pCannon = assert_cast <CBaseCombatWeapon*>(pWeapon);

					if( pCannon )
					{
						if( PhysCannonAccountableForObject(pCannon, info.GetInflictor() ) )
						{
							// Antlions can always be squashed!
							if ( pVictim->Classify() == CLASS_ANTLION )
								return true;

  							return false;
						}
					}
				}
			}
		}
	}
#endif
  		return true;
  	}
	//-----------------------------------------------------------------------------
	// Purpose: Whether or not the NPC should drop a health vial
	// Output : Returns true on success, false on failure.
	//-----------------------------------------------------------------------------
	bool CHalfLife2::NPC_ShouldDropHealth( CBasePlayer *pRecipient )
	{
		// Can only do this every so often
		if ( m_flLastHealthDropTime > gpGlobals->curtime )
			return false;

		//Try to throw dynamic health
		float healthPerc = ( (float) pRecipient->m_iHealth / (float) pRecipient->m_iMaxHealth );

		if ( random->RandomFloat( 0.0f, 1.0f ) > healthPerc*1.5f )
			return true;

		return false;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Whether or not the NPC should drop grenades
	// Output : Returns true on success, false on failure.
	//-----------------------------------------------------------------------------
	bool CHalfLife2::NPC_ShouldDropGrenade( CBasePlayer *pRecipient )
	{
		// Can only do this every so often
		if ( m_flLastGrenadeDropTime > gpGlobals->curtime )
			return false;
		
		int grenadeIndex = GetAmmoDef()->Index( "grenade" );
		int numGrenades = pRecipient->GetAmmoCount( grenadeIndex );

		// If we're not maxed out on grenades and we've randomly okay'd it
		if ( ( numGrenades < GetAmmoDef()->AmmoMaxCarry( grenadeIndex, pRecipient ) ) && ( random->RandomInt( 0, 2 ) == 0 ) )
			return true;

		return false;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Update the drop counter for health
	//-----------------------------------------------------------------------------
	void CHalfLife2::NPC_DroppedHealth( void )
	{
		m_flLastHealthDropTime = gpGlobals->curtime + sk_plr_health_drop_time.GetFloat();
	}

	//-----------------------------------------------------------------------------
	// Purpose: Update the drop counter for grenades
	//-----------------------------------------------------------------------------
	void CHalfLife2::NPC_DroppedGrenade( void )
	{
		m_flLastGrenadeDropTime = gpGlobals->curtime + sk_plr_grenade_drop_time.GetFloat();
	}

#endif //} !CLIENT_DLL


// ------------------------------------------------------------------------------------ //
// Shared CHalfLife2 implementation.
// ------------------------------------------------------------------------------------ //
bool CHalfLife2::ShouldCollide( int collisionGroup0, int collisionGroup1 )
{
	// The smaller number is always first
	if ( collisionGroup0 > collisionGroup1 )
	{
		// swap so that lowest is always first
		int tmp = collisionGroup0;
		collisionGroup0 = collisionGroup1;
		collisionGroup1 = tmp;
	}
	
	// Prevent the player movement from colliding with spit globs (caused the player to jump on top of globs while in water)
	if ( collisionGroup0 == COLLISION_GROUP_PLAYER_MOVEMENT && collisionGroup1 == HL2COLLISION_GROUP_SPIT )
		return false;

	// HL2 treats movement and tracing against players the same, so just remap here
	if ( collisionGroup0 == COLLISION_GROUP_PLAYER_MOVEMENT )
	{
		collisionGroup0 = COLLISION_GROUP_PLAYER;
	}

	if( collisionGroup1 == COLLISION_GROUP_PLAYER_MOVEMENT )
	{
		collisionGroup1 = COLLISION_GROUP_PLAYER;
	}

	//If collisionGroup0 is not a player then NPC_ACTOR behaves just like an NPC.
	if ( collisionGroup1 == COLLISION_GROUP_NPC_ACTOR && collisionGroup0 != COLLISION_GROUP_PLAYER )
	{
		collisionGroup1 = COLLISION_GROUP_NPC;
	}

	// This is only for the super physcannon
	if ( m_bMegaPhysgun )
	{
		if ( collisionGroup0 == COLLISION_GROUP_INTERACTIVE_DEBRIS && collisionGroup1 == COLLISION_GROUP_PLAYER )
			return false;
	}

	if ( collisionGroup0 == HL2COLLISION_GROUP_COMBINE_BALL )
	{
		if ( collisionGroup1 == HL2COLLISION_GROUP_COMBINE_BALL )
			return false;
	}

	if ( collisionGroup0 == HL2COLLISION_GROUP_COMBINE_BALL && collisionGroup1 == HL2COLLISION_GROUP_COMBINE_BALL_NPC )
		return false;

	if ( ( collisionGroup0 == COLLISION_GROUP_WEAPON ) ||
		( collisionGroup0 == COLLISION_GROUP_PLAYER ) ||
		( collisionGroup0 == COLLISION_GROUP_PROJECTILE ) )
	{
		if ( collisionGroup1 == HL2COLLISION_GROUP_COMBINE_BALL )
			return false;
	}

	if ( collisionGroup0 == COLLISION_GROUP_DEBRIS )
	{
		if ( collisionGroup1 == HL2COLLISION_GROUP_COMBINE_BALL )
			return true;
	}

	if (collisionGroup0 == HL2COLLISION_GROUP_HOUNDEYE && collisionGroup1 == HL2COLLISION_GROUP_HOUNDEYE )
		return false;

	if (collisionGroup0 == HL2COLLISION_GROUP_HOMING_MISSILE && collisionGroup1 == HL2COLLISION_GROUP_HOMING_MISSILE )
		return false;

	if ( collisionGroup1 == HL2COLLISION_GROUP_CROW )
	{
		if ( collisionGroup0 == COLLISION_GROUP_PLAYER || collisionGroup0 == COLLISION_GROUP_NPC ||
			 collisionGroup0 == HL2COLLISION_GROUP_CROW )
			return false;
	}

	if ( ( collisionGroup0 == HL2COLLISION_GROUP_HEADCRAB ) && ( collisionGroup1 == HL2COLLISION_GROUP_HEADCRAB ) )
		return false;

	// striders don't collide with other striders
	if ( collisionGroup0 == HL2COLLISION_GROUP_STRIDER && collisionGroup1 == HL2COLLISION_GROUP_STRIDER )
		return false;

	// gunships don't collide with other gunships
	if ( collisionGroup0 == HL2COLLISION_GROUP_GUNSHIP && collisionGroup1 == HL2COLLISION_GROUP_GUNSHIP )
		return false;

	// weapons and NPCs don't collide
	if ( collisionGroup0 == COLLISION_GROUP_WEAPON && (collisionGroup1 >= HL2COLLISION_GROUP_FIRST_NPC && collisionGroup1 <= HL2COLLISION_GROUP_LAST_NPC ) )
		return false;

	//players don't collide against NPC Actors.
	//I could've done this up where I check if collisionGroup0 is NOT a player but I decided to just
	//do what the other checks are doing in this function for consistency sake.
	if ( collisionGroup1 == COLLISION_GROUP_NPC_ACTOR && collisionGroup0 == COLLISION_GROUP_PLAYER )
		return false;
		
	// In cases where NPCs are playing a script which causes them to interpenetrate while riding on another entity,
	// such as a train or elevator, you need to disable collisions between the actors so the mover can move them.
	if ( collisionGroup0 == COLLISION_GROUP_NPC_SCRIPTED && collisionGroup1 == COLLISION_GROUP_NPC_SCRIPTED )
		return false;

	// Spit doesn't touch other spit
	if ( collisionGroup0 == HL2COLLISION_GROUP_SPIT && collisionGroup1 == HL2COLLISION_GROUP_SPIT )
		return false;

	return BaseClass::ShouldCollide( collisionGroup0, collisionGroup1 ); 
}

#ifndef CLIENT_DLL
//---------------------------------------------------------
//---------------------------------------------------------
void CHalfLife2::AdjustPlayerDamageTaken( CTakeDamageInfo *pInfo )
{
	if( pInfo->GetDamageType() & (DMG_DROWN|DMG_CRUSH|DMG_FALL|DMG_POISON|DMG_CLUB) )
	{
		// Skill level doesn't affect these types of damage.
		return;
	}

	switch( GetSkillLevel() )
	{
	case SKILL_EASY:
		pInfo->ScaleDamage( sk_dmg_take_scale1.GetFloat() );
		break;

	case SKILL_MEDIUM:
		pInfo->ScaleDamage( sk_dmg_take_scale2.GetFloat() );
		break;

	case SKILL_HARD:
		pInfo->ScaleDamage( sk_dmg_take_scale3.GetFloat() );
		break;
	}
}

//---------------------------------------------------------
//---------------------------------------------------------
float CHalfLife2::AdjustPlayerDamageInflicted( float damage )
{
	switch( GetSkillLevel() ) 
	{
	case SKILL_EASY:
		return damage * sk_dmg_inflict_scale1.GetFloat();
		break;

	case SKILL_MEDIUM:
		return damage * sk_dmg_inflict_scale2.GetFloat();
		break;

	case SKILL_HARD:
		return damage * sk_dmg_inflict_scale3.GetFloat();
		break;

	default:
		return damage;
		break;
	}
}
#endif//CLIENT_DLL

//---------------------------------------------------------
//---------------------------------------------------------
bool CHalfLife2::ShouldUseRobustRadiusDamage(CBaseEntity *pEntity)
{
#ifdef CLIENT_DLL
	return false;
#endif

	if( !sv_robust_explosions.GetBool() )
		return false;

	if( !pEntity->IsNPC() )
	{
		// Only NPC's
		return false;
	}

#ifndef CLIENT_DLL
	CAI_BaseNPC *pNPC = pEntity->MyNPCPointer();
	if( pNPC->CapabilitiesGet() & bits_CAP_SIMPLE_RADIUS_DAMAGE )
	{
		// This NPC only eligible for simple radius damage.
		return false;
	}
#endif//CLIENT_DLL

	return true;
}

#ifndef CLIENT_DLL
//---------------------------------------------------------
//---------------------------------------------------------
bool CHalfLife2::ShouldAutoAim( CBasePlayer *pPlayer, edict_t *target )
{
	return sk_allow_autoaim.GetBool() != 0;
}

//---------------------------------------------------------
//---------------------------------------------------------
float CHalfLife2::GetAutoAimScale( CBasePlayer *pPlayer )
{
#ifdef _X360
	return 1.0f;
#else
	switch( GetSkillLevel() )
	{
	case SKILL_EASY:
		return sk_autoaim_scale1.GetFloat();

	case SKILL_MEDIUM:
		return sk_autoaim_scale2.GetFloat();

	case SKILL_HARD:
		return sk_autoaim_scale3.GetFloat();

	default:
		return 0.0f;
	}
#endif
}

//---------------------------------------------------------
//---------------------------------------------------------
float CHalfLife2::GetAmmoQuantityScale( int iAmmoIndex )
{
	switch( GetSkillLevel() )
	{
	case SKILL_EASY:
		return sk_ammo_qty_scale1.GetFloat();

	case SKILL_MEDIUM:
		return sk_ammo_qty_scale2.GetFloat();

	case SKILL_HARD:
		return sk_ammo_qty_scale3.GetFloat();

	default:
		return 0.0f;
	}
}

void CHalfLife2::LevelInitPreEntity()
{
	// Remove this if you fix the bug in ep1 where the striders need to touch
	// triggers using their absbox instead of their bbox
#ifdef HL2_EPISODIC
	if ( !Q_strnicmp( gpGlobals->mapname.ToCStr(), "ep1_", 4 ) )
	{
		// episode 1 maps use the surrounding box trigger behavior
		CBaseEntity::sm_bAccurateTriggerBboxChecks = false;
	}
#endif
	BaseClass::LevelInitPreEntity();
}

//-----------------------------------------------------------------------------
// Returns whether or not Alyx cares about light levels in order to see.
//-----------------------------------------------------------------------------
bool CHalfLife2::IsAlyxInDarknessMode()
{
#ifdef HL2_EPISODIC
	return ( GlobalEntity_GetState( "ep_alyx_darknessmode" ) == GLOBAL_ON );
#else
	return false;
#endif // HL2_EPISODIC
}


//-----------------------------------------------------------------------------
// This takes the long way around to see if a prop should emit a DLIGHT when it
// ignites, to avoid having Alyx-related code in props.cpp.
//-----------------------------------------------------------------------------
bool CHalfLife2::ShouldBurningPropsEmitLight()
{
#ifdef HL2_EPISODIC
	return IsAlyxInDarknessMode();
#else
	return false;
#endif // HL2_EPISODIC
}


#endif//CLIENT_DLL

// ------------------------------------------------------------------------------------ //
// Global functions.
// ------------------------------------------------------------------------------------ //


