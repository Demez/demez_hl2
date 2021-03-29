//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef CS_HUD_CHAT_H
#define CS_HUD_CHAT_H
#ifdef _WIN32
#pragma once
#endif

#include <hud_basechat.h>

#if ENGINE_CSGO
#include "hl2_usermessages.pb.h"
#endif

class CHudChatLine : public CBaseHudChatLine
{
	DECLARE_CLASS_SIMPLE( CHudChatLine, CBaseHudChatLine );

public:
	CHudChatLine( vgui::Panel *parent, const char *panelName ) : CBaseHudChatLine( parent, panelName ) {}

	virtual void	ApplySchemeSettings(vgui::IScheme *pScheme);

#if !ENGINE_CSGO
	void			MsgFunc_SayText(bf_read &msg);
#endif

private:
	CHudChatLine( const CHudChatLine & ); // not defined, not accessible
};

//-----------------------------------------------------------------------------
// Purpose: The prompt and text entry area for chat messages
//-----------------------------------------------------------------------------
class CHudChatInputLine : public CBaseHudChatInputLine
{
	DECLARE_CLASS_SIMPLE( CHudChatInputLine, CBaseHudChatInputLine );
	
public:
	CHudChatInputLine( CBaseHudChat *parent, char const *panelName ) : CBaseHudChatInputLine( parent, panelName ) {}

	virtual void	ApplySchemeSettings(vgui::IScheme *pScheme);
};

class CHudChat : public CBaseHudChat
{
	DECLARE_CLASS_SIMPLE( CHudChat, CBaseHudChat );

public:
	CHudChat( const char *pElementName );

	virtual void	CreateChatInputLine( void );
	virtual void	CreateChatLines( void );

	virtual void	Init( void );
	virtual void	Reset( void );
	virtual void	ApplySchemeSettings(vgui::IScheme *pScheme);

	int				GetChatInputOffset( void );

	virtual Color	GetClientColor( int clientIndex );
	
#if ENGINE_CSGO
	CUserMessageBinder m_UMCMsgSayText;
	CUserMessageBinder m_UMCMsgSayText2;
	CUserMessageBinder m_UMCMsgTextMsg;
#endif

/*#if ENGINE_CSGO
	bool			MsgFunc_SayText(const CCSUsrMsg_SayText &msg);
	CUserMessageBinder m_UMCMsgSayText;
#else
	void			MsgFunc_SayText(bf_read &msg);
#endif*/

};

#endif	//CS_HUD_CHAT_H