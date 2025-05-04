#include "cbase.h"
#include "fr_shareddefs.h"

FRTeam_t CFRTeamLoader::FRTeamMap[] =
{
	{ TEAM_RED,				COLOR_RED },
	{ TEAM_BLUE,			COLOR_BLUE },
	{ TEAM_YELLOW,			COLOR_YELLOW },
	{ TEAM_GREEN,			COLOR_GREEN },
	{ TEAM_GREY,			COLOR_GREY },
	{ TEAM_WHITE,			COLOR_WHITE },
	{ TEAM_BLACK,			COLOR_BLACK },
	{ TEAM_PURPLE,			COLOR_PURPLE },
	//"yellow" is orange.....
	{ TEAM_ORANGE,			COLOR_YELLOW },
	{ TEAM_CYAN,			COLOR_CYAN },
	{ TEAM_TURQUOISE,		COLOR_TURQUOISE },
	{ TEAM_PINK,			COLOR_PINK },
	{ TEAM_MAGENTA,			COLOR_MAGENTA },
	{ TEAM_SMOD,			COLOR_FR_OLD },
	{ TEAM_FR,				COLOR_FR },
};

Color CFRTeamLoader::GetColorForTeam(int iTeamNumber)
{
	Color col = Color(255, 255, 255, 255);

	for (int i = 0; i < ARRAYSIZE(FRTeamMap); i++)
	{
		if (FRTeamMap[i].id == iTeamNumber)
		{
			col = FRTeamMap[i].col;
			break;
		}
	}

	return col;
}

void CFRTeamLoader::LoadColors(Color* colorArray)
{
	for (int i = 0; i < ARRAYSIZE(FRTeamMap); i++)
	{
		colorArray[FRTeamMap[i].id] = FRTeamMap[i].col;
	}
}