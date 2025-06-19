//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "PlaylistDialog.h"

#include <stdio.h>

using namespace vgui;

#include <vgui/ILocalize.h>

#include "filesystem.h"
#include <KeyValues.h>
#include <vgui_controls/ComboBox.h>
#include <vgui_controls/BitmapImagePanel.h>
#include "tier1/convar.h"

// for SRC
#include <vstdlib/random.h>

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CPlaylistDialog::CPlaylistDialog(vgui::Panel *parent) : BaseClass(NULL, "PlaylistDialog")
{
	SetSize(348, 460);
	//SetOKButtonText("#GameUI_Start");
	
	// we can use this if we decide we want to put "listen server" at the end of the game name
	m_pPlaylistList = new ComboBox(this, "PlayListList", 12, false);

	m_lcurrentPlaylist = new Label(this, "CurrentPlaylistPanel", "");

	LoadControlSettings("Resource/PlaylistDialog.res");

	// create KeyValues object to load/save config options

	LoadPlaylistList();
	DialogInit();
	SetSizeable(false);
	SetDeleteSelfOnClose(true);
	MoveToCenterOfScreen();
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CPlaylistDialog::~CPlaylistDialog()
{
}

void CPlaylistDialog::DialogInit()
{
	static ConVarRef snd_fmod_musicsystem_playlist("snd_fmod_musicsystem_playlist");
	const char* playermodel = snd_fmod_musicsystem_playlist.GetString();
	char szModelName[1024];
	Q_snprintf(szModelName, sizeof(szModelName), "Current: %s\n", playermodel);
	m_lcurrentPlaylist->SetText(szModelName);
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CPlaylistDialog::OnClose()
{
	BaseClass::OnClose();
	MarkForDeletion();
}

void CPlaylistDialog::OnOK()
{
	char szModelName[64];
	Q_strncpy(szModelName, GetPlaylistName(), sizeof(szModelName));

	char szModelCommand[1024];

	// create the command to execute
	Q_snprintf(szModelCommand, sizeof(szModelCommand), "snd_fmod_musicsystem_playlist scripts/playlists/%s;snd_fmod_musicsystem_reload\n", szModelName);

	// exec
	engine->ClientCmd_Unrestricted(szModelCommand);

	OnClose();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *command - 
//-----------------------------------------------------------------------------
void CPlaylistDialog::OnCommand(const char *command)
{
	if ( !stricmp( command, "Ok" ) )
	{
		OnOK();
		return;
	}

	BaseClass::OnCommand( command );
}

void CPlaylistDialog::OnKeyCodeTyped(KeyCode code)
{
	// force ourselves to be closed if the escape key it pressed
	if (code == KEY_ESCAPE)
	{
		Close();
	}
	else
	{
		BaseClass::OnKeyCodeTyped(code);
	}
}

//-----------------------------------------------------------------------------
// Purpose: loads the list of available maps into the map list
//-----------------------------------------------------------------------------
void CPlaylistDialog::LoadPlaylists()
{
	FileFindHandle_t findHandle = NULL;

	const char *pszFilename = g_pFullFileSystem->FindFirst("scripts/playlists/*.txt", &findHandle);
	while (pszFilename)
	{
		m_pPlaylistList->AddItem(pszFilename, new KeyValues("data", "listname", pszFilename));
		pszFilename = g_pFullFileSystem->FindNext(findHandle);
	}
	g_pFullFileSystem->FindClose(findHandle);
}

//-----------------------------------------------------------------------------
// Purpose: loads the list of available maps into the map list
//-----------------------------------------------------------------------------
void CPlaylistDialog::LoadPlaylistList()
{
	// clear the current list (if any)
	m_pPlaylistList->DeleteAllItems();

	// Load the GameDir maps
	LoadPlaylists();

	// set the first item to be selected
	m_pPlaylistList->ActivateItem( 0 );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char *CPlaylistDialog::GetPlaylistName()
{
	int count = m_pPlaylistList->GetItemCount();

	// if there is only one entry it's the special "select random map" entry
	if( count <= 1 )
		return NULL;

	const char *modelname = NULL;
	modelname = m_pPlaylistList->GetActiveItemUserData()->GetString("listname");

	return modelname;
}

//-----------------------------------------------------------------------------
// Purpose: Sets currently selected map in the map combobox
//-----------------------------------------------------------------------------
void CPlaylistDialog::SetPlaylist(const char *name)
{
	for (int i = 0; i < m_pPlaylistList->GetItemCount(); i++)
	{
		if (!m_pPlaylistList->IsItemIDValid(i))
			continue;

		if (!stricmp(m_pPlaylistList->GetItemUserData(i)->GetString("listname"), name))
		{
			m_pPlaylistList->ActivateItem(i);
			break;
		}
	}
}

CON_COMMAND(playlistdialog, "")
{
	CPlaylistDialog* pCPlaylistDialog = new CPlaylistDialog(NULL);
	pCPlaylistDialog->Activate();
}