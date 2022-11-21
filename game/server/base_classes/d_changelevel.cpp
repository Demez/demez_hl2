#include "cbase.h"
#include "d_changelevel.h"
#include "ai_basenpc.h"
#include "ai_behavior_lead.h"
#include "saverestoretypes.h"
// #include "gameinterface.h"
#include "eiface.h"
#include "saverestore.h"

#include "hl2_player.h"
#include "hl2mp_gamerules.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define DEBUG_TRANSITIONS_VERBOSE	2
ConVar g_debug_transitions( "g_debug_transitions", "0", FCVAR_NONE, "Set to 1 and restart the map to be warned if the map has no trigger_transition volumes. Set to 2 to see a dump of all entities & associated results during a transition." );


enum
{
	TRANSITION_VOLUME_SCREENED_OUT = 0,
	TRANSITION_VOLUME_NOT_FOUND = 1,
	TRANSITION_VOLUME_PASSED = 2,
};

struct collidelist_t
{
	const CPhysCollide	*pCollide;
	Vector			origin;
	QAngle			angles;
};


// NOTE: This routine is relatively slow.  If you need to use it for per-frame work, consider that fact.
// UNDONE: Expand this to the full matrix of solid types on each side and move into enginetrace
static bool TestEntityTriggerIntersection_Accurate( CBaseEntity *pTrigger, CBaseEntity *pEntity )
{
	Assert( pTrigger->GetSolid() == SOLID_BSP );

	if ( pTrigger->Intersects( pEntity ) )	// It touches one, it's in the volume
	{
		switch ( pEntity->GetSolid() )
		{
		case SOLID_BBOX:
		{
			ICollideable *pCollide = pTrigger->CollisionProp();
			Ray_t ray;
			trace_t tr;
			ray.Init( pEntity->GetAbsOrigin(), pEntity->GetAbsOrigin(), pEntity->WorldAlignMins(), pEntity->WorldAlignMaxs() );
			enginetrace->ClipRayToCollideable( ray, MASK_ALL, pCollide, &tr );

			if ( tr.startsolid )
				return true;
		}
		break;
		case SOLID_BSP:
		case SOLID_VPHYSICS:
		{
			CPhysCollide *pTriggerCollide = modelinfo->GetVCollide( pTrigger->GetModelIndex() )->solids[0];
			Assert( pTriggerCollide );

			CUtlVector<collidelist_t> collideList;
			IPhysicsObject *pList[VPHYSICS_MAX_OBJECT_LIST_COUNT];
			int physicsCount = pEntity->VPhysicsGetObjectList( pList, ARRAYSIZE(pList) );
			if ( physicsCount )
			{
				for ( int i = 0; i < physicsCount; i++ )
				{
					const CPhysCollide *pCollide = pList[i]->GetCollide();
					if ( pCollide )
					{
						collidelist_t element;
						element.pCollide = pCollide;
						pList[i]->GetPosition( &element.origin, &element.angles );
						collideList.AddToTail( element );
					}
				}
			}
			else
			{
				vcollide_t *pVCollide = modelinfo->GetVCollide( pEntity->GetModelIndex() );
				if ( pVCollide && pVCollide->solidCount )
				{
					collidelist_t element;
					element.pCollide = pVCollide->solids[0];
					element.origin = pEntity->GetAbsOrigin();
					element.angles = pEntity->GetAbsAngles();
					collideList.AddToTail( element );
				}
			}
			for ( int i = collideList.Count()-1; i >= 0; --i )
			{
				const collidelist_t &element = collideList[i];
				trace_t tr;
				physcollision->TraceCollide( element.origin, element.origin, element.pCollide, element.angles, pTriggerCollide, pTrigger->GetAbsOrigin(), pTrigger->GetAbsAngles(), &tr );
				if ( tr.startsolid )
					return true;
			}
		}
		break;

		default:
			return true;
		}
	}
	return false;
}


//------------------------------------------------------------------------------
// Tests bits in a bitfield
//------------------------------------------------------------------------------
static inline bool IsBitSet( char *pBuf, int nBit )
{
	return (pBuf[ nBit >> 3 ] & ( 1 << (nBit & 0x7) )) != 0;
}

static inline void Set( char *pBuf, int nBit )
{
	pBuf[ nBit >> 3 ] |= 1 << (nBit & 0x7);
}


LINK_ENTITY_TO_CLASS( trigger_changelevel, CChangeLevel );

