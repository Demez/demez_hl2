//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: HL2 specific input handling
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "kbutton.h"
#include "input.h"
#include "vr_input.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: HL Input interface
//-----------------------------------------------------------------------------
class CInput;

class CHLInput : public CVRInput
{
public:
	typedef CVRInput BaseClass;
};

static CHLInput g_Input;

// Expose this interface
IInput *input = ( IInput * )&g_Input;
