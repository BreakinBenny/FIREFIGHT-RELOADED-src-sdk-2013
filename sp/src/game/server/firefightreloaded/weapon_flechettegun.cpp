//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "basehlcombatweapon.h"
#include "npcevent.h"
#include "basecombatcharacter.h"
#include "ai_basenpc.h"
#include "player.h"
#include "game.h"
#include "in_buttons.h"
#include "ai_memory.h"
#include "soundent.h"
#include "rumble_shared.h"
#include "gamestats.h"
#include "npc_hunter.h"
#include "te_effect_dispatch.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"	

extern ConVar hunter_flechette_delay;
extern ConVar hunter_first_flechette_delay;

class CWeaponFlechetteGun : public CHLMachineGun
{
	DECLARE_CLASS(CWeaponFlechetteGun, CHLMachineGun);
public:

	CWeaponFlechetteGun(void);
	
	DECLARE_SERVERCLASS();
	
	void	PrimaryAttack(void);
	void	ItemPostFrame(void);
	int		GetMinBurst() { return 5; }
	int		GetMaxBurst() { return 8; }
	void	Precache(void);
	int		CapabilitiesGet(void) { return bits_CAP_WEAPON_RANGE_ATTACK1; }
	void	FireNPCPrimaryAttack(CBaseCombatCharacter* pOperator, Vector& vecShootOrigin, Vector& vecShootDir);
	void	Operator_HandleAnimEvent(animevent_t *pEvent, CBaseCombatCharacter *pOperator);
	void	Operator_ForceNPCFire(CBaseCombatCharacter* pOperator, bool bSecondary);
	bool	Reload(void);
	float	GetFireRate(void) 
	{ 
		if (GetOwner()->MyNPCPointer())
		{
			return 0.05;
		}

		if (m_nShotsFired <= 0)
		{
			return hunter_first_flechette_delay.GetFloat();
		}

		return hunter_flechette_delay.GetFloat();
	}

	int WeaponRangeAttack1Condition(float flDot, float flDist);

	float	WeaponAutoAimScale()	{ return 0.6f; }

	virtual const Vector& GetBulletSpread(void)
	{
		static Vector cone = VECTOR_CONE_4DEGREES;
		return cone;
	}

	const WeaponProficiencyInfo_t* GetProficiencyValues();
    
    bool	Deploy(void);
	
protected:
	int				m_nShotsFired;
	
	DECLARE_DATADESC();
	DECLARE_ACTTABLE();
};

IMPLEMENT_SERVERCLASS_ST(CWeaponFlechetteGun, DT_WeaponFlechetteGun)
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS( weapon_flechettegun, CWeaponFlechetteGun );
PRECACHE_WEAPON_REGISTER(weapon_flechettegun);

BEGIN_DATADESC( CWeaponFlechetteGun )
DEFINE_FIELD(m_nShotsFired, FIELD_INTEGER),
END_DATADESC()

