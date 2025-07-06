//========= Copyright Valve Corporation, All rights reserved. ============//
#ifndef HUD_BOSS_HEALTH_METER_H
#define HUD_BOSS_HEALTH_METER_H

#include "hud.h"
#include "hudelement.h"
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/Label.h>

class CHudBossHealthMeter : public CHudElement, public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE( CHudBossHealthMeter, vgui::EditablePanel );

public:
	CHudBossHealthMeter( const char *pElementName );

	virtual void	ApplySchemeSettings( vgui::IScheme *scheme );
	virtual bool	ShouldDraw( void );

	void Update( void );		// update HUD due to data changes

private:

	vgui::EditablePanel *m_pHealthBarPanel;

	vgui::ImagePanel *m_pBarImagePanel;

	vgui::ImagePanel *m_pBorderImagePanel;

	vgui::Label *m_pEnemyName;

	CPanelAnimationVar(Color, m_shieldedColor, "ShieldedColor", "255 0 255 255");
	CPanelAnimationVar(Color, m_unshieldedColor, "UnshieldedColor", "FgColor");
	CPanelAnimationVar(Color, m_dangerColor, "DangerColor", "Caution");

	CPanelAnimationVarAliasType( int, m_nHealthBarWide, "health_bar_wide", "168", "proportional_xpos" );
};

#endif // HUD_BOSS_HEALTH_METER_H
