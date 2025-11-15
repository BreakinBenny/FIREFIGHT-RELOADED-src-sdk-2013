#ifndef HUD_TASKLIST_H
#define HUD_TASKLIST_H

#ifdef _WIN32
#pragma once
#endif

#include "hudelement.h"
#include "hud_macros.h"
#include "iclientmode.h"
#include "view.h"
#include "tasks_shared.h"

using namespace vgui;

#include <vgui_controls/Panel.h>
#include <vgui_controls/Frame.h>
#include <vgui/IScheme.h>
#include <vgui/ISurface.h>
#include <vgui/ILocalize.h>

#include "c_baseplayer.h"

#define TASKLINE_NUM_FLASHES 8.0f
#define TASKLINE_FLASH_TIME 5.0f
#define TASKLINE_FADE_TIME 1.0f

class CHudTaskList : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CHudTaskList, vgui::Panel);

public:
	CHudTaskList(const char* pElementName);	//The constructor.
	void Init(void);	//This gets called when the element is created.
	void VidInit(void);	//Same as above, this may only gets called when you load up the map for the first time.
	void Reset();	//This gets called when you reset the HUD.
	void Paint(void);	//This gets called every frame, to display the element on the screen!
	// void OnThink( void );	//This gets called also almost every frame, so we can update things often.
	void MsgFunc_TaskList(bf_read& msg);

private:
	// CHudTexture *m_pIcon;		// Icon texture reference
	wchar_t		m_pText[TASKLIST_MAX_TASKS][256];	// Unicode text buffer
	int			m_iPriority[TASKLIST_MAX_TASKS];		// 0=inactive, 1=complete, 2=low, 3=medium, 4=high
	float		m_flStartTime[TASKLIST_MAX_TASKS];	// When the message was recevied
	float		m_flDuration[TASKLIST_MAX_TASKS];	// Duration of the message

protected:
	virtual void ApplySchemeSettings(vgui::IScheme* pScheme);

private:

	CPanelAnimationVar(vgui::HFont, m_hSmallFont, "TaskFont", "Default");
	CPanelAnimationVar(vgui::HFont, m_hLargeFont, "TitleFont", "Default");
	CPanelAnimationVarAliasType(float, text_xpos, "text_xpos", "8", "proportional_float");
};
#endif