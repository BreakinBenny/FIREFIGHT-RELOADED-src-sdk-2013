//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

#ifndef WEAPON_KATANA_H
#define WEAPON_KATANA_H

#include "basebludgeonweapon.h"
#include "fr_shareddefs.h"

#if defined( _WIN32 )
#pragma once
#endif

#ifdef HL2MP
#error weapon_katana.h must not be included in hl2mp. The windows compiler will use the wrong class elsewhere if it is.
#endif

#define	KATANA_RANGE	165.0f
#define	KATANA_REFIRE	0.75f

extern ConVar sv_katana_healthbonus_maxmultiplier;

//-----------------------------------------------------------------------------
// CWeaponKatana
//-----------------------------------------------------------------------------

class CWeaponKatana : public CBaseHLBludgeonWeapon
{
public:
	DECLARE_CLASS( CWeaponKatana, CBaseHLBludgeonWeapon );

	DECLARE_SERVERCLASS();
	DECLARE_ACTTABLE();
	DECLARE_DATADESC();

	CWeaponKatana();

	float		GetRange( void )		{	return	KATANA_RANGE;	}
	float		GetFireRate( void )		{	return	KATANA_REFIRE;	}

	void		AddViewKick( void );

	void		Equip(CBaseCombatCharacter* pOwner);

	void		PrimaryAttack(void);
	void		SecondaryAttack(void);
	bool		CanHolster(void);
	bool		Holster(CBaseCombatWeapon* pSwitchingTo);
	void		ItemPostFrame(void);
	void		ImpactEffect(trace_t& traceHit);
	float		GetDamageForActivity(Activity hitActivity) { return 0; };

	int			GetKillMultiplier() { return m_iKillMultiplier; }
	bool		IsKillMultiplierEnabled() { return (g_pGameRules->isInBullettime && m_bKillMultiplier); }
	
private:
	int			m_iKillMultiplier;
	int			m_iKills;
	float		m_flLastKill;
	bool		m_bKillMultiplier;

	FRKeyValuesLoader m_kvEnemyResist;
};

#endif // WEAPON_KATANA_H