acttable_t	CWeaponFlechetteGun::m_acttable[] =
{
	{ ACT_RANGE_ATTACK1,			ACT_RANGE_ATTACK_AR2,			true },
	{ ACT_RELOAD,					ACT_RELOAD_SMG1,				true },		// FIXME: hook to AR2 unique
	{ ACT_IDLE,						ACT_IDLE_SMG1,					true },		// FIXME: hook to AR2 unique
	{ ACT_IDLE_ANGRY,				ACT_IDLE_ANGRY_SMG1,			true },		// FIXME: hook to AR2 unique

	{ ACT_WALK,						ACT_WALK_RIFLE,					true },

// Readiness activities (not aiming)
	{ ACT_IDLE_RELAXED,				ACT_IDLE_SMG1_RELAXED,			false },//never aims
	{ ACT_IDLE_STIMULATED,			ACT_IDLE_SMG1_STIMULATED,		false },
	{ ACT_IDLE_AGITATED,			ACT_IDLE_ANGRY_SMG1,			false },//always aims

	{ ACT_WALK_RELAXED,				ACT_WALK_RIFLE_RELAXED,			false },//never aims
	{ ACT_WALK_STIMULATED,			ACT_WALK_RIFLE_STIMULATED,		false },
	{ ACT_WALK_AGITATED,			ACT_WALK_AIM_RIFLE,				false },//always aims

	{ ACT_RUN_RELAXED,				ACT_RUN_RIFLE_RELAXED,			false },//never aims
	{ ACT_RUN_STIMULATED,			ACT_RUN_RIFLE_STIMULATED,		false },
	{ ACT_RUN_AGITATED,				ACT_RUN_AIM_RIFLE,				false },//always aims

// Readiness activities (aiming)
	{ ACT_IDLE_AIM_RELAXED,			ACT_IDLE_SMG1_RELAXED,			false },//never aims	
	{ ACT_IDLE_AIM_STIMULATED,		ACT_IDLE_AIM_RIFLE_STIMULATED,	false },
	{ ACT_IDLE_AIM_AGITATED,		ACT_IDLE_ANGRY_SMG1,			false },//always aims

	{ ACT_WALK_AIM_RELAXED,			ACT_WALK_RIFLE_RELAXED,			false },//never aims
	{ ACT_WALK_AIM_STIMULATED,		ACT_WALK_AIM_RIFLE_STIMULATED,	false },
	{ ACT_WALK_AIM_AGITATED,		ACT_WALK_AIM_RIFLE,				false },//always aims

	{ ACT_RUN_AIM_RELAXED,			ACT_RUN_RIFLE_RELAXED,			false },//never aims
	{ ACT_RUN_AIM_STIMULATED,		ACT_RUN_AIM_RIFLE_STIMULATED,	false },
	{ ACT_RUN_AIM_AGITATED,			ACT_RUN_AIM_RIFLE,				false },//always aims
//End readiness activities

	{ ACT_WALK_AIM,					ACT_WALK_AIM_RIFLE,				true },
	{ ACT_WALK_CROUCH,				ACT_WALK_CROUCH_RIFLE,			true },
	{ ACT_WALK_CROUCH_AIM,			ACT_WALK_CROUCH_AIM_RIFLE,		true },
	{ ACT_RUN,						ACT_RUN_RIFLE,					true },
	{ ACT_RUN_AIM,					ACT_RUN_AIM_RIFLE,				true },
	{ ACT_RUN_CROUCH,				ACT_RUN_CROUCH_RIFLE,			true },
	{ ACT_RUN_CROUCH_AIM,			ACT_RUN_CROUCH_AIM_RIFLE,		true },
	{ ACT_GESTURE_RANGE_ATTACK1,	ACT_GESTURE_RANGE_ATTACK_AR2,	false },
	{ ACT_COVER_LOW,				ACT_COVER_SMG1_LOW,				false },		// FIXME: hook to AR2 unique
	{ ACT_RANGE_AIM_LOW,			ACT_RANGE_AIM_AR2_LOW,			false },
	{ ACT_RANGE_ATTACK1_LOW,		ACT_RANGE_ATTACK_SMG1_LOW,		true },		// FIXME: hook to AR2 unique
	{ ACT_RELOAD_LOW,				ACT_RELOAD_SMG1_LOW,			false },
	{ ACT_GESTURE_RELOAD,			ACT_GESTURE_RELOAD_SMG1,		true },
//	{ ACT_RANGE_ATTACK2, ACT_RANGE_ATTACK_AR2_GRENADE, true },
	{ ACT_HL2MP_IDLE,				ACT_HL2AC_IDLE_LASER,				false },
	{ ACT_HL2MP_RUN,				ACT_HL2AC_RUN_LASER,				false },
	{ ACT_HL2MP_IDLE_CROUCH,		ACT_HL2AC_CROUCH_LASER,		false },
	{ ACT_HL2MP_WALK_CROUCH,		ACT_HL2AC_WALK_CROUCH_LASER,		false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,			ACT_HL2AC_GESTURE_RANGE_ATTACK_LASER,			false },
	{ ACT_HL2MP_GESTURE_RELOAD,		ACT_HL2AC_GESTURE_RELOAD_LASER,		false },
	{ ACT_HL2MP_JUMP,				ACT_HL2AC_JUMP_LASER,				false },
	{ ACT_RANGE_ATTACK1,			ACT_RANGE_ATTACK_AR2,			false },
};

IMPLEMENT_ACTTABLE(CWeaponFlechetteGun);

//=========================================================
CWeaponFlechetteGun::CWeaponFlechetteGun( )
{
	m_fMinRange1		= 0;// No minimum range. 
	m_fMaxRange1		= 6400;
	
	m_nShotsFired = 0;

	m_bFiresUnderwater = false;
}

bool CWeaponFlechetteGun::Deploy(void)
{
	m_nShotsFired = 0;

	return BaseClass::Deploy();
}