// Global Savedata for changelevel trigger
BEGIN_DATADESC( CChangeLevel )

	DEFINE_AUTO_ARRAY( m_szMapName, FIELD_CHARACTER ),
	DEFINE_AUTO_ARRAY( m_szLandmarkName, FIELD_CHARACTER ),
//	DEFINE_FIELD( m_touchTime, FIELD_TIME ),	// don't save
//	DEFINE_FIELD( m_bTouched, FIELD_BOOLEAN ),

	// Function Pointers
	DEFINE_FUNCTION( TouchChangeLevel ),

	DEFINE_INPUTFUNC( FIELD_VOID, "ChangeLevel", InputChangeLevel ),

	// Outputs
	DEFINE_OUTPUT( m_OnChangeLevel, "OnChangeLevel"),

END_DATADESC()


//
// Cache user-entity-field values until spawn is called.
//

bool CChangeLevel::KeyValue( const char *szKeyName, const char *szValue )
{
	if (FStrEq(szKeyName, "map"))
	{
		if (strlen(szValue) >= cchMapNameMost)
		{
			Warning( "Map name '%s' too long (32 chars)\n", szValue );
			Assert(0);
		}
		Q_strncpy(m_szMapName, szValue, sizeof(m_szMapName));
	}
	else if (FStrEq(szKeyName, "landmark"))
	{
		if (strlen(szValue) >= cchMapNameMost)
		{
			Warning( "Landmark name '%s' too long (32 chars)\n", szValue );
			Assert(0);
		}
		
		Q_strncpy(m_szLandmarkName, szValue, sizeof( m_szLandmarkName ));
	}
	else
		return BaseClass::KeyValue( szKeyName, szValue );

	return true;
}



void CChangeLevel::Spawn( void )
{
	if ( FStrEq( m_szMapName, "" ) )
	{
		Msg( "a trigger_changelevel doesn't have a map" );
	}

	if ( FStrEq( m_szLandmarkName, "" ) )
	{
		Msg( "trigger_changelevel to %s doesn't have a landmark", m_szMapName );
	}

	InitTrigger();
	
	if ( !HasSpawnFlags(SF_CHANGELEVEL_NOTOUCH) )
	{
		SetTouch( &CChangeLevel::TouchChangeLevel );
	}

	// look for a landmark entity		
	FindLandmark();

	m_iTransitionPlayers = 0;

//	Msg( "TRANSITION: %s (%s)\n", m_szMapName, m_szLandmarkName );
}

void CChangeLevel::Activate( void )
{
	BaseClass::Activate();

	if ( gpGlobals->eLoadType == MapLoad_NewGame )
	{
		if ( HasSpawnFlags( SF_CHANGELEVEL_CHAPTER ) )
		{
			VPhysicsInitStatic();
			RemoveSolidFlags( FSOLID_NOT_SOLID | FSOLID_TRIGGER );
			SetTouch( NULL );
			return;
		}
	}

	// Level transitions will bust if they are in solid
	FindLandmark();
	if ( m_pLandmark )
	{
		int clusterIndex = engine->GetClusterForOrigin( m_pLandmark->GetAbsOrigin() );
		if ( clusterIndex < 0 )
		{
			Warning( "trigger_changelevel to map %s has a landmark embedded in solid!\n"
					"This will break level transitions!\n", m_szMapName ); 
		}

		if ( g_debug_transitions.GetInt() )
		{
			if ( !gEntList.FindEntityByClassname( NULL, "trigger_transition" ) )
			{
				Warning( "Map has no trigger_transition volumes for landmark %s\n", m_szLandmarkName );
			}
		}
	}

	m_bTouched = false;
}


static char st_szNextMap[cchMapNameMost];
static char st_szNextSpot[cchMapNameMost];

// Used to show debug for only the transition volume we're currently in
static int g_iDebuggingTransition = 0;

CBaseEntity *CChangeLevel::FindLandmark( const char *pLandmarkName )
{
	CBaseEntity *pentLandmark;

	pentLandmark = gEntList.FindEntityByName( NULL, pLandmarkName );
	while ( pentLandmark )
	{
		// Found the landmark
		if ( FClassnameIs( pentLandmark, "info_landmark" ) )
			return pentLandmark;
		else
			pentLandmark = gEntList.FindEntityByName( pentLandmark, pLandmarkName );
	}
	Warning( "Can't find landmark %s\n", pLandmarkName );
	return NULL;
}


void CChangeLevel::FindLandmark()
{
	if ( m_pLandmark == NULL )
	{
		m_pLandmark = FindLandmark( m_szLandmarkName );
	}
}


//-----------------------------------------------------------------------------
// Purpose: Allows level transitions to be triggered by buttons, etc.
//-----------------------------------------------------------------------------
void CChangeLevel::InputChangeLevel( inputdata_t &inputdata )
{
	// Ignore changelevel transitions if the player's dead or attempting a challenge
	if ( gpGlobals->maxClients == 1 )
	{
		CBasePlayer *pPlayer = UTIL_GetLocalPlayer();
		if ( pPlayer && ( !pPlayer->IsAlive() || pPlayer->GetBonusChallenge() > 0 ) )
			return;
	}

	ChangeLevelNow( inputdata.pActivator );
}


//-----------------------------------------------------------------------------
// Purpose: Performs the level change and fires targets.
// Input  : pActivator - 
//-----------------------------------------------------------------------------
bool CChangeLevel::IsEntityInTransition( CBaseEntity *pEntity )
{
	int transitionState = InTransitionVolume(pEntity, m_szLandmarkName);
	if ( transitionState == TRANSITION_VOLUME_SCREENED_OUT )
	{
		return false;
	}

	if ( !m_pLandmark )
		return false;

	// Check to make sure it's also in the PVS of landmark
	byte pvs[MAX_MAP_CLUSTERS/8];
	int clusterIndex = engine->GetClusterForOrigin( m_pLandmark->GetAbsOrigin() );
	engine->GetPVSForCluster( clusterIndex, sizeof(pvs), pvs );
	Vector vecSurroundMins, vecSurroundMaxs;
	pEntity->CollisionProp()->WorldSpaceSurroundingBounds( &vecSurroundMins, &vecSurroundMaxs );

	return engine->CheckBoxInPVS( vecSurroundMins, vecSurroundMaxs, pvs, sizeof( pvs ) );
}

void CChangeLevel::NotifyEntitiesOutOfTransition()
{
	CBaseEntity *pEnt = gEntList.FirstEnt();
	while ( pEnt )
	{
		// Found the landmark
		if ( pEnt->ObjectCaps() & FCAP_NOTIFY_ON_TRANSITION )
		{
			variant_t emptyVariant;
			if ( !(pEnt->ObjectCaps() & (FCAP_ACROSS_TRANSITION|FCAP_FORCE_TRANSITION)) || !IsEntityInTransition( pEnt ) )
			{
				pEnt->AcceptInput( "OutsideTransition", this, this, emptyVariant, 0 );
			}
			else
			{
				pEnt->AcceptInput( "InsideTransition", this, this, emptyVariant, 0 );
			}
		}
		pEnt = gEntList.NextEnt( pEnt );
	}
}

//------------------------------------------------------------------------------
// Purpose : Checks all spawned AIs and prints a warning if any are actively leading
// Input   :
// Output  :
//------------------------------------------------------------------------------
void CChangeLevel::WarnAboutActiveLead( void )
{
	int					i;
	CAI_BaseNPC *		ai;
	CAI_BehaviorBase *	behavior;

	for ( i = 0; i < g_AI_Manager.NumAIs(); i++ )
	{
		ai = g_AI_Manager.AccessAIs()[i];
		behavior = ai->GetPrimaryBehavior();
		if ( behavior )
		{
			if ( dynamic_cast<CAI_LeadBehavior *>( behavior ) )
			{
				Warning( "Entity '%s' is still actively leading\n", STRING( ai->GetEntityName() ) );
			} 
		}
	}
}


ConVar mp_transition_players_percent(
	"mp_transition_players_percent",
	"66",
	FCVAR_NOTIFY | FCVAR_REPLICATED,
	"How many players in percent are needed for a level transition?"
);

ConVar d_sv_use_changelevel2("d_sv_use_changelevel2", "0");


extern CSaveRestoreData *SaveInit( int size );

void CChangeLevel::ChangeLevelNow( CBaseEntity *pActivator, bool force )
{
	CBaseEntity	*pLandmark;
	levellist_t	levels[16];

	Assert(!FStrEq(m_szMapName, ""));

	if ( !ShouldChangeLevel( pActivator, force ) )
		return;

	if ( !m_pLandmark )
		return;

	// We have enough players for a transition

	WarnAboutActiveLead();

	g_iDebuggingTransition = 0;
	st_szNextSpot[0] = 0;	// Init landmark to NULL
	Q_strncpy(st_szNextSpot, m_szLandmarkName,sizeof(st_szNextSpot));
	// This object will get removed in the call to engine->ChangeLevel, copy the params into "safe" memory
	Q_strncpy(st_szNextMap, m_szMapName, sizeof(st_szNextMap));

	m_hActivator = pActivator;

	m_OnChangeLevel.FireOutput(pActivator, this);

	NotifyEntitiesOutOfTransition();

	CBasePlayer* pTeleportPlayer = NULL;

	UTIL_FOREACHPLAYER(i)
	{
		CHL2MP_Player* hl2Player = (CHL2MP_Player*)UTIL_PlayerByIndex(i);

		if (hl2Player && hl2Player->IsAlive() && hl2Player->m_bInTransition)
		{
			pTeleportPlayer = hl2Player;
			break;
		}
	}

	if ( pTeleportPlayer )
	{
		UTIL_FOREACHPLAYER(i)
		{
			CHL2MP_Player* pPlayer = (CHL2MP_Player*)UTIL_PlayerByIndex(i);

			if (pPlayer && pPlayer->IsAlive() && !pPlayer->m_bInTransition && pPlayer != pTeleportPlayer)
			{
				pPlayer->Teleport( &pTeleportPlayer->GetAbsOrigin(), &pTeleportPlayer->GetAbsAngles(), 0 );
				pPlayer->SetAbsOrigin( pTeleportPlayer->GetAbsOrigin() );
			}
		}
	}

////	Msg( "Level touches %d levels\n", ChangeList( levels, 16 ) );
	if ( g_debug_transitions.GetInt() )
	{
		Msg( "CHANGE LEVEL: %s %s\n", st_szNextMap, st_szNextSpot );
	}

	// If we're debugging, don't actually change level
	if ( g_debug_transitions.GetInt() == 0 )
	{
		// DemezGameRules()->OnBeginChangeLevel( st_szNextMap, NULL );

		if ( d_sv_use_changelevel2.GetBool() )
		{
			engine->ChangeLevel( st_szNextMap, st_szNextSpot );
		}
		else
		{
			engine->ChangeLevel( st_szNextMap, NULL );
		}
	}
	else
	{
		// Build a change list so we can see what would be transitioning
		CSaveRestoreData *pSaveData = SaveInit( 0 );
		if ( pSaveData )
		{
			g_pGameSaveRestoreBlockSet->PreSave( pSaveData );
			pSaveData->levelInfo.connectionCount = BuildChangeList( pSaveData->levelInfo.levelList, MAX_LEVEL_CONNECTIONS );
			g_pGameSaveRestoreBlockSet->PostSave();
		}

		SetTouch( NULL );
	}
}



bool CChangeLevel::ShouldChangeLevelOld( CBaseEntity *pActivator )
{
	FindLandmark();

	// Don't work in deathmatch
	// if ( g_pGameRules->IsDeathmatch() )
	// 	return false;

	if ( mp_transition_players_percent.GetInt() > 0 )
	{
		int totalPlayers = 0;
		int transitionPlayers = 0;

		// maybe freeze the player and unfreeze when a transition starts?
		for (int i = 1; i <= gpGlobals->maxClients; i++)
		{
			CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);
			if (pPlayer && pPlayer->IsAlive())
			{
				totalPlayers++;

				int transitionState = InTransitionVolume( pPlayer, m_szLandmarkName );

				if ( transitionState == TRANSITION_VOLUME_PASSED )
				{
					transitionPlayers++;
					Msg("Transitions: player in transition: %s\n", pPlayer->GetPlayerName());
				}
				// no transition volumes, check PVS of landmark
				else if ( transitionState == TRANSITION_VOLUME_NOT_FOUND )
				{
					byte pvs[MAX_MAP_CLUSTERS/8];
					int clusterIndex = engine->GetClusterForOrigin( m_pLandmark->GetAbsOrigin() );
					engine->GetPVSForCluster( clusterIndex, sizeof(pvs), pvs );

					Vector vecSurroundMins, vecSurroundMaxs;
					pPlayer->CollisionProp()->WorldSpaceSurroundingBounds( &vecSurroundMins, &vecSurroundMaxs );
					bool playerInPVS = engine->CheckBoxInPVS( vecSurroundMins, vecSurroundMaxs, pvs, sizeof( pvs ) );

					if ( playerInPVS )
					{
						transitionPlayers++;
						Msg("Transitions: player in transition: %s\n", pPlayer->GetPlayerName());
					}
				}
			}
		}

		Msg("Transitions: transition status - %d/%d players - %d%%\n", transitionPlayers, totalPlayers, (int) (transitionPlayers / totalPlayers * 100));

		if (((int) (transitionPlayers / totalPlayers * 100)) > mp_transition_players_percent.GetInt())
		{
			// Msg("Transitions: Not enough players to trigger level change - %d/%d players\n", transitionPlayers, totalPlayers);
			Msg("Transitions: Triggering level change - %d/%d players - %d%%\n", transitionPlayers, totalPlayers, (int) (transitionPlayers / totalPlayers * 100));
			return false;
		}
	}

	return true;
}

bool CChangeLevel::ShouldChangeLevel( CBaseEntity *pActivator, bool force )
{
	FindLandmark();

	// Don't work in deathmatch
	// if ( g_pGameRules->IsDeathmatch() )
	// 	return false;

	if ( gpGlobals->maxClients == 1 )
		return true;

	if ( force && gpGlobals->maxClients > 1 )
	{
		UTIL_FOREACHPLAYER(i)
		{
			UTIL_GETNEXTPLAYER(i);
			pPlayer->UnlockPlayer();
		}

		return true;
	}

	if ( mp_transition_players_percent.GetInt() > 0 )
	{
		if ( pActivator && pActivator->IsPlayer() )
		{
			CHL2MP_Player* hl2Player = (CHL2MP_Player*)pActivator;

			if ( !hl2Player->m_bInTransition )
			{
				hl2Player->m_bInTransition = true;
				m_iTransitionPlayers++;
			}
			else
			{
				// continuous touching of trigger
				return false;
			}

			if ( gpGlobals->maxClients > 1 )
				((CBasePlayer*)pActivator)->LockPlayerInPlace();
		}

		int totalPlayers = 0;

		// maybe freeze the player and unfreeze when a transition starts?
		UTIL_FOREACHPLAYER(i)
		{
			CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);

			if (pPlayer && pPlayer->IsAlive())
			{
				totalPlayers++;
			}
		}

		int percentage = (int)(( (float)m_iTransitionPlayers / (float)totalPlayers ) * 100);

		Msg("Transitions: transition status - %d/%d players - %d%%\n", m_iTransitionPlayers, totalPlayers, percentage);

		if (percentage > mp_transition_players_percent.GetInt())
		{
			Msg("Transitions: Triggering level change - %d/%d players - %d%%\n", m_iTransitionPlayers, totalPlayers, percentage);
			DemezGameRules()->StartTransitionTimer( this );
			DemezGameRules()->EndTransitionTimer();
			if ( gpGlobals->maxClients > 1 )
			{
				UTIL_FOREACHPLAYER(i)
				{
					UTIL_GETNEXTPLAYER(i);
					pPlayer->UnlockPlayer();
				}
			}
			return true;
		}
		else
		{
			Msg("Transitions: Not enough players to trigger level change - %d/%d players - %d%%\n", m_iTransitionPlayers, totalPlayers, percentage);
			DemezGameRules()->StartTransitionTimer( this );
			return false;
		}
	}

	DemezGameRules()->EndTransitionTimer();
	return true;
}


//
// GLOBALS ASSUMED SET:  st_szNextMap
//
void CChangeLevel::TouchChangeLevel( CBaseEntity *pOther )
{
	CBasePlayer *pPlayer = ToBasePlayer(pOther);
	if ( !pPlayer )
		return;

	if( pPlayer->IsSinglePlayerGameEnding() )
	{
		// Some semblance of deceleration, but allow player to fall normally.
		// Also, disable controls.
		Vector vecVelocity = pPlayer->GetAbsVelocity();
		vecVelocity.x *= 0.5f;
		vecVelocity.y *= 0.5f;
		pPlayer->SetAbsVelocity( vecVelocity );
		pPlayer->AddFlag( FL_FROZEN );
		return;
	}

	if ( !pPlayer->IsInAVehicle() && pPlayer->GetMoveType() == MOVETYPE_NOCLIP )
	{
		DevMsg("In level transition: %s %s\n", st_szNextMap, st_szNextSpot );
		return;
	}

	ChangeLevelNow( pOther );
}


