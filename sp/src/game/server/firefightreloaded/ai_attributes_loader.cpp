#include "cbase.h"
#include "ai_attributes_loader.h"
#include "KeyValues.h"
#include "filesystem.h"
// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

ConVar entity_attributes_numpresets("entity_attributes_numpresets", "10", FCVAR_ARCHIVE);
ConVar entity_attributes_chance("entity_attributes_chance", "5", FCVAR_ARCHIVE);
ConVar entity_attributes("entity_attributes", "1", FCVAR_ARCHIVE);

CAttributesLoader *LoadRandomPresetFile(const char* className, bool noNag)
{
	random->SetSeed(gpGlobals->curtime);
	int randPreset = random->RandomInt(1, entity_attributes_numpresets.GetInt());
	CAttributesLoader* loader = new CAttributesLoader(className, randPreset, noNag);

	if (!loader->loadedAttributes)
	{
		bool shouldLoadFirstPreset = (random->RandomInt(0, entity_attributes_chance.GetInt()) == entity_attributes_chance.GetInt() ? true : false);
		
		if (shouldLoadFirstPreset)
		{
			//load the first preset.
			loader = new CAttributesLoader(className, 1, true);

			if (!loader->loadedAttributes)
			{
				if (!noNag)
				{
					Warning("CAttributesLoader: Cannot load random attribute preset %i for %s.\n", randPreset, className);
				}
				return NULL;
			}
		}
		else
		{
			if (!noNag)
			{
				Warning("CAttributesLoader: Cannot load random attribute preset %i for %s.\n", randPreset, className);
			}
			return NULL;
		}
	}

	if (!loader->GetBool("spawner_only"))
	{
		int randChance = random->RandomInt(1, entity_attributes_chance.GetInt());

		if (loader->GetBool("persist") || randChance == entity_attributes_chance.GetInt())
		{
			return loader;
		}
		else
		{
			loader->Die();
			return NULL;
		}
	}
	else
	{
		if (!noNag)
		{
			Warning("CAttributesLoader: Preset %i for %s can only be spawned-in manually. Disable \"spawner_only\" to allow this preset to spawn randomly.\n", randPreset, className);
		}
		loader->Die();
		return NULL;
	}
}

CAttributesLoader* LoadPresetFile(const char* className, int preset)
{
	CAttributesLoader* loader = new CAttributesLoader(className, preset);

	if (!loader->loadedAttributes)
	{
		return NULL;
	}

	return loader;
}

CAttributesLoader::CAttributesLoader(const char *className, int preset, bool noError)
{
	Init(className, preset, noError);
}

void CAttributesLoader::Init(const char *className, int preset, bool noError)
{
	if (!className || className == NULL || strlen(className) == 0)
	{
		if (!noError)
		{
			Warning("CAttributesLoader: Definition has no classname!\n");
		}
		return;
	}
	
	char szFullName[512];
	Q_snprintf( szFullName, sizeof( szFullName ), "scripts/entity_attributes/%s/preset%i.txt", className, preset );

	char szFullKVName[512];
	Q_snprintf(szFullKVName, sizeof(szFullKVName), "%s_preset%i", className, preset);
	
	KeyValues* pKV = new KeyValues(className);
	if (pKV->LoadFromFile(filesystem, szFullName, "GAME"))
	{
		data = pKV->MakeCopy();
		loadedAttributes = true;
	}
	else
	{
		if (!noError)
		{
			Warning("CAttributesLoader: Failed to load attributes for %s on preset %i!! File may not exist.\n", className, preset);
		}
		loadedAttributes = false;
	}
	
	pKV->deleteThis();
}

const char* CAttributesLoader::GetString(const char* szString, const char* defaultValue)
{
	return STRING(AllocPooledString(data->GetString(szString, defaultValue)));
}

int CAttributesLoader::GetInt(const char* szString, int defaultValue)
{
	return data->GetInt(szString, defaultValue);
}

float CAttributesLoader::GetFloat(const char* szString, float defaultValue)
{
	return data->GetFloat(szString, defaultValue);
}

bool CAttributesLoader::GetBool(const char* szString, bool defaultValue)
{
	return data->GetBool(szString, defaultValue);
}

Color CAttributesLoader::GetColor(const char* szString)
{
	return data->GetColor(szString);
}

Vector CAttributesLoader::GetVector(const char* szString, Vector defaultValue)
{
	return data->GetVector(szString, defaultValue);
}

void CAttributesLoader::SwitchEntityModel(CBaseEntity* ent, const char* szString, const char* defaultValue)
{
	const char* newModelName = GetString(szString, defaultValue);

	if (strlen(newModelName) > 0)
	{
		ent->SetModelName(AllocPooledString(newModelName));
		CBaseEntity::PrecacheModel(STRING(ent->GetModelName()));
		ent->SetModel(STRING(ent->GetModelName()));
	}
}

void CAttributesLoader::SwitchEntityColor(CBaseEntity* ent, const char* szString)
{
	Color newColor = GetColor(szString);
	if (newColor.GetRawColor() != 0)
	{
		ent->SetRenderColor(newColor.r(), newColor.g(), newColor.b(), newColor.a());
	}
}

void CAttributesLoader::SwitchEntityBodygroup(CBaseAnimating* ent, const char* szNum, const char* szVal)
{
	int num, val;
	num = GetInt(szNum);
	val = GetInt(szVal);
	ent->SetBodygroup(num, val);
}

void CAttributesLoader::SwitchEntitySkin(CBaseAnimating* ent, const char* szVal)
{
	int val;
	val = GetInt(szVal);
	ent->m_nSkin = val;
}

void CAttributesLoader::Die()
{
	if (data)
	{
		data->deleteThis();
	}
}