#pragma once

#include "triggers.h"

//------------------------------------------------------------------------------
// Reesponsible for changing levels when the player touches it
//------------------------------------------------------------------------------
#define SF_CHANGELEVEL_NOTOUCH		0x0002
#define SF_CHANGELEVEL_CHAPTER		0x0004

#define cchMapNameMost 32

class CChangeLevel : public CBaseTrigger
{
	DECLARE_DATADESC();

public:
	DECLARE_CLASS( CChangeLevel, CBaseTrigger );

	void Spawn( void );
	void Activate( void );
	bool KeyValue( const char *szKeyName, const char *szValue );

	static int ChangeList( levellist_t *pLevelList, int maxList );

// private:
	void TouchChangeLevel( CBaseEntity *pOther );
	void ChangeLevelNow( CBaseEntity *pActivator, bool force = false );

	bool ShouldChangeLevelOld( CBaseEntity *pActivator );
	bool ShouldChangeLevel( CBaseEntity *pActivator, bool force );

	void InputChangeLevel( inputdata_t &inputdata );

	bool IsEntityInTransition( CBaseEntity *pEntity );
	void NotifyEntitiesOutOfTransition();

	void WarnAboutActiveLead( void );

	void FindLandmark();

	static CBaseEntity *FindLandmark( const char *pLandmarkName );
	static int AddTransitionToList( levellist_t *pLevelList, int listCount, const char *pMapName, const char *pLandmarkName, edict_t *pentLandmark );
	static int InTransitionVolume( CBaseEntity *pEntity, const char *pVolumeName );

	// Builds the list of entities to save when moving across a transition
	static int BuildChangeLevelList( levellist_t *pLevelList, int maxList );

	// Builds the list of entities to bring across a particular transition
	static int BuildEntityTransitionList( CBaseEntity *pLandmarkEntity, const char *pLandmarkName, CBaseEntity **ppEntList, int *pEntityFlags, int nMaxList );

	// Adds a single entity to the transition list, if appropriate. Returns the new count
	static int AddEntityToTransitionList( CBaseEntity *pEntity, int flags, int nCount, CBaseEntity **ppEntList, int *pEntityFlags );

	// Adds in all entities depended on by entities near the transition
	static int AddDependentEntities( int nCount, CBaseEntity **ppEntList, int *pEntityFlags, int nMaxList );

	// Figures out save flags for the entity
	static int ComputeEntitySaveFlags( CBaseEntity *pEntity );

	int m_iTransitionPlayers;

private:
	char m_szMapName[cchMapNameMost];		// trigger_changelevel only:  next map
	char m_szLandmarkName[cchMapNameMost];		// trigger_changelevel only:  landmark on next map
	bool m_bTouched;

	CBaseEntity *m_pLandmark;

	// Outputs
	COutputEvent m_OnChangeLevel;
};