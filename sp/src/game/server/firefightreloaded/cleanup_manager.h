#ifndef CLEANUP
#define CLEANUP
#include "cbase.h"
#include "utlvector.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar g_max_combine_mines;
extern ConVar g_max_gib_pieces;
extern ConVar g_max_thrown_knives;
extern ConVar g_ragdoll_maxcount;

class CCleanupManager : public CBaseEntity
{
	DECLARE_CLASS(CCleanupManager, CBaseEntity)
	DECLARE_DATADESC()

	CUtlVector<EHANDLE> m_CombineMines;
	CUtlVector<EHANDLE> m_Gibs;
	CUtlVector<EHANDLE> m_Ragdolls;
	CUtlVector<EHANDLE> m_ThrownKnives;
	CUtlVector<EHANDLE> m_Weapons;

	static CCleanupManager* pManager;
	static CCleanupManager* GetManager();

	typedef void cleanupFunc( EHANDLE );
	static void Add(CUtlVector<EHANDLE>& handles, EHANDLE handle, const ConVar& var, cleanupFunc& func );
	static bool Remove(CUtlVector<EHANDLE>& handles, EHANDLE handle );

public:
	static void AddCombineMine( EHANDLE mine );
	static void AddGib( EHANDLE gib );
	static void AddRagdoll( EHANDLE ragdoll );
	static void AddThrownKnife( EHANDLE knife );
	static void AddWeapon(EHANDLE weapon);

	static bool RemoveCombineMine( EHANDLE mine );
	static bool RemoveGib( EHANDLE gib );
	static bool RemoveRagdoll( EHANDLE ragdoll );
	static bool RemoveThrownKnife( EHANDLE knife );
	static bool RemoveWeapon(EHANDLE weapon);

	static void Shutdown();
};

#endif