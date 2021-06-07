#pragma once

#include "ammodef.h"
#include "engine_defines.h"


class CDemezAmmoDef: public CAmmoDef
{
	typedef CAmmoDef BaseClass;

public:
	CDemezAmmoDef();
	~CDemezAmmoDef();

	void UpdateAmmoType( char const* name, int damageType, int tracerType, int plr_dmg, int npc_dmg, int carry, float physicsForceImpulse, int nFlags, int minSplashSize = 4, int maxSplashSize = 8 );
	void UpdateAmmoType( char const* name, int damageType, int tracerType, char const* plr_cvar, char const* npc_var, char const* carry_cvar, float physicsForceImpulse, int nFlags, int minSplashSize = 4, int maxSplashSize = 8 );

	void ResetAmmoTypes();

private:
	bool UpdateAmmoType(char const* name, int damageType, int tracerType, int nFlags, int minSplashSize = 4, int maxSplashSize = 8 );
};



void UpdateAmmoDef();

