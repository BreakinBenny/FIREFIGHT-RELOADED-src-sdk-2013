#include "cbase.h"
#include "fr_shareddefs.h"
#include "filesystem.h"
#include "gamestringpool.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static ConVar fr_kvloader_debug("fr_kvloader_debug", "0");

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
