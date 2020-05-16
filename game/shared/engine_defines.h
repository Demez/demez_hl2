#ifndef ENGINE_DEFINES_H
#define ENGINE_DEFINES_H
#ifdef _WIN32
#pragma once
#endif

// yes, this is evil
// but tell me, do you want to place about 100 #if ENGINE_ASW statements just so it's not "evil"?
// it would be way messier as well, this is actually a very clean way to do it
#if ENGINE_ASW || ENGINE_CSGO

	#define SetRenderColorAlpha(pEntity, red, green, blue, alpha) \
					pEntity->SetRenderColor( red, green, blue ); \
					pEntity->SetRenderAlpha( alpha );

	#define RENDER_INSTANCE_INPUT , const RenderableInstance_t &instance
	#define RENDER_INSTANCE , instance
	
	#define extern_g_sModelIndexFireball extern int g_sModelIndexFireball
	
	#define BeamSegColors(bemSeg) beamSeg.m_color.r, beamSeg.m_color.g, beamSeg.m_color.b, beamSeg.m_color.a
	#define FireBullets_PlayerDamage(info) info.m_flPlayerDamage
	#define Engine_UTIL_PointContents(vector, contentsMask) UTIL_PointContents(vector, contentsMask)
	#define VectorCount() Count()
	#define TypeDesc_FieldOffset(dataDesc, offset) dataDesc->fieldOffset
	#define AmmoMaxCarry(iAmmoType, pPlayer) MaxCarry(iAmmoType, pPlayer)

	#if CLIENT_DLL
		// #define SteamUser() steamapicontext->SteamUser()
	#elif GAME_DLL
		#define GetAnimEvent( pEvent ) pEvent->Event()
		#define SetAnimEvent( pEvent, newEvent ) pEvent->Event( newEvent )
	#endif

#elif ENGINE_QUIVER || ENGINE_2013
	#define RENDER_INSTANCE_INPUT
	#define RENDER_INSTANCE

	#define RENDER_GROUP_TRANSLUCENT RENDER_GROUP_TRANSLUCENT_ENTITY
	#define RENDER_GROUP_OPAQUE RENDER_GROUP_OPAQUE_ENTITY

	#define GET_ACTIVE_SPLITSCREEN_SLOT()

	#define matrix3x4a_t matrix3x4_t
	#define extern_g_sModelIndexFireball extern short g_sModelIndexFireball

	#define PRECACHE_REGISTER_BEGIN( _system, _className ) CLIENTEFFECT_REGISTER_BEGIN( _className )
	#define PRECACHE( _type, _name ) CLIENTEFFECT_MATERIAL( _name )
	#define PRECACHE_REGISTER_END() CLIENTEFFECT_REGISTER_END()

	#define DECLARE_CLIENT_EFFECT( effectName, callbackFunction ) \
		static CClientEffectRegistration ClientEffectReg_##callbackFunction( #effectName, callbackFunction );

	#define BeamSegColors(bemSeg) VectorExpand( beamSeg.m_vColor ), beamSeg.m_flAlpha
	#define Engine_UTIL_PointContents(vector, contentsMask) UTIL_PointContents(vector) & (contentsMask)
	#define FireBullets_PlayerDamage(info) info.m_iPlayerDamage
	#define GetRenderAlpha() GetRenderColor().a
	#define SetRenderAlpha   SetRenderColorA
	#define VectorCount() Size()
	#define TypeDesc_FieldOffset(dataDesc, offset) dataDesc->fieldOffset[offset]
	#define AmmoMaxCarry(iAmmoType, pPlayer) MaxCarry(iAmmoType)

	struct RenderableInstance_t {};

	#if CLIENT_DLL
		#define GetHud() gHUD
		#define GetClientMode() g_pClientMode
		#define GetMapOverView() g_pMapOverview
		#define GetViewPortInterface() gViewPortInterface

		#define GetHudList() m_HudList
	#elif GAME_DLL
		#define LAST_SHARED_ENTITY_CLASS NUM_AI_CLASSES

		#define SetRenderColorAlpha(pEntity, red, green, blue, alpha) \
				pEntity->SetRenderColor( red, green, blue, alpha );

		#define GetAnimEvent( pEvent ) pEvent->event
		#define SetAnimEvent( pEvent, newEvent ) pEvent->event = newEvent

		#define GetPrimaryBehavior() GetRunningBehavior()
	#endif

#endif // ENGINE_QUIVER || ENGINE_2013

#if ENGINE_2013 || ENGINE_NEW
	#define FireBullets_Damage(info) info.m_flDamage
#elif ENGINE_QUIVER
	#define FireBullets_Damage(info) info.m_iDamage
#endif

#endif // ENGINE_DEFINES_H


