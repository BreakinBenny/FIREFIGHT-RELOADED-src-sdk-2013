//========= Copyright Valve Corporation, All rights reserved. ============//
#ifndef C_MONSTER_RESOURCE
#define C_MONSTER_RESOURCE
#ifdef _WIN32
#pragma once
#endif


class C_MonsterResource : public C_BaseEntity
{
	DECLARE_CLASS( C_MonsterResource, C_BaseEntity );
public:
	DECLARE_CLIENTCLASS();

	C_MonsterResource();
	virtual ~C_MonsterResource();

	float GetBossHealthPercentage( void );

	int GetBossState() const { return m_iBossState; }
	const char* GetBossName() const { return m_szBossName; }

private:
	int m_iBossHealthPercentageByte;
	int m_iBossState;
	char m_szBossName[MAX_PATH];
};

extern C_MonsterResource *g_pMonsterResource;


#endif // C_MONSTER_RESOURCE
