#include "cbase.h"
#include "game/client/iviewport.h"
#include "tier3/tier3.h"
#include "vgui/ILocalize.h"
#include "fmtstr.h"
#include "filesystem.h"
#include "animation.h"
#if !defined(MOD_VER) && !defined( _X360 ) && !defined( NO_STEAM )
#include "steam/steam_api.h"
#endif

#include "vgui_controls/MessageBox.h"

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

CON_COMMAND( list_centities, "List all client entities." )
{
	for ( auto iter = ClientEntityList().FirstHandle(); iter.IsValid(); iter = ClientEntityList().NextHandle( iter ) )
		ConMsg( "%d: %s\n", iter.GetEntryIndex(), ((C_BaseEntity*)iter.Get())->GetClassname() );
}

CON_COMMAND( report_centities, "Report all client entities." )
{
	CUtlStringMap<int> classnames_count;
	CUtlVector<CUtlString> sorted_classnames;

	for ( auto iter = ClientEntityList().FirstBaseEntity(); iter != nullptr; iter = ClientEntityList().NextBaseEntity( iter ))
	{
		auto classname = iter->GetClassname();
		if ( classnames_count.Defined( classname ) )
			++classnames_count[classname];
		else
		{
			sorted_classnames.AddToTail( classname );
			classnames_count[classname] = 1;
		}
	}

	sorted_classnames.Sort(CUtlString::SortCaseSensitive);
	for ( auto iter : sorted_classnames )
		ConMsg( "Class: %s (%d)\n", (const char*)iter, classnames_count[iter] );
}

CON_COMMAND(showworkshop, "")
{
#if !defined(MOD_VER) && !defined( _X360 ) && !defined( NO_STEAM )
	if (steamapicontext && steamapicontext->SteamFriends())
	{
		char szWorkshopURL[1024];
		Q_snprintf(szWorkshopURL, sizeof(szWorkshopURL), "https://steamcommunity.com/app/%i/workshop/\n", engine->GetAppID());
		steamapicontext->SteamFriends()->ActivateGameOverlayToWebPage(szWorkshopURL);
	}
#endif
}

CON_COMMAND(fr_version, "")
{
	char verString[1024];
	if (g_pFullFileSystem->FileExists("version.txt"))
	{
		FileHandle_t fh = filesystem->Open("version.txt", "r", "MOD");
		int file_len = filesystem->Size(fh);
		char* GameInfo = new char[file_len + 1];

		filesystem->Read((void*)GameInfo, file_len, fh);
		GameInfo[file_len] = 0; // null terminator

		filesystem->Close(fh);

		Q_snprintf(verString, sizeof(verString), "Game Version: v%s (%s)", GameInfo + 8, __DATE__);

		delete[] GameInfo;
	}
	
	Msg(verString);
}

#ifdef _DEBUG
CON_COMMAND(leaksun, "Sets fullbright to 1 and spams \"THE SUN IS LEAKING\" in the console repeatedly.")
{
	engine->ClientCmd("mat_fullbright 1\n");
	Warning("THE SUN IS LEAKING.\nTHE SUN IS LEAKING.\nTHE SUN IS LEAKING.\nTHE SUN IS LEAKING.\nTHE SUN IS LEAKING.\nTHE SUN IS LEAKING.\nTHE SUN IS LEAKING.\nTHE SUN IS LEAKING.\nTHE SUN IS LEAKING.\nTHE SUN IS LEAKING.\nTHE SUN IS LEAKING.\n\n\n\n\n\n\nRestore the lighting with mat_fullbright 0\n");
}

void OpenMessageBox(const CCommand& args)
{
	vgui::MessageBox* pMessageBox = new vgui::MessageBox(args[1], args[2]);
	pMessageBox->DoModal();
}

ConCommand show_messagebox("show_messagebox", OpenMessageBox);

void OpenErrorBox(const CCommand& args)
{
	Error(args[1]);
}

ConCommand show_error("show_error", OpenErrorBox);
#endif