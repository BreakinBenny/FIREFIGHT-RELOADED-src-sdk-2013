//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "iclientmode.h"
#include "ienginevgui.h"
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui_controls/Label.h>
#include "c_monster_resource.h"
#include "tf_hud_boss_health.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

DECLARE_HUDELEMENT_DEPTH(CHudBossHealthMeter, 100);

//-----------------------------------------------------------------------------
CHudBossHealthMeter::CHudBossHealthMeter( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "HudBossHealth" )
{
	Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	m_pHealthBarPanel = new vgui::EditablePanel( this, "HealthBarPanel" );

	SetHiddenBits( HIDEHUD_MISCSTATUS );

	vgui::ivgui()->AddTickSignal( GetVPanel() );
}

//-----------------------------------------------------------------------------
void CHudBossHealthMeter::ApplySchemeSettings( IScheme *pScheme )
{
	// load control settings...
	LoadControlSettings( "resource/UI/HudBossHealth.res" );

	//BarImage
	m_pBarImagePanel = dynamic_cast<ImagePanel*>( m_pHealthBarPanel->FindChildByName( "BarImage" ) );

	// BorderImage
	m_pBorderImagePanel = dynamic_cast<ImagePanel*>( FindChildByName( "BorderImage" ) );

	m_pEnemyName = dynamic_cast<Label*>(FindChildByName("BossLabel"));

	BaseClass::ApplySchemeSettings( pScheme );
}

//-----------------------------------------------------------------------------
bool CHudBossHealthMeter::ShouldDraw( void )
{
	CBasePlayer *pPlayer = CBasePlayer::GetLocalPlayer();
	if ( !pPlayer)
	{
		return false;
	}

	if ( CHudElement::ShouldDraw() && g_pMonsterResource )
	{
		return g_pMonsterResource->GetBossHealthPercentage() > 0.0f ? true : false;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Update HUD due to data changes
void CHudBossHealthMeter::Update( void )
{
	if ( g_pMonsterResource )
	{
		m_pHealthBarPanel->SetWide( m_nHealthBarWide );
		m_pBarImagePanel->SetWide(m_nHealthBarWide * g_pMonsterResource->GetBossHealthPercentage());

		int iState = g_pMonsterResource->GetBossState();
	
		if ( m_pBarImagePanel )
		{
			Color barColor = ( iState == FR_BOSS_SHIELDED ) ? m_shieldedColor : m_unshieldedColor;

			if (g_pMonsterResource->GetBossHealthPercentage() <= 0.25f && iState != FR_BOSS_SHIELDED)
			{
				barColor = m_dangerColor;
			}

			m_pBarImagePanel->SetDrawColor( barColor );
		}

		if (m_pEnemyName)
		{
			m_pEnemyName->SetText(g_pMonsterResource->GetBossName());

			// dirty hack to uppercase
			wchar_t wbuf[MAX_PATH];
			m_pEnemyName->GetText(wbuf, sizeof(wbuf));
			V_wcsupr(wbuf);
			m_pEnemyName->SetText(wbuf);
		}
	}
}