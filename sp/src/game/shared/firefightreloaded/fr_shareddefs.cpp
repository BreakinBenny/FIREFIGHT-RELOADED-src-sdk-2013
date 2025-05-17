#include "cbase.h"
#include "fr_shareddefs.h"
#include "filesystem.h"
#include "gamestringpool.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static ConVar fr_kvloader_debug("fr_kvloader_debug", "0");

FRTeam_t CFRTeamLoader::FRTeamMap[] =
{
	//FR COLORS
	{ TEAM_RED,				COLOR_RED, "RED" },
	{ TEAM_BLUE,			COLOR_BLUE, "BLUE" },
	{ TEAM_YELLOW,			COLOR_YELLOW, "YELLOW" },
	{ TEAM_GREEN,			COLOR_GREEN, "GREEN"},
	{ TEAM_GREY,			COLOR_GREY, "GREY"},
	{ TEAM_WHITE,			COLOR_WHITE, "WHITE"},
	{ TEAM_BLACK,			COLOR_BLACK, "BLACK"},
	{ TEAM_PURPLE,			COLOR_PURPLE, "PURPLE"},
	//"yellow" is orange.....
	{ TEAM_ORANGE,			COLOR_YELLOW, "YELLOW"},
	{ TEAM_CYAN,			COLOR_CYAN, "CYAN"},
	{ TEAM_TURQUOISE,		COLOR_TURQUOISE, "TURQUOISE"},
	{ TEAM_PINK,			COLOR_PINK, "PINK"},
	{ TEAM_MAGENTA,			COLOR_MAGENTA, "MAGENTA"},
	//FR FUN COLORS
	{ TEAM_SMOD,			COLOR_FR_OLD, "SMOD_BLUE" },
	{ TEAM_FR,				COLOR_FR, "FR_BLUE" },
	//tf2 quality/skin colors
	//items
	{ TEAM_UNIQUE,			COLOR_UNIQUE, "TF_UNIQUE_QUALITY" },
	{ TEAM_VINTAGE,			COLOR_VINTAGE, "TF_VINTAGE_QUALITY" },
	{ TEAM_GENUINE,			COLOR_GENUINE, "TF_GENUINE_QUALITY" },
	{ TEAM_STRANGE,			COLOR_STRANGE, "TF_STRANGE_QUALITY" },
	{ TEAM_UNUSUAL,			COLOR_UNUSUAL, "TF_UNUSUAL_QUALITY" },
	{ TEAM_HAUNTED,			COLOR_HAUNTED, "TF_HAUNTED_QUALITY" },
	{ TEAM_COLLECTORS,		COLOR_COLLECTORS, "TF_COLLECTORS_QUALITY" },
	{ TEAM_COMMUNITY,		COLOR_COMMUNITY, "TF_COMMUNITY_QUALITY" },
	{ TEAM_VALVE,			COLOR_VALVE, "TF_VALVE_QUALITY" },
	//paints
	{ TEAM_CIVILIAN,		COLOR_CIVILIAN, "TF_CIVILIAN_GRADE" },
	{ TEAM_FREELANCE,		COLOR_FREELANCE, "TF_FREELANCE_GRADE" },
	{ TEAM_MERCENARY,		COLOR_MERCENARY, "TF_MERCENARY_GRADE" },
	{ TEAM_COMMANDO,		COLOR_COMMANDO, "TF_COMMANDO_GRADE" },
	{ TEAM_ASSASSIN,		COLOR_ASSASSIN, "TF_ASSASSIN_GRADE" },
	{ TEAM_ELITE,			COLOR_ELITE, "TF_ELITE_GRADE" },
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

const char* CFRTeamLoader::GetNameForTeam(int iTeamNumber)
{
	const char* name = "NONE";

	for (int i = 0; i < ARRAYSIZE(FRTeamMap); i++)
	{
		if (FRTeamMap[i].id == iTeamNumber)
		{
			name = FRTeamMap[i].name;
			break;
		}
	}

	return name;
}

void CFRTeamLoader::LoadColors(Color* colorArray)
{
	for (int i = 0; i < ARRAYSIZE(FRTeamMap); i++)
	{
		colorArray[FRTeamMap[i].id] = FRTeamMap[i].col;
	}
}

void FRKeyValuesLoader::LoadEntries(const char* fileName, const char* kvHeader)
{
	m_storedVector.RemoveAll();

	KeyValues* pKV = new KeyValues(kvHeader);
	if (pKV->LoadFromFile(filesystem, fileName, "GAME"))
	{
		FOR_EACH_VALUE(pKV, pSubData)
		{
			if (FStrEq(pSubData->GetString(), ""))
				continue;

			string_t iName = AllocPooledString(pSubData->GetString());
			if (m_storedVector.Find(iName) == m_storedVector.InvalidIndex())
				m_storedVector[m_storedVector.AddToTail()] = iName;
		}
	}

	if (fr_kvloader_debug.GetBool())
	{
		KeyValuesDumpAsDevMsg(pKV, 1);
	}

	pKV->deleteThis();
}

bool FRKeyValuesLoader::FindEntry(string_t query)
{
	bool result = false;

	FOR_EACH_VEC(m_storedVector, i)
	{
		string_t iszName = m_storedVector[i];
		const char* pszName = STRING(iszName);
		const char* pszQuery = STRING(query);

		bool IsMatching = (!strcmp(pszName, pszQuery) ? true : false);
		if (IsMatching)
		{
			result = true;
			break;
		}
	}

	return result;
}

string_t FRKeyValuesLoader::GrabRandomEntryString(void)
{
	string_t result = m_storedVector.Random();
	return result;
}

KeyValues* CMapInfo::GetMapInfoData()
{
	char mapname[256];
#if !defined( CLIENT_DLL )
	Q_snprintf(mapname, sizeof(mapname), "%s", STRING(gpGlobals->mapname));
#else
	const char* levelname = engine->GetLevelName();

	const char* str = Q_strstr(levelname, "maps");
	if (str)
	{
		Q_strncpy(mapname, str + 5, sizeof(mapname) - 1);	// maps + \\ = 5
	}
	else
	{
		Q_strncpy(mapname, levelname, sizeof(mapname) - 1);
	}

	char* ext = Q_strstr(mapname, ".bsp");
	if (ext)
	{
		*ext = 0;
	}
#endif

	char szFullName[512];
	Q_snprintf(szFullName, sizeof(szFullName), "maps/%s_mapinfo.txt", mapname);

	char szFullKVName[512];
	Q_snprintf(szFullKVName, sizeof(szFullKVName), "%s", mapname);

	KeyValues* pKV = new KeyValues(mapname);
	if (pKV->LoadFromFile(filesystem, szFullName))
	{
		return pKV;
	}

	return NULL;
}
