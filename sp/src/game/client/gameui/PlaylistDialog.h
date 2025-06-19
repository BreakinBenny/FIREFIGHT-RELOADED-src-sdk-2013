//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef SPAWNLISTDIALOG_H
#define SPAWNLISTDIALOG_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Frame.h>

//-----------------------------------------------------------------------------
// Purpose: dialog for launching a listenserver
//-----------------------------------------------------------------------------
class CPlaylistDialog : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE( CPlaylistDialog,  vgui::Frame );

public:
	CPlaylistDialog(vgui::Panel *parent);
	~CPlaylistDialog();
	
	// returns currently entered information about the server
	void SetPlaylist(const char *name);
	const char *GetPlaylistName();
	void DialogInit();

private:
	virtual void OnCommand( const char *command );
	virtual void OnClose();
	virtual void OnOK();
	virtual void OnKeyCodeTyped(vgui::KeyCode code);
	
	void LoadPlaylistList();
	void LoadPlaylists();

	vgui::ComboBox *m_pPlaylistList;
	vgui::Label *m_lcurrentPlaylist;
};


#endif