// Add a transition to the list, but ignore duplicates 
// (a designer may have placed multiple trigger_changelevels with the same landmark)
int CChangeLevel::AddTransitionToList( levellist_t *pLevelList, int listCount, const char *pMapName, const char *pLandmarkName, edict_t *pentLandmark )
{
	int i;

	if ( !pLevelList || !pMapName || !pLandmarkName || !pentLandmark )
		return 0;

	// Ignore changelevels to the level we're ready in. Mapmakers love to do this!
	if ( stricmp( pMapName, gpGlobals->mapname.ToCStr() ) == 0 )
		return 0;

	for ( i = 0; i < listCount; i++ )
	{
		if ( pLevelList[i].pentLandmark == pentLandmark && stricmp( pLevelList[i].mapName, pMapName ) == 0 )
			return 0;
	}
	Q_strncpy( pLevelList[listCount].mapName, pMapName, sizeof(pLevelList[listCount].mapName) );
	Q_strncpy( pLevelList[listCount].landmarkName, pLandmarkName, sizeof(pLevelList[listCount].landmarkName) );
	pLevelList[listCount].pentLandmark = pentLandmark;

	CBaseEntity *ent = CBaseEntity::Instance( pentLandmark );
	Assert( ent );

	pLevelList[listCount].vecLandmarkOrigin = ent->GetAbsOrigin();

	return 1;
}

int BuildChangeList( levellist_t *pLevelList, int maxList )
{
	return CChangeLevel::ChangeList( pLevelList, maxList );
}


int CChangeLevel::InTransitionVolume( CBaseEntity *pEntity, const char *pVolumeName )
{
	CBaseEntity *pVolume;

	if ( pEntity->ObjectCaps() & FCAP_FORCE_TRANSITION )
		return TRANSITION_VOLUME_PASSED;

	// If you're following another entity, follow it through the transition (weapons follow the player)
	pEntity = pEntity->GetRootMoveParent();

	int inVolume = TRANSITION_VOLUME_NOT_FOUND;	// Unless we find a trigger_transition, everything is in the volume

	pVolume = gEntList.FindEntityByName( NULL, pVolumeName );
	while ( pVolume )
	{
		if ( pVolume && FClassnameIs( pVolume, "trigger_transition" ) )
		{
			if ( TestEntityTriggerIntersection_Accurate(pVolume,  pEntity ) )	// It touches one, it's in the volume
				return TRANSITION_VOLUME_PASSED;

			inVolume = TRANSITION_VOLUME_SCREENED_OUT;	// Found a trigger_transition, but I don't intersect it -- if I don't find another, don't go!
		}
		pVolume = gEntList.FindEntityByName( pVolume, pVolumeName );
	}
	return inVolume;
}


//------------------------------------------------------------------------------
// Builds the list of entities to save when moving across a transition
//------------------------------------------------------------------------------
int CChangeLevel::BuildChangeLevelList( levellist_t *pLevelList, int maxList )
{


	int nCount = 0;
	CBaseEntity *pentChangelevel = gEntList.FindEntityByClassname( NULL, "trigger_changelevel" );
	while ( pentChangelevel )
	{
		CChangeLevel *pTrigger = dynamic_cast<CChangeLevel *>(pentChangelevel);
		if ( pTrigger )
		{
			// Find the corresponding landmark
			CBaseEntity *pentLandmark = FindLandmark( pTrigger->m_szLandmarkName );
			if ( pentLandmark )
			{
				// Build a list of unique transitions
				if ( AddTransitionToList( pLevelList, nCount, pTrigger->m_szMapName, pTrigger->m_szLandmarkName, pentLandmark->edict() ) )
				{
					++nCount;
					if ( nCount >= maxList )		// FULL!!
						break;
				}
			}
		}
		pentChangelevel = gEntList.FindEntityByClassname( pentChangelevel, "trigger_changelevel" );
	}

	return nCount;
}


//------------------------------------------------------------------------------
// Adds a single entity to the transition list, if appropriate. Returns the new count
//------------------------------------------------------------------------------
int CChangeLevel::ComputeEntitySaveFlags( CBaseEntity *pEntity )
{
	if ( g_iDebuggingTransition == DEBUG_TRANSITIONS_VERBOSE )
	{
		Msg( "Trying %s (%s): ", pEntity->GetClassname(), pEntity->GetDebugName() );
	}

	int caps = pEntity->ObjectCaps();
	if ( caps & FCAP_DONT_SAVE )
	{
		if ( g_iDebuggingTransition == DEBUG_TRANSITIONS_VERBOSE )
		{
			Msg( "IGNORED due to being marked \"Don't save\".\n" );
		}
		return 0;
	}

	// If this entity can be moved or is global, mark it
	int flags = 0;
	if ( caps & FCAP_ACROSS_TRANSITION )
	{
		flags |= FENTTABLE_MOVEABLE;
	}
	if ( pEntity->m_iGlobalname != NULL_STRING && !pEntity->IsDormant() )
	{
		flags |= FENTTABLE_GLOBAL;
	}
	
	if ( g_iDebuggingTransition == DEBUG_TRANSITIONS_VERBOSE && !flags )
	{
		Msg( "IGNORED, no across_transition flag & no globalname\n" );
	}

	return flags;
}


//------------------------------------------------------------------------------
// Adds a single entity to the transition list, if appropriate. Returns the new count
//------------------------------------------------------------------------------
inline int CChangeLevel::AddEntityToTransitionList( CBaseEntity *pEntity, int flags, int nCount, CBaseEntity **ppEntList, int *pEntityFlags )
{
	ppEntList[ nCount ] = pEntity;
	pEntityFlags[ nCount ] = flags;
	++nCount;

	// If we're debugging, make it visible
	if ( g_iDebuggingTransition )
	{
		if ( g_iDebuggingTransition == DEBUG_TRANSITIONS_VERBOSE )
		{
			// In verbose mode we've already printed out what the entity is
			Msg("ADDED.\n");
		}
		else
		{
			// In non-verbose mode, we just print this line
			Msg( "ADDED %s (%s) to transition.\n", pEntity->GetClassname(), pEntity->GetDebugName() );
		}

		pEntity->m_debugOverlays |= (OVERLAY_BBOX_BIT | OVERLAY_NAME_BIT);
	}

	return nCount;
}


//------------------------------------------------------------------------------
// Builds the list of entities to bring across a particular transition
//------------------------------------------------------------------------------
int CChangeLevel::BuildEntityTransitionList( CBaseEntity *pLandmarkEntity, const char *pLandmarkName,
	CBaseEntity **ppEntList, int *pEntityFlags, int nMaxList )
{
	int iEntity = 0;

	// Only show debug for the transition to the level we're going to
	if ( g_debug_transitions.GetInt() && pLandmarkEntity->NameMatches(st_szNextSpot) )
	{
		g_iDebuggingTransition = g_debug_transitions.GetInt();

		// Show us where the landmark entity is
		pLandmarkEntity->m_debugOverlays |= (OVERLAY_PIVOT_BIT | OVERLAY_BBOX_BIT | OVERLAY_NAME_BIT);
	}
	else
	{
		g_iDebuggingTransition = 0;
	}

	// Follow the linked list of entities in the PVS of the transition landmark
	CBaseEntity *pEntity = NULL; 
	while ( (pEntity = UTIL_EntitiesInPVS( pLandmarkEntity, pEntity)) != NULL )
	{
		int flags = ComputeEntitySaveFlags( pEntity );
		if ( !flags )
			continue;

		// Check to make sure the entity isn't screened out by a trigger_transition
		if ( !InTransitionVolume( pEntity, pLandmarkName ) )
		{
			if ( g_iDebuggingTransition == DEBUG_TRANSITIONS_VERBOSE )
			{
				Msg( "IGNORED, outside transition volume.\n" );
			}
			continue;
		}

		if ( iEntity >= nMaxList )
		{
			Warning( "Too many entities across a transition!\n" );
			Assert( 0 );
			return iEntity;
		}

		iEntity = AddEntityToTransitionList( pEntity, flags, iEntity, ppEntList, pEntityFlags );
	}

	return iEntity;
}



//------------------------------------------------------------------------------
// Adds in all entities depended on by entities near the transition
//------------------------------------------------------------------------------
#define MAX_ENTITY_BYTE_COUNT	(NUM_ENT_ENTRIES >> 3)
int CChangeLevel::AddDependentEntities( int nCount, CBaseEntity **ppEntList, int *pEntityFlags, int nMaxList )
{
	char pEntitiesSaved[MAX_ENTITY_BYTE_COUNT];
	memset( pEntitiesSaved, 0, MAX_ENTITY_BYTE_COUNT * sizeof(char) );
			  
	// Populate the initial bitfield
	int i;
	for ( i = 0; i < nCount; ++i )
	{
		// NOTE: Must use GetEntryIndex because we're saving non-networked entities
		int nEntIndex = ppEntList[i]->GetRefEHandle().GetEntryIndex();

		// We shouldn't already have this entity in the list!
		Assert( !IsBitSet( pEntitiesSaved, nEntIndex ) );

		// Mark the entity as being in the list
		Set( pEntitiesSaved, nEntIndex );
	}

	IEntitySaveUtils *pSaveUtils = GetEntitySaveUtils();

	// Iterate over entities whose dependencies we've not yet processed
	// NOTE: nCount will change value during this loop in AddEntityToTransitionList
	for ( i = 0; i < nCount; ++i )
	{
		CBaseEntity *pEntity = ppEntList[i];

		// Find dependencies in the hash.
		int nDepCount = pSaveUtils->GetEntityDependencyCount( pEntity );
		if ( !nDepCount )
			continue;

		CBaseEntity **ppDependentEntities = (CBaseEntity**)stackalloc( nDepCount * sizeof(CBaseEntity*) );
		pSaveUtils->GetEntityDependencies( pEntity, nDepCount, ppDependentEntities );
		for ( int j = 0; j < nDepCount; ++j )
		{
			CBaseEntity *pDependent = ppDependentEntities[j];
			if ( !pDependent )
				continue;

			// NOTE: Must use GetEntryIndex because we're saving non-networked entities
			int nEntIndex = pDependent->GetRefEHandle().GetEntryIndex();

			// Don't re-add it if it's already in the list
			if ( IsBitSet( pEntitiesSaved, nEntIndex ) )
				continue;

			// Mark the entity as being in the list
			Set( pEntitiesSaved, nEntIndex );

			int flags = ComputeEntitySaveFlags( pEntity );
			if ( flags )
			{
				if ( nCount >= nMaxList )
				{
					Warning( "Too many entities across a transition!\n" );
					Assert( 0 );
					return false;
				}

				if ( g_debug_transitions.GetInt() )
				{
					Msg( "ADDED DEPENDANCY: %s (%s)\n", pEntity->GetClassname(), pEntity->GetDebugName() );
				}

				nCount = AddEntityToTransitionList( pEntity, flags, nCount, ppEntList, pEntityFlags );
			}
			else
			{
				Warning("Warning!! Save dependency is linked to an entity that doesn't want to be saved!\n");
			}
		}
	}

	return nCount;
}


//------------------------------------------------------------------------------
// This builds the list of all transitions on this level and which entities 
// are in their PVS's and can / should be moved across.
//------------------------------------------------------------------------------

// We can only ever move 512 entities across a transition
#define MAX_ENTITY 512

// FIXME: This has grown into a complicated beast. Can we make this more elegant?
int CChangeLevel::ChangeList( levellist_t *pLevelList, int maxList )
{
	// Find all of the possible level changes on this BSP
	int count = BuildChangeLevelList( pLevelList, maxList );

	if ( !gpGlobals->pSaveData || ( static_cast<CSaveRestoreData *>(gpGlobals->pSaveData)->NumEntities() == 0 ) )
		return count;

	CSave saveHelper( static_cast<CSaveRestoreData *>(gpGlobals->pSaveData) );

	// For each level change, find nearby entities and save them
	int	i;
	for ( i = 0; i < count; i++ )
	{
		CBaseEntity *pEntList[ MAX_ENTITY ];
		int			 entityFlags[ MAX_ENTITY ];

		// First, figure out which entities are near the transition
		CBaseEntity *pLandmarkEntity = CBaseEntity::Instance( pLevelList[i].pentLandmark );
		int iEntity = BuildEntityTransitionList( pLandmarkEntity, pLevelList[i].landmarkName, pEntList, entityFlags, MAX_ENTITY );

		// FIXME: Activate if we have a dependency problem on level transition
		// Next, add in all entities depended on by entities near the transition
//		iEntity = AddDependentEntities( iEntity, pEntList, entityFlags, MAX_ENTITY );

		int j;
		for ( j = 0; j < iEntity; j++ )
		{
			// Mark entity table with 1<<i
			int index = saveHelper.EntityIndex( pEntList[j] );
			// Flag it with the level number
			saveHelper.EntityFlagsSet( index, entityFlags[j] | (1<<i) );
		}
	}

	return count;
}
