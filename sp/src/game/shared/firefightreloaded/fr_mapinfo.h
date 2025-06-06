#ifndef FR_MAPINFO
#define FR_MAPINFO

#include "KeyValues.h"

#ifdef _WIN32
#pragma once
#endif

class CMapInfo
{
public:
#ifndef GAMEPADUI_DLL
	static KeyValues* GetMapInfoData();
#endif
	static KeyValues* GetMapInfoData(const char* pMapName);
};

#endif // FR_MAPINFO