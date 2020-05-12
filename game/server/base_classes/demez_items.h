
#ifndef DEMEZ_ITEMS_H
#define DEMEZ_ITEMS_H

#ifdef _WIN32
#pragma once
#endif

// full smile, again...
#define private public
#include "items.h"
#undef private

#include "engine_defines.h"

class CDemezItem: public CItem
{
	DECLARE_CLASS( CDemezItem, CItem )

public:
	CDemezItem();
	~CDemezItem();
	
	DECLARE_DATADESC()
	
	void    Spawn( void );
	void    FallThink( void );

	float   m_flNextResetCheckTime;
};


#endif // DEMEZ_ITEMS_H
