#include "cbase.h"
#include "nb_header_footer.h"
#include "vgui_controls/Label.h"
#include "vgui_controls/ImagePanel.h"
#include <vgui/ISurface.h>
#include "vgui_hudvideo.h"
//#include "sdk_video.h"
#include "VGUIMatSurface/IMatSystemSurface.h"
#ifdef HL2_CLIENT_DLL
#include "hl2_gamerules.h"
#elif DEATHMATCH_CLIENT_DLL
#include "dm_gamerules.h"
#endif

// UI defines. Include if you want to implement some of them [str]
//#ifdef HL2_CLIENT_DLL
//#include "hl2/ui_defines.h"
//#elif GAMEUI_SHARED
#include "ui_defines.h"
//#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

#ifdef HL2_CLIENT_DLL
CHL2_Background_Movie *g_pBackgroundMovie = NULL;

CHL2_Background_Movie* HL2BackgroundMovie()
{
	if ( !g_pBackgroundMovie )
	{
		g_pBackgroundMovie = new CHL2_Background_Movie();
	}
	return g_pBackgroundMovie;
}


CHL2_Background_Movie::CHL2_Background_Movie()
{
#elif GAMEUI_SHARED
C_Background_Movie *g_pBackgroundMovie = NULL;

C_Background_Movie* BackgroundMovie()
{
	if (!g_pBackgroundMovie)
	{
		g_pBackgroundMovie = new C_Background_Movie();
	}
	return g_pBackgroundMovie;
}


C_Background_Movie::C_Background_Movie()
{
#endif
#ifdef ASW_BINK_MOVIES
	m_nBIKMaterial = BIKMATERIAL_INVALID;
#else
	m_nAVIMaterial = AVIMATERIAL_INVALID;
#endif
	m_nTextureID = -1;
	m_szCurrentMovie[0] = 0;
	m_nLastGameState = -1;
}

#ifdef HL2_CLIENT_DLL
CHL2_Background_Movie::~CHL2_Background_Movie()
{

}

void CHL2_Background_Movie::SetCurrentMovie( const char *szFilename )
{
	if ( Q_strcmp( m_szCurrentMovie, szFilename ) )
	{
#elif GAMEUI_SHARED
C_Background_Movie::~C_Background_Movie()
{

}

void C_Background_Movie::SetCurrentMovie(const char *szFilename)
{
	if (Q_strcmp(m_szCurrentMovie, szFilename))
	{
#endif
#ifdef ASW_BINK_MOVIES
		if ( m_nBIKMaterial != BIKMATERIAL_INVALID )
		{
			// FIXME: Make sure the m_pMaterial is actually destroyed at this point!
			g_pBIK->DestroyMaterial( m_nBIKMaterial );
			m_nBIKMaterial = BIKMATERIAL_INVALID;
			m_nTextureID = -1;
		}

		char szMaterialName[ MAX_PATH ];
		Q_snprintf( szMaterialName, sizeof( szMaterialName ), "BackgroundBIKMaterial%i", g_pBIK->GetGlobalMaterialAllocationNumber() );
		m_nBIKMaterial = bik->CreateMaterial( szMaterialName, szFilename, "GAME", BIK_LOOP );
#else
		if ( m_nAVIMaterial != AVIMATERIAL_INVALID )
		{
			// FIXME: Make sure the m_pMaterial is actually destroyed at this point!
			g_pAVI->DestroyAVIMaterial( m_nAVIMaterial );
			m_nAVIMaterial = AVIMATERIAL_INVALID;
			m_nTextureID = -1;
		}

		char szMaterialName[ MAX_PATH ];
		static int g_nGlobalAVIAllocationCount = 0;
		Q_snprintf( szMaterialName, sizeof( szMaterialName ), "BackgroundAVIMaterial%i", g_nGlobalAVIAllocationCount++ );
		m_nAVIMaterial = g_pAVI->CreateAVIMaterial( szMaterialName, szFilename, "GAME" );
		m_flStartTime = gpGlobals->realtime;

		IMaterial *pMaterial = avi->GetMaterial( m_nAVIMaterial );
		pMaterial->IncrementReferenceCount();
#endif

		Q_snprintf( m_szCurrentMovie, sizeof( m_szCurrentMovie ), "%s", szFilename );
	}
}

#ifdef HL2_CLIENT_DLL
void CHL2_Background_Movie::ClearCurrentMovie()
#elif GAMEUI_SHARED
void C_Background_Movie::ClearCurrentMovie()
#endif
{
#ifdef ASW_BINK_MOVIES
	if ( m_nBIKMaterial != BIKMATERIAL_INVALID )
	{
		// FIXME: Make sure the m_pMaterial is actually destroyed at this point!
		g_pBIK->DestroyMaterial( m_nBIKMaterial );
		m_nBIKMaterial = BIKMATERIAL_INVALID;
		m_nTextureID = -1;
	}
#else
	if ( m_nAVIMaterial != AVIMATERIAL_INVALID )
	{
		// FIXME: Make sure the m_pMaterial is actually destroyed at this point!
		g_pAVI->DestroyAVIMaterial( m_nAVIMaterial );
		m_nAVIMaterial = AVIMATERIAL_INVALID;
		m_nTextureID = -1;
	}
#endif
}

#ifdef HL2_CLIENT_DLL
int CHL2_Background_Movie::SetTextureMaterial()
#elif GAMEUI_SHARED
int C_Background_Movie::SetTextureMaterial()
#endif
{
#ifdef ASW_BINK_MOVIES
	if ( m_nBIKMaterial == BIKMATERIAL_INVALID )
		return -1;
#else
	if ( m_nAVIMaterial == AVIMATERIAL_INVALID )
		return -1;
#endif

	if ( m_nTextureID == -1 )
	{
		m_nTextureID = g_pMatSystemSurface->CreateNewTextureID( true );
	}
	
#ifdef ASW_BINK_MOVIES
	g_pMatSystemSurface->DrawSetTextureMaterial( m_nTextureID, g_pBIK->GetMaterial( m_nBIKMaterial ) );
#else
	g_pMatSystemSurface->DrawSetTextureMaterial( m_nTextureID, g_pAVI->GetMaterial( m_nAVIMaterial ) );
#endif
	return m_nTextureID;
}

#ifdef HL2_CLIENT_DLL
void CHL2_Background_Movie::Update()
{
	if ( engine->IsConnected() && HL2GameRules() )
	{
		// Do something based on the gamerules...
		int nGameState = 1;
		if ( nGameState != m_nLastGameState )
		{
			// todo: whats this? [str]

#elif GAMEUI_SHARED
void C_Background_Movie::Update()
{
#ifdef DEATHMATCH_CLIENT_DLL
	if (engine->IsConnected() && DMGameRules())
#elif HL2_CLIENT_DLL
	if (engine->IsConnected() && HL2GameRules())
#endif
	{
		// Do something based on the gamerules...
		int nGameState = 1;
		if (nGameState != m_nLastGameState)
		{
			// todo: whats this? [str]

#endif

#ifdef ASW_BINK_MOVIES
			SetCurrentMovie( "media/BGFX_01.bik" );
#else
			SetCurrentMovie( "media/test.avi" );
#endif
			m_nLastGameState = nGameState;
		}
	}
	else
	{
		int nGameState = 0;
		if ( nGameState != m_nLastGameState )
		{
#ifdef UI_USING_RANDOMMENUMOVIES
			int nMovieIndex = 0;
			nMovieIndex = random->RandomInt(0, ARRAYSIZE(g_ppszRandomMenuMovies)); 
			SetCurrentMovie( g_ppszRandomMenuMovies[nMovieIndex] );
#else
#ifdef ASW_BINK_MOVIES
			SetCurrentMovie( "media/BG_02.bik" );
#else
			SetCurrentMovie( "media/test.avi" );
#endif
#endif //UI_USING_RANDOMMENUMOVIES
			m_nLastGameState = nGameState;
		}
	}

#ifdef ASW_BINK_MOVIES
	if ( m_nBIKMaterial == BIKMATERIAL_INVALID )
		return;

	if ( g_pBIK->ReadyForSwap( m_nBIKMaterial ) )
	{
		if ( g_pBIK->Update( m_nBIKMaterial ) == false )
		{
			// FIXME: Make sure the m_pMaterial is actually destroyed at this point!
			g_pBIK->DestroyMaterial( m_nBIKMaterial );
			m_nBIKMaterial = BIKMATERIAL_INVALID;
		}
	}
#else
	if ( m_nAVIMaterial == AVIMATERIAL_INVALID )
		return;

	int nFrames = avi->GetFrameCount( m_nAVIMaterial );
	float flTimePerFrame = 1.0f / avi->GetFrameRate( m_nAVIMaterial );
	float flTimePassed = gpGlobals->realtime - m_flStartTime;
	int nFramesPassed = flTimePassed / flTimePerFrame;
	nFramesPassed = nFramesPassed % nFrames;
	avi->SetFrame( m_nAVIMaterial, nFramesPassed );

// 	float flMaxU, flMaxV;
// 	g_pAVI->GetTexCoordRange( m_nAVIMaterial, &flMaxU, &flMaxV );
#endif
}

// ======================================

CNB_Header_Footer::CNB_Header_Footer( vgui::Panel *parent, const char *name ) : BaseClass( parent, name )
{
	// == MANAGED_MEMBER_CREATION_START: Do not edit by hand ==
	m_pBackground = new vgui::Panel( this, "Background" );
	m_pBackgroundImage = new vgui::ImagePanel( this, "BackgroundImage" );	
	m_pTitle = new vgui::Label( this, "Title", "" );
	m_pBottomBar = new vgui::Panel( this, "BottomBar" );
	m_pBottomBarLine = new vgui::Panel( this, "BottomBarLine" );
	m_pTopBar = new vgui::Panel( this, "TopBar" );
	m_pTopBarLine = new vgui::Panel( this, "TopBarLine" );
	// == MANAGED_MEMBER_CREATION_END ==
	m_pGradientBar = new CNB_Gradient_Bar( this, "GradientBar" );
	m_pGradientBar->SetZPos( 2 );

	m_bHeaderEnabled = true;
	m_bFooterEnabled = true;
	m_bMovieEnabled = true;
	m_bGradientBarEnabled = 0;
	m_nTitleStyle = NB_TITLE_MEDIUM;
	m_nBackgroundStyle = NB_BACKGROUND_TRANSPARENT_BLUE;
	m_nGradientBarY = 0;
	m_nGradientBarHeight = 480;
}

CNB_Header_Footer::~CNB_Header_Footer()
{

}

extern ConVar asw_force_background_movie;
ConVar asw_background_color( "asw_background_color", "32 32 32 128", FCVAR_NONE, "Color of background tinting in briefing screens" );

void CNB_Header_Footer::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );
	
	LoadControlSettings( "resource/ui/nb_header_footer.res" );

	// TODO: Different image in widescreen to avoid stretching
	// this image is no longer used
	//m_pBackgroundImage->SetImage( "lobby/swarm_background01" );

	switch( m_nTitleStyle )
	{
		case NB_TITLE_BRIGHT: m_pTitle->SetFgColor( Color( 255, 255, 255, 255 ) ); break;
		case NB_TITLE_MEDIUM: m_pTitle->SetFgColor( Color( UI_STYLE_NB_TITLE_MEDIUM ) ); break;
	}

	switch( m_nBackgroundStyle )
	{
		case NB_BACKGROUND_DARK:
			{
				m_pBackground->SetVisible( true );
				m_pBackgroundImage->SetVisible( false );
				m_pBackground->SetBgColor( Color( 0, 0, 0, 230 ) );
				break;
			}
		case NB_BACKGROUND_TRANSPARENT_BLUE:
			{
				m_pBackground->SetVisible( true );
				m_pBackgroundImage->SetVisible( false );
				m_pBackground->SetBgColor( asw_background_color.GetColor() );
				break;
			}
		case NB_BACKGROUND_TRANSPARENT_RED:
			{
				m_pBackground->SetVisible( true );
				m_pBackgroundImage->SetVisible( false );
				m_pBackground->SetBgColor( Color( 128, 0, 0, 128 ) );
				break;
			}
		case NB_BACKGROUND_BLUE:
			{
				m_pBackground->SetVisible( true );
				m_pBackgroundImage->SetVisible( false );
				m_pBackground->SetBgColor( Color( 16, 32, 46, 230 ) );
				break;
			}
		case NB_BACKGROUND_IMAGE:
			{
				m_pBackground->SetVisible( false );
				m_pBackgroundImage->SetVisible( true );
				break;
			}

		case NB_BACKGROUND_NONE:
			{
				m_pBackground->SetVisible( false );
				m_pBackgroundImage->SetVisible( false );
			}
	}

	m_pTopBar->SetVisible( m_bHeaderEnabled );
	m_pTopBarLine->SetVisible( m_bHeaderEnabled );
	m_pBottomBar->SetVisible( m_bFooterEnabled );
	m_pBottomBarLine->SetVisible( m_bFooterEnabled );
	m_pGradientBar->SetVisible( m_bGradientBarEnabled );
}

void CNB_Header_Footer::PerformLayout()
{
	BaseClass::PerformLayout();

	m_pGradientBar->SetBounds( 0, YRES( m_nGradientBarY ), ScreenWidth(), YRES( m_nGradientBarHeight ) );
}

void CNB_Header_Footer::OnThink()
{
	BaseClass::OnThink();
}

void CNB_Header_Footer::SetTitle( const char *pszTitle )
{
	m_pTitle->SetText( pszTitle );
}

void CNB_Header_Footer::SetTitle( wchar_t *pwszTitle )
{
	m_pTitle->SetText( pwszTitle );
}

void CNB_Header_Footer::SetHeaderEnabled( bool bEnabled )
{
	m_pTopBar->SetVisible( bEnabled );
	m_pTopBarLine->SetVisible( bEnabled );
	m_bHeaderEnabled = bEnabled;
}

void CNB_Header_Footer::SetFooterEnabled( bool bEnabled )
{
	m_pBottomBar->SetVisible( bEnabled );
	m_pBottomBarLine->SetVisible( bEnabled );
	m_bFooterEnabled = bEnabled;
}

void CNB_Header_Footer::SetGradientBarEnabled( bool bEnabled )
{
	m_pGradientBar->SetVisible( bEnabled );
	m_bGradientBarEnabled = bEnabled;
}

void CNB_Header_Footer::SetGradientBarPos( int nY, int nHeight )
{
	m_nGradientBarY = nY;
	m_nGradientBarHeight = nHeight;
	m_pGradientBar->SetBounds( 0, YRES( m_nGradientBarY ), ScreenWidth(), YRES( m_nGradientBarHeight ) );
}

void CNB_Header_Footer::SetTitleStyle( NB_Title_Style nTitleStyle )
{
	m_nTitleStyle = nTitleStyle;
	InvalidateLayout( false, true );
}

void CNB_Header_Footer::SetBackgroundStyle( NB_Background_Style nBackgroundStyle )
{
	m_nBackgroundStyle = nBackgroundStyle;
	InvalidateLayout( false, true );
}

void CNB_Header_Footer::SetMovieEnabled( bool bMovieEnabled )
{
	m_bMovieEnabled = bMovieEnabled;
	InvalidateLayout( false, true );
}

void CNB_Header_Footer::PaintBackground()
{
	BaseClass::PaintBackground();
#ifdef HL2_CLIENT_DLL
	if ( m_bMovieEnabled && HL2BackgroundMovie() )
	{
		HL2BackgroundMovie()->Update();
		if ( HL2BackgroundMovie()->SetTextureMaterial() != -1 )
#elif GAMEUI_SHARED
	if (m_bMovieEnabled && BackgroundMovie())
	{
		BackgroundMovie()->Update();
		if (BackgroundMovie()->SetTextureMaterial() != -1)
#endif
		{
			surface()->DrawSetColor( 255, 255, 255, 255 );

			int x, y, w, t;
			GetBounds( x, y, w, t );

			// center, 16:10 aspect ratio
			int width_at_ratio = t * (16.0f / 9.0f);
			x = ( w * 0.5f ) - ( width_at_ratio * 0.5f );
			
			surface()->DrawTexturedRect( x, y, x + width_at_ratio, y + t );
		}
	}

	// test of gradient header/footer
	
	int nScreenWidth = GetWide();
	int nScreenHeight = GetTall();
	int iHalfWide = nScreenWidth * 0.5f;
	int nBarHeight = YRES( 22 );

	//surface()->DrawSetColor( Color( 16, 32, 46, 230 ) );
	surface()->DrawSetColor( Color( 0, 0, 0, 230 ) );
	surface()->DrawFilledRect( 0, 0, nScreenWidth, nScreenHeight );

	if ( m_bHeaderEnabled )
	{
		surface()->DrawSetColor( Color( UI_STYLE_FOOTER_GRADIENT ) );
		surface()->DrawFilledRect( 0, 0, nScreenWidth, nBarHeight );

		surface()->DrawSetColor( Color( UI_STYLE_FOOTER_GRALINE ) );
		surface()->DrawFilledRectFade( iHalfWide, 0, iHalfWide + YRES( 320 ), nBarHeight, 255, 0, true );
		surface()->DrawFilledRectFade( iHalfWide - YRES( 320 ), 0, iHalfWide, nBarHeight, 0, 255, true );
	}

	if ( m_bFooterEnabled )
	{
		surface()->DrawSetColor( Color( UI_STYLE_FOOTER_GRADIENT ) );
		surface()->DrawFilledRect( 0, nScreenHeight - nBarHeight, nScreenWidth, nScreenHeight );

		surface()->DrawSetColor( Color( UI_STYLE_FOOTER_GRALINE ) );
		surface()->DrawFilledRectFade( iHalfWide, nScreenHeight - nBarHeight, iHalfWide + YRES( 320 ), nScreenHeight, 255, 0, true );
		surface()->DrawFilledRectFade( iHalfWide - YRES( 320 ), nScreenHeight - nBarHeight, iHalfWide, nScreenHeight, 0, 255, true );
	}
	
}

// =================

CNB_Gradient_Bar::CNB_Gradient_Bar( vgui::Panel *parent, const char *name ) : BaseClass( parent, name )
{
}

void CNB_Gradient_Bar::PaintBackground()
{
	int wide, tall;
	GetSize( wide, tall );

	int y = 0;
	int iHalfWide = wide * 0.5f;

	float flAlpha = 200.0f / 255.0f;

	// fill bar background
	vgui::surface()->DrawSetColor( Color( 0, 0, 0, 255 * flAlpha ) );
	vgui::surface()->DrawFilledRect( 0, y, wide, y + tall );

	vgui::surface()->DrawSetColor( Color( UI_STYLE_BACKGROUNDFILL * flAlpha ) );

	int nBarPosY = y + YRES( 4 );
	int nBarHeight = tall - YRES( 8 );
	vgui::surface()->DrawFilledRectFade( iHalfWide, nBarPosY, wide, nBarPosY + nBarHeight, 255, 0, true );
	vgui::surface()->DrawFilledRectFade( 0, nBarPosY, iHalfWide, nBarPosY + nBarHeight, 0, 255, true );
	// draw highlights
	nBarHeight = YRES( 2 );
	nBarPosY = y;
	vgui::surface()->DrawSetColor( Color( UI_STYLE_HIGHLIGHTS * flAlpha ) );
	vgui::surface()->DrawFilledRectFade( iHalfWide, nBarPosY, wide, nBarPosY + nBarHeight, 255, 0, true );
	vgui::surface()->DrawFilledRectFade( 0, nBarPosY, iHalfWide, nBarPosY + nBarHeight, 0, 255, true );

	nBarPosY = y + tall - YRES( 2 );
	vgui::surface()->DrawFilledRectFade( iHalfWide, nBarPosY, wide, nBarPosY + nBarHeight, 255, 0, true );
	vgui::surface()->DrawFilledRectFade( 0, nBarPosY, iHalfWide, nBarPosY + nBarHeight, 0, 255, true );
}
