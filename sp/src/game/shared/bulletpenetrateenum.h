#ifndef BULLETPENETRATEENUM_H
#define BULLETPENETRATEENUM_H

#ifdef _WIN32
#pragma once
#endif

#include "const.h"

//=============================================================================
//
// Shared player code that isn't CTFPlayerShared
//
//-----------------------------------------------------------------------------
struct penetrated_target_list
{
	CBaseEntity* pTarget;
	float flDistanceFraction;
};

//-----------------------------------------------------------------------------
class CBulletPenetrateEnum : public IEntityEnumerator
{
public:
	CBulletPenetrateEnum(Vector vecStart, Vector vecEnd, CBaseEntity* pShooter)
	{
		m_vecStart = vecStart;
		m_vecEnd = vecEnd;
		m_pShooter = pShooter;
	}

	// We need to sort the penetrated targets into order, with the closest target first
	class PenetratedTargetLess
	{
	public:
		bool Less(const penetrated_target_list& src1, const penetrated_target_list& src2, void* pCtx)
		{
			return src1.flDistanceFraction < src2.flDistanceFraction;
		}
	};

	virtual bool EnumEntity(IHandleEntity* pHandleEntity)
	{
		trace_t tr;

		CBaseEntity* pEnt = static_cast<CBaseEntity*>(pHandleEntity);

		// Ignore collisions with the shooter
		if (pEnt == m_pShooter)
			return true;

		if (ToBaseCombatCharacter(pEnt))
		{
			Ray_t ray;
			ray.Init(m_vecStart, m_vecEnd);
			enginetrace->ClipRayToEntity(ray, MASK_SOLID | CONTENTS_HITBOX, pHandleEntity, &tr);

			if (tr.fraction < 1.0f)
			{
				penetrated_target_list newEntry;
				newEntry.pTarget = pEnt;
				newEntry.flDistanceFraction = tr.fraction;
				m_Targets.Insert(newEntry);
				return true;
			}
		}

		return true;
	}

public:
	Vector		 m_vecStart;
	Vector		 m_vecEnd;
	CBaseEntity* m_pShooter;
	CUtlSortVector<penetrated_target_list, PenetratedTargetLess> m_Targets;
};

#endif // BULLETPENETRATEENUM_H