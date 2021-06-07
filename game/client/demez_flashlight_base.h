//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
//===========================================================================//

#include "flashlighteffect.h"
#include "dlight.h"
#include "iefx.h"
#include "iviewrender.h"
#include "view.h"
#include "engine/ivdebugoverlay.h"
#include "tier0/vprof.h"
#include "tier1/KeyValues.h"
#include "toolframework_client.h"

#include "c_basehlplayer.h"

#if ENGINE_QUIVER
extern ConVar r_shadowmapresolution;
#else
extern ConVar r_flashlightdepthres;
#endif

#if ENGINE_NEW
static ConVar r_flashlight_topdown( "r_flashlight_topdown", "0" );
#elif ENGINE_OLD
static ConVar r_newflashlight( "r_newflashlight", "1", FCVAR_CHEAT, "" );
#endif

static ConVar r_swingflashlight( "r_swingflashlight", "1", FCVAR_CHEAT );
static ConVar r_flashlightlockposition( "r_flashlightlockposition", "0", FCVAR_CHEAT );
static ConVar r_flashlightnear( "r_flashlightnear", "4.0", FCVAR_CHEAT );
static ConVar r_flashlightfar( "r_flashlightfar", "750.0", FCVAR_CHEAT );
static ConVar r_flashlightvisualizetrace( "r_flashlightvisualizetrace", "0", FCVAR_CHEAT );
static ConVar r_flashlightambient( "r_flashlightambient", "0.0", FCVAR_CHEAT );
static ConVar r_flashlightshadowatten( "r_flashlightshadowatten", "0.35", FCVAR_CHEAT );
static ConVar r_flashlightladderdist( "r_flashlightladderdist", "40.0", FCVAR_CHEAT );

static ConVar r_flashlightoffsetx( "r_flashlightoffsetright", "5.0", FCVAR_CHEAT );
static ConVar r_flashlightoffsety( "r_flashlightoffsetup", "-10.0", FCVAR_CHEAT ); // raised to stop it from snapping up when right against a wall and looking up
static ConVar r_flashlightoffsetz( "r_flashlightoffsetforward", "8.0", FCVAR_CHEAT );

static ConVar r_flashlightnearoffsetscale( "r_flashlightnearoffsetscale", "1.0", FCVAR_CHEAT );
static ConVar r_flashlightbacktraceoffset( "r_flashlightbacktraceoffset", "0.4", FCVAR_CHEAT );
static ConVar r_flashlighttracedistcutoff( "r_flashlighttracedistcutoff", "0", FCVAR_CHEAT, "If the trace at this distance hits the wall, fuck you and die  The distance until flashlight gets closer to wall" );

// demez stuff
static ConVar r_flashlight_fov( "d_lightfov", "75.0", FCVAR_ARCHIVE);
static ConVar r_flashlight_filtersize( "d_lightfiltersize", "1.0", FCVAR_ARCHIVE );

static ConVar r_flashlight_constant(  "d_lightconstant",    "0.0" );
static ConVar r_flashlight_linear(    "d_lightlinear",      "0.0" );
static ConVar r_flashlight_quadratic( "d_lightquadratic",   "12500.0" );