void CWeaponFlechetteGun::Precache(void)
{
	PrecacheParticleSystem("hunter_muzzle_flash");

	UTIL_PrecacheOther("hunter_flechette");
	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponFlechetteGun::Operator_HandleAnimEvent(animevent_t *pEvent, CBaseCombatCharacter *pOperator)
{
	switch (pEvent->event)
	{
	case EVENT_WEAPON_AR2:
	{
		Vector vecShootOrigin, vecShootDir;
		QAngle	angShootDir;

		CAI_BaseNPC* npc = pOperator->MyNPCPointer();
		ASSERT(npc != NULL);

		vecShootOrigin = pOperator->Weapon_ShootPosition();
		vecShootDir = npc->GetActualShootTrajectory(vecShootOrigin);

		FireNPCPrimaryAttack(pOperator, vecShootOrigin, vecShootDir);
	}
	break;

	default:
		BaseClass::Operator_HandleAnimEvent(pEvent, pOperator);
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponFlechetteGun::FireNPCPrimaryAttack(CBaseCombatCharacter* pOperator, Vector& vecShootOrigin, Vector& vecShootDir)
{
	// FIXME: use the returned number of bullets to account for >10hz firerate
	WeaponSoundRealtime(SINGLE);

	CSoundEnt::InsertSound(SOUND_COMBAT | SOUND_CONTEXT_GUNFIRE, pOperator->GetAbsOrigin(), SOUNDENT_VOLUME_MACHINEGUN, 0.2, pOperator, SOUNDENT_CHANNEL_WEAPON, pOperator->GetEnemy());

	Vector vecUp, vecRight;
	VectorVectors(vecShootDir, vecRight, vecUp);

	float x, y, z;

	//Gassian spread
	do {
		x = random->RandomFloat(-0.5, 0.5) + random->RandomFloat(-0.5, 0.5);
		y = random->RandomFloat(-0.5, 0.5) + random->RandomFloat(-0.5, 0.5);
		z = x * x + y * y;
	} while (z > 1);

	vecShootDir = vecShootDir + x * GetBulletSpread().x * vecRight + y * GetBulletSpread().y * vecUp;

	QAngle angAiming;
	VectorAngles(vecShootDir, angAiming);

	FlechetteCreateAutoShoot(vecShootOrigin, vecShootDir, angAiming, pOperator);

	if (GetWpnData().m_bUseMuzzleSmoke)
	{
		if (g_fr_npc_muzzlesmoke.GetBool())
		{
			DispatchParticleEffect("weapon_muzzle_smoke", PATTACH_POINT_FOLLOW, this, "muzzle");
		}
	}

	DispatchParticleEffect("hunter_muzzle_flash", PATTACH_POINT_FOLLOW, this, "muzzle");

	// Dispatch the elight	
	CEffectData data;
	data.m_nAttachmentIndex = LookupAttachment("muzzle");
	data.m_nEntIndex = entindex();
	DispatchEffect("HunterMuzzleFlash", data);

	m_iClip1 = m_iClip1 - 1;
}

bool CWeaponFlechetteGun::Reload(void)
{
	return DefaultReload(GetMaxClip1(), GetMaxClip2(), ACT_VM_RELOAD);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponFlechetteGun::Operator_ForceNPCFire(CBaseCombatCharacter* pOperator, bool bSecondary)
{
	// Ensure we have enough rounds in the clip
	m_iClip1++;

	Vector vecShootOrigin, vecShootDir;
	QAngle	angShootDir;

	CAI_BaseNPC* npc = pOperator->MyNPCPointer();
	ASSERT(npc != NULL);

	vecShootOrigin = pOperator->Weapon_ShootPosition();
	vecShootDir = npc->GetActualShootTrajectory(vecShootOrigin);
	FireNPCPrimaryAttack(pOperator, vecShootOrigin, vecShootDir);
}

//-----------------------------------------------------------------------------
// AI should almost always have ammo in the clip, so we shouldn't need to reload 
//-----------------------------------------------------------------------------
int CWeaponFlechetteGun::WeaponRangeAttack1Condition(float flDot, float flDist)
{
	if (flDist < m_fMinRange1)
	{
		return COND_TOO_CLOSE_TO_ATTACK;
	}
	else if (flDist > m_fMaxRange1)
	{
		return COND_TOO_FAR_TO_ATTACK;
	}
	else if (flDot < 0.5) 	// UNDONE: Why check this here? Isn't the AI checking this already?
	{
		return COND_NOT_FACING_ATTACK;
	}

	return COND_CAN_RANGE_ATTACK1;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponFlechetteGun::PrimaryAttack(void)
{
	// Only the player fires this way so we can cast
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());
	if (!pPlayer)
		return;

	if (m_bFireOnEmpty)
	{
		return;
	}
    
    if (m_flNextPrimaryAttack > gpGlobals->curtime)
		return;

	// Abort here to handle burst and auto fire modes
	if ((UsesClipsForAmmo1() && m_iClip1 == 0) || (!UsesClipsForAmmo1() && !pPlayer->GetAmmoCount(m_iPrimaryAmmoType)))
		return;

	m_nShotsFired++;

	pPlayer->DoMuzzleFlash();

	// To make the firing framerate independent, we may have to fire more than one bullet here on low-framerate systems, 
	// especially if the weapon we're firing has a really fast rate of fire.
	int iBulletsToFire = 0;
	float fireRate = GetFireRate();

	while (m_flNextPrimaryAttack <= gpGlobals->curtime)
	{
		// MUST call sound before removing a round from the clip of a CHLMachineGun
		WeaponSound(SINGLE, m_flNextPrimaryAttack);
		m_flNextPrimaryAttack = m_flNextPrimaryAttack + fireRate;
		iBulletsToFire++;
	}

	m_iPrimaryAttacks++;
	gamestats->Event_WeaponFired(pPlayer, true, GetClassname());

	Vector vecSrc = pPlayer->Weapon_ShootPosition();

	Vector	aimDir = pPlayer->GetAutoaimVector(AUTOAIM_5DEGREES);

	Vector vecUp, vecRight;
	VectorVectors(aimDir, vecRight, vecUp);

	float x, y, z;

	//Gassian spread
	do {
		x = random->RandomFloat(-0.5, 0.5) + random->RandomFloat(-0.5, 0.5);
		y = random->RandomFloat(-0.5, 0.5) + random->RandomFloat(-0.5, 0.5);
		z = x * x + y * y;
	} while (z > 1);

	aimDir = aimDir + x * GetBulletSpread().x * vecRight + y * GetBulletSpread().y * vecUp;

	QAngle angAiming;
	VectorAngles(aimDir, angAiming);

	FlechetteCreateAutoShoot(vecSrc, aimDir, angAiming, pPlayer);

	//Factor in the view kick
	AddViewKick();

	CSoundEnt::InsertSound(SOUND_COMBAT, GetAbsOrigin(), SOUNDENT_VOLUME_MACHINEGUN, 0.2, pPlayer);

	if (!m_iClip1 && pPlayer->GetAmmoCount(m_iPrimaryAmmoType) <= 0)
	{
		// HEV suit - indicate out of ammo condition
		pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
	}

	SendWeaponAnim(ACT_VM_PRIMARYATTACK);
	pPlayer->SetAnimation(PLAYER_ATTACK1);
	pPlayer->RumbleEffect(RUMBLE_AR2, 0, RUMBLE_FLAG_RESTART);

	if (GetWpnData().m_bUseMuzzleSmoke)
	{
		if (g_fr_plr_muzzlesmoke.GetBool())
		{
			//we should really use a for loop for this
			DispatchParticleEffect("weapon_muzzle_smoke", PATTACH_POINT_FOLLOW, pPlayer->GetViewModel(), "muzzle", true);
		}
	}

	CBaseViewModel* vm = pPlayer->GetViewModel();

	if (vm)
	{
		DispatchParticleEffect("hunter_muzzle_flash", PATTACH_POINT_FOLLOW, vm, "muzzle", true);
	}
	else
	{
		DispatchParticleEffect("hunter_muzzle_flash", PATTACH_POINT_FOLLOW, this, "muzzle", true);
	}

	// Dispatch the elight	
	CEffectData data;
	data.m_nAttachmentIndex = LookupAttachment("muzzle");
	data.m_nEntIndex = entindex();
	DispatchEffect("HunterMuzzleFlash", data);

	m_iClip1--;

	// Register a muzzleflash for the AI
	pPlayer->SetMuzzleFlashTime(gpGlobals->curtime + 0.5);
}

//-----------------------------------------------------------------------------
// Purpose: Handle grenade detonate in-air (even when no ammo is left)
//-----------------------------------------------------------------------------
void CWeaponFlechetteGun::ItemPostFrame(void)
{
	CBasePlayer *pOwner = ToBasePlayer(GetOwner());
	if (!pOwner)
		return;

	if ((pOwner->m_nButtons & IN_ATTACK) == false)
	{
		m_nShotsFired = 0;
		SendWeaponAnim(ACT_VM_IDLE);
	}
	
	BaseClass::ItemPostFrame();
}

const WeaponProficiencyInfo_t* CWeaponFlechetteGun::GetProficiencyValues()
{
	static WeaponProficiencyInfo_t proficiencyTable[] =
	{
		{ 7.0,		0.75	},
		{ 5.00,		0.75	},
		{ 10.0 / 3.0, 0.75	},
		{ 5.0 / 3.0,	0.75	},
		{ 1.00,		1.0		},
	};

	COMPILE_TIME_ASSERT(ARRAYSIZE(proficiencyTable) == WEAPON_PROFICIENCY_PERFECT + 1);

	return proficiencyTable;
}