"GameMenu"
{
	"1"
	{
		"label" "#GameUI_GameMenu_ResumeGame"
		"command" "ResumeGame"
		"OnlyInGame" "1"
	}
	"2"
	{
		"label" "#GameUI_GameMenu_NewGame"
		"command" "engine createnewgame"
		"notmulti" "1"
	}
	"3"
	{
		"label" "#GameUI_GameMenu_SaveGame"
		"command" "OpenSaveGameDialog"
		"notmulti" "1"
		"OnlyInGame" "1"
	}
	"4"
	{
		"label" "#GameUI_GameMenu_LoadGame"
		"command" "OpenLoadGameDialog"
		"notmulti" "1"
	}
	"5"
	{
		"label" "#GameUI_GameMenu_AdvancedOptions"
		"command" "engine singleplayeroptions"
	}
	"6"
	{
		"label" "#GameUI_GameMenu_Options"
		"command" "OpenOptionsDialog"
	}
	"7"
	{
		"label" "#FRMP_PlayerModel_Title"
		"command" "engine playermodeldialog"
	}
	"8"
	{
		"label" "#FR_Spawnlist_Title"
		"command" "engine spawnlistdialog"
	}
	"9"
	{
		"label" "#FR_Playlist_Title"
		"command" "engine gamepadui_openplaylistchooser"
	}
	"10"
	{
		"label" "#FIREFIGHTRELOADED_ModMenu_Title"
		"command" "engine showworkshop"
	}
	"11"
	{
		"label" "#GameUI_GameMenu_Achievements"
		"command" "OpenAchievementsDialog"
	}
	"12"
	{
		"label" "#GameUI_GameMenu_MainMenu"
		"command" "Disconnect"
		"OnlyInGame" "1"
	}
	"13"
	{
		"label" "#GameUI_GameMenu_Quit"
		"command" "Quit"
	}
}