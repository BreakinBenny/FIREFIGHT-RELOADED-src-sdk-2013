"MainMenu"
{	
	"ResumeGame"
	{
		"text"			"#GameUI_GameMenu_ResumeGame"
		"command"		"cmd gamemenucommand resumegame"
		"priority"		"12"
		"family"		"ingame"
	}
	
	"NewGame"
	{
		"text"			"#GameUI_GameMenu_NewGame"
		"command"		"cmd gamepadui_openmapchooser"
		"command_alt"	"cmd createnewgame"
		"priority"		"11"
		"family"		"all"
	}
	
	"SaveGame"
	{
		"text"			"#GameUI_GameMenu_SaveGame"
		"command"		"cmd gamepadui_opensavegamedialog"
		"command_alt"	"cmd gamemenucommand opensavegamedialog"
		"priority"		"10"
		"family"		"ingame"
	}

	"LoadGame"
	{
		"text"			"#GameUI_GameMenu_LoadGame"
		"command"		"cmd gamepadui_openloadgamedialog"
		"command_alt"	"cmd gamemenucommand OpenLoadGameDialog"
		"priority"		"9"
		"family"		"all"
	}

	"Options"
	{
		"text"			"#GameUI_GameMenu_Options"
		"command"		"cmd gamepadui_openoptionsdialog"
		"command_alt"	"cmd gamemenucommand OpenOptionsDialog"
		"priority"		"8"
		"family"		"all"
	}

	"ModelOptions"
	{
		"text"			"#FRMP_PlayerModel_Title"
		"command"		"cmd gamepadui_openmodelchooser"
		"command_alt"	"cmd playermodeldialog"
		"priority"		"7"
		"family"		"all"
	}

	"SpawnlistOptions"
	{
		"text"			"#FR_Spawnlist_Title"
		"command"		"cmd gamepadui_openspawnlistchooser"
		"command_alt"	"cmd spawnlistdialog"
		"priority"		"6"
		"family"		"all"
	}
    
    "PlaylistOptions"
	{
		"text"			"#FR_Playlist_Title"
		"command"		"cmd gamepadui_openplaylistchooser"
		"command_alt"	"cmd playlistdialog"
		"priority"		"5"
		"family"		"all"
	}

	"OpenWorkshop"
	{
		"text"			"#FIREFIGHTRELOADED_ModMenu_Title"
		"command"		"cmd showworkshop"
		"priority"		"4"
		"family"		"all"
	}
	
	"Achievements"
	{
		"text"			"#GameUI_GameMenu_Achievements"
		"command"		"cmd gamepadui_openachievementsdialog"
		"command_alt"	"cmd gamemenucommand OpenAchievementsDialog"
		"priority"		"3"
		"family"		"all"
	}
    
    "BackToMain"
    {
    	"text"			"#GameUI_GameMenu_MainMenu"
		"command"		"cmd disconnect"
		"priority"		"2"
		"family"		"ingame"
    }

	"Quit"
	{
		"text"			"#GameUI_GameMenu_Quit"
		"command"		"cmd gamepadui_openquitgamedialog"
		"command_alt"	"cmd gamemenucommand Quit"
		"priority"		"1"
		"family"		"all"
	}
}