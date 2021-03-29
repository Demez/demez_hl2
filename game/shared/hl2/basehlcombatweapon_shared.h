//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "basecombatweapon_shared.h"
#include "hl2mp_weapon_parse.h"
#include "hl2_shareddefs.h"

#ifdef CLIENT_DLL
#include "c_hl2mp_player.h"
#else
#include "hl2mp_player.h"
#endif

#ifndef BASEHLCOMBATWEAPON_SHARED_H
#define BASEHLCOMBATWEAPON_SHARED_H
#ifdef _WIN32
#pragma once
#endif

#if defined( CLIENT_DLL )
#define CBaseHLCombatWeapon C_BaseHLCombatWeapon
#endif

class CHL2MP_Player;

extern ConVar sk_auto_reload_time;

class CBaseHLCombatWeapon : public CBaseCombatWeapon
{
#if !defined( CLIENT_DLL )
#ifndef _XBOX
	DECLARE_DATADESC();
#else
protected:
	DECLARE_DATADESC();
private:
#endif
#endif

	DECLARE_CLASS( CBaseHLCombatWeapon, CBaseCombatWeapon );

public:
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	CBaseHLCombatWeapon();

	virtual bool	WeaponShouldBeLowered( void );

			bool	CanLower();
	virtual bool	Ready( void );
	virtual bool	Lower( void );
	virtual bool	Deploy( void );
	virtual bool	Holster( CBaseCombatWeapon *pSwitchingTo );
	virtual void	WeaponIdle( void );

	virtual bool	CanBePickedUpByNPCs() { return true; }

	virtual void	AddViewmodelBob( CBaseViewModel *viewmodel, Vector &origin, QAngle &angles );
	virtual	float	CalcViewmodelBob( void );

	virtual Vector	GetBulletSpread( WeaponProficiency_t proficiency );
	virtual float	GetSpreadBias( WeaponProficiency_t proficiency );

	virtual const	WeaponProficiencyInfo_t *GetProficiencyValues();
	static const	WeaponProficiencyInfo_t *GetDefaultProficiencyValues();

	virtual void	ItemHolsterFrame( void );

	// =============================================================
	// From HL2MP Weapons 

	// All predicted weapons need to implement and return true
	virtual bool	IsPredicted() const;
#ifdef CLIENT_DLL
	virtual bool	ShouldPredict();
	virtual void	OnDataChanged( DataUpdateType_t type );

	virtual bool	OnFireEvent( C_BaseViewModel *pViewModel, const Vector& origin, const QAngle& angles, int event, const char *options );
#endif

	Vector			GetOriginalSpawnOrigin(void) { return m_vOriginalSpawnOrigin; }
	QAngle			GetOriginalSpawnAngles(void) { return m_vOriginalSpawnAngles; }

	CBasePlayer*	GetPlayerOwner() const;
	CHL2MP_Player*	GetHL2MPPlayerOwner() const;

	void			WeaponSound(WeaponSound_t sound_type, float soundtime = 0.0f);

	CHL2MPSWeaponInfo const& GetHL2MPWpnData() const;

	virtual void FireBullets(const FireBulletsInfo_t& info);
	virtual void FallInit(void);

	bool IsWeaponZoomed()
	{
		return false;
	}

	#ifdef GAME_DLL
	// void			SendReloadSoundEvent(void);

	void			Materialize(void);
	virtual	int		ObjectCaps(void);
	#endif

	int				m_iPrimaryAttacks;		// # of primary attacks performed with this weapon
	int				m_iSecondaryAttacks;	// # of secondary attacks performed with this weapon
	float			m_flNextResetCheckTime;

protected:

	bool			m_bLowered;			// Whether the viewmodel is raised or lowered
	float			m_flRaiseTime;		// If lowered, the time we should raise the viewmodel
	float			m_flHolsterTime;	// When the weapon was holstered

private:

	Vector m_vOriginalSpawnOrigin;
	QAngle m_vOriginalSpawnAngles;
};

#endif // BASEHLCOMBATWEAPON_SHARED_H
