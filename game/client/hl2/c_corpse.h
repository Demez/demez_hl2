//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#if !defined( C_CORPSE_H )
#define C_CORPSE_H
#ifdef _WIN32
#pragma once
#endif

#include "engine_defines.h"

class C_Corpse : public C_BaseAnimating
{
public:
	DECLARE_CLASS( C_Corpse, C_BaseAnimating );
	DECLARE_CLIENTCLASS();

						C_Corpse( void );

	virtual int			DrawModel( int flags
#if ENGINE_NEW
								  , const RenderableInstance_t &instance
#endif
	);

public:
	// The player whom we are copying our data from
	int					m_nReferencePlayer;

private:
						C_Corpse( const C_Corpse & );
};


#endif // C_CORPSE_H