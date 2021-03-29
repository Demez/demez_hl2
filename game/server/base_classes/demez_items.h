
#ifndef DEMEZ_ITEMS_H
#define DEMEZ_ITEMS_H

#ifdef _WIN32
#pragma once
#endif

// just use the functions ffs
#define private public
#include "items.h"
#undef private

#include "props.h"
#include "engine_defines.h"

#if ENGINE_CSGO
#include "cstrike15_usermessages.pb.h"
#include "hl2_usermessages.pb.h"
#endif

class CDemezItem: public CItem
{
	DECLARE_CLASS( CDemezItem, CItem )

public:
	CDemezItem();
	~CDemezItem();
	
	DECLARE_DATADESC()
	
	void		Spawn( void );
	void		FallThink( void );

	bool		CanRespawn( void );

	float		m_flNextResetCheckTime;
	bool		m_bCanRespawn;
};



#endif // DEMEZ_ITEMS_H
