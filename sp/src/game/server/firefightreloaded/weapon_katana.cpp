//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		Katana - HATATATATATATATATATATATATATATATATATATATATATATATATATATATA
// https://twitter.com/SadlyItsBradley/status/1516391717867507712
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "basehlcombatweapon.h"
#include "gamerules.h"
#include "ammodef.h"
#include "mathlib/mathlib.h"
#include "in_buttons.h"
#include "soundent.h"
#include "basebludgeonweapon.h"
#include "vstdlib/random.h"
#include "npcevent.h"
#include "ai_basenpc.h"
#include "weapon_katana.h"
#include "rumble_shared.h"
#include "gamestats.h"
#include "npc_combineace.h"
#include "npc_advisor.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define BLUDGEON_HULL_DIM		16

static const Vector g_bludgeonMins(-BLUDGEON_HULL_DIM, -BLUDGEON_HULL_DIM, -BLUDGEON_HULL_DIM);
static const Vector g_bludgeonMaxs(BLUDGEON_HULL_DIM, BLUDGEON_HULL_DIM, BLUDGEON_HULL_DIM);
extern ConVar sk_plr_dmg_katana;

static ConVar sv_katana_killmultiplier_postdelay("sv_katana_killmultiplier_postdelay", "5.0", FCVAR_CHEAT);
ConVar sv_katana_killmultiplier_maxmultiplier("sv_katana_killmultiplier_maxmultiplier", "5", FCVAR_CHEAT);
static ConVar sv_katana_killmultiplier_maxtimestogivebonus("sv_katana_killmultiplier_maxtimestogivebonus", "10", FCVAR_CHEAT);
ConVar sv_katana_killmultiplier_suitpower("sv_katana_killmultiplier_suitpower", "20", FCVAR_CHEAT);

static ConVar sv_katana_charge_damagebonus("sv_katana_charge_damagebonus", "2", FCVAR_CHEAT);

static ConVar sk_katana_enemy_damageresistance("sk_katana_enemy_damageresistance", "0.5");

//-----------------------------------------------------------------------------
// CWeaponKatana
//-----------------------------------------------------------------------------

IMPLEMENT_SERVERCLASS_ST(CWeaponKatana, DT_WeaponKatana)
END_SEND_TABLE()

#ifndef HL2MP
LINK_ENTITY_TO_CLASS( weapon_katana, CWeaponKatana );
PRECACHE_WEAPON_REGISTER( weapon_katana );
#endif

BEGIN_DATADESC(CWeaponKatana)
DEFINE_FIELD(m_iKillMultiplierCount, FIELD_INTEGER),
DEFINE_FIELD(m_iKills, FIELD_INTEGER),
DEFINE_FIELD(m_flLastKill, FIELD_TIME),
DEFINE_FIELD(m_bKillMultiplier, FIELD_BOOLEAN),
DEFINE_FIELD(m_iLastSuitCharge, FIELD_INTEGER),
DEFINE_FIELD(m_bHitMaxKillMultiplier, FIELD_BOOLEAN)
END_DATADESC()

acttable_t CWeaponKatana::m_acttable[] = 
{
	{ ACT_MELEE_ATTACK1,	ACT_MELEE_ATTACK_SWING, true },
	{ ACT_IDLE,				ACT_IDLE_ANGRY_MELEE,	false },
	{ ACT_IDLE_ANGRY,		ACT_IDLE_ANGRY_MELEE,	false },
	{ ACT_RANGE_ATTACK1,	ACT_RANGE_ATTACK_SLAM,	true },
	{ ACT_HL2MP_IDLE,		ACT_HL2MP_IDLE_MELEE,	false },
	{ ACT_HL2MP_RUN,		ACT_HL2MP_RUN_MELEE,	false },
	{ ACT_HL2MP_IDLE_CROUCH,	ACT_HL2MP_IDLE_CROUCH_MELEE,	false },
	{ ACT_HL2MP_WALK_CROUCH,	ACT_HL2MP_WALK_CROUCH_MELEE,	false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,	ACT_HL2MP_GESTURE_RANGE_ATTACK_MELEE,	false },
	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_MELEE,			false },
	{ ACT_HL2MP_JUMP,		ACT_HL2MP_JUMP_MELEE,	false },
};

IMPLEMENT_ACTTABLE(CWeaponKatana);

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CWeaponKatana::CWeaponKatana( void )
{
	m_flLastKill = 0;
	ResetKillMultiplier();
	m_bKillMultiplier = true;
	m_bHitMaxKillMultiplier = false;
	m_nSkin = 0;
}

void CWeaponKatana::Equip(CBaseCombatCharacter* pOwner)
{
	m_kvEnemyResist.LoadEntries("scripts/katana_enemy_damageresist.txt", "KatanaDMGResist");
	BaseClass::Equip(pOwner);
}

//-----------------------------------------------------------------------------
// Purpose: Add in a view kick for this weapon
//-----------------------------------------------------------------------------
void CWeaponKatana::AddViewKick( void )
{
	CBasePlayer *pPlayer  = ToBasePlayer( GetOwner() );
	
	if ( pPlayer == NULL )
		return;

	QAngle punchAng;

	punchAng.x = random->RandomFloat( 1.0f, 2.0f );
	punchAng.y = random->RandomFloat( -2.0f, -1.0f );
	punchAng.z = 0.0f;
	
	pPlayer->ViewPunch( punchAng ); 
}

float CWeaponKatana::GetRange(void)
{ 
	CHL2_Player* pPlayer = ToHL2Player(GetOwner());

	if ((pPlayer && pPlayer->IsCharging()) || g_pGameRules->isInBullettime)
	{
		return KATANA_RANGE_CHARGING;
	}
	else
	{
		return KATANA_RANGE;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Primary fire button attack
//-----------------------------------------------------------------------------
void CWeaponKatana::PrimaryAttack(void)
{
	// Only the player fires this way so we can cast
	CBasePlayer *pPlayer = ToBasePlayer(GetOwner());

	if (!pPlayer)
	{
		return;
	}

	trace_t traceHit;

	Vector swingStart = pPlayer->Weapon_ShootPosition();
	Vector forward;

	forward = pPlayer->GetAutoaimVector(AUTOAIM_SCALE_DEFAULT, GetRange());

	Vector swingEnd = swingStart + forward * GetRange();
	UTIL_TraceLine(swingStart, swingEnd, MASK_SHOT_HULL, pPlayer, COLLISION_GROUP_NONE, &traceHit);

	if (traceHit.fraction == 1.0)
	{
		float bludgeonHullRadius = 1.732f * BLUDGEON_HULL_DIM;  // hull is +/- 16, so use cuberoot of 2 to determine how big the hull is from center to the corner point

		// Back off by hull "radius"
		swingEnd -= forward * bludgeonHullRadius;

		UTIL_TraceHull(swingStart, swingEnd, g_bludgeonMins, g_bludgeonMaxs, MASK_SHOT_HULL, pPlayer, COLLISION_GROUP_NONE, &traceHit);
		if (traceHit.fraction < 1.0 && traceHit.m_pEnt)
		{
			Vector vecToTarget = traceHit.m_pEnt->GetAbsOrigin() - swingStart;
			VectorNormalize(vecToTarget);

			float dot = vecToTarget.Dot(forward);

			// YWB:  Make sure they are sort of facing the guy at least...
			if (dot < 0.70721f)
			{
				// Force amiss
				traceHit.fraction = 1.0f;
			}
		}
	}

	m_iPrimaryAttacks++;

	WeaponSound(SINGLE);
	SendWeaponAnim(ACT_VM_HITCENTER);

	pPlayer->SetAnimation(PLAYER_ATTACK1);

	pPlayer->RumbleEffect(RUMBLE_CROWBAR_SWING, 0, RUMBLE_FLAG_RESTART);

	// -------------------------
	//	Miss
	// -------------------------
	if (traceHit.fraction == 1.0f)
	{
		// We want to test the first swing again
		Vector testEnd = swingStart + forward * GetRange();

		// See if we happened to hit water
		ImpactWater(swingStart, testEnd);
	}
	else
	{
		if (traceHit.DidHitWorld())
		{
			Hit(traceHit, GetActivity(), false);
			WeaponSound(MELEE_HIT_WORLD);
		}
		else
		{
			if (traceHit.m_pEnt)
			{
				if (traceHit.m_pEnt->IsNPC() || traceHit.m_pEnt->IsPlayer() || traceHit.m_pEnt->IsBaseCombatWeapon())
				{
					CBaseEntity* ent = (traceHit.m_pEnt->IsBaseCombatWeapon() && 
						traceHit.m_pEnt->GetOwnerEntity() && 
						(traceHit.m_pEnt->GetOwnerEntity()->IsNPC() || traceHit.m_pEnt->GetOwnerEntity()->IsPlayer()))
						? traceHit.m_pEnt->GetOwnerEntity() 
						: traceHit.m_pEnt;
					Vector vecSrc = pPlayer->Weapon_ShootPosition();
					Vector vecAiming = pPlayer->GetAutoaimVector(AUTOAIM_SCALE_DEFAULT);

					CHL2_Player* pHL2Player = ToHL2Player(GetOwner());

					//CAmmoDef* def = GetAmmoDef();

					FireBulletsInfo_t info;
					info.m_iShots = 3;
					info.m_vecSrc = vecSrc;
					info.m_vecDirShooting = vecAiming;
					info.m_vecSpread = VECTOR_CONE_4DEGREES;
					info.m_flDistance = GetRange();
					info.m_iAmmoType = m_iPrimaryAmmoType;
					info.m_iTracerFreq = 0;
					info.m_flDamage = info.m_iPlayerDamage = CalculateDamage(ent, pHL2Player);

					//int dmgType = def->DamageType(info.m_iAmmoType);
					//info.m_nDamageFlags = g_pGameRules->isInBullettime ? (dmgType &= ~DMG_NEVERGIB) : dmgType;

					info.m_pAttacker = pPlayer;
					info.m_nFlags = FIRE_BULLETS_FIRST_SHOT_ACCURATE;
					info.m_bPrimaryAttack = true;
					info.m_bAffectedByBullettime = false;

					pPlayer->FireBullets(info);

					if (ent)
					{
						bool usesShield = false;

						CNPC_CombineAce* pAce = dynamic_cast<CNPC_CombineAce*>(ent);
						if (pAce)
						{
							if (!pAce->m_bBulletResistanceBroken)
							{
								usesShield = true;
							}
						}
						else
						{
							CNPC_Advisor* pAdvisor = dynamic_cast<CNPC_Advisor*>(ent);
							if (pAdvisor)
							{
								if (!pAdvisor->m_bBulletResistanceBroken)
								{
									usesShield = true;
								}
							}
						}

						if (!usesShield)
						{
							if (ent->BloodColor() == BLOOD_COLOR_RED)
							{
								m_nSkin = 1;

								CBaseViewModel* pVM = pPlayer->GetViewModel();

								if (pVM)
								{
									pVM->m_nSkin = 1;
								}
							}
							else if (ent->BloodColor() == BLOOD_COLOR_YELLOW ||
								ent->BloodColor() == BLOOD_COLOR_ZOMBIE ||
								ent->BloodColor() == BLOOD_COLOR_GREEN ||
								ent->BloodColor() == BLOOD_COLOR_ANTLION_WORKER ||
								ent->BloodColor() == BLOOD_COLOR_ANTLION)
							{
								m_nSkin = 2;

								CBaseViewModel* pVM = pPlayer->GetViewModel();

								if (pVM)
								{
									pVM->m_nSkin = 2;
								}
							}
						}
					}

					ActivateKillMultiplier(ent, pHL2Player);

					WeaponSound(MELEE_HIT);
				}
				else
				{
					Hit(traceHit, GetActivity(), false);
					WeaponSound(MELEE_HIT_WORLD);
				}
			}
			else
			{
				WeaponSound(SINGLE);
			}
		}

		AddViewKick();
	}

	//Setup our next attack times
	m_flNextPrimaryAttack = gpGlobals->curtime + GetFireRate();
	m_flNextSecondaryAttack = gpGlobals->curtime + GetFireRate();

	CSoundEnt::InsertSound(SOUND_COMBAT, GetAbsOrigin(), 300, 0.2, GetOwner());
}

int CWeaponKatana::CalculateDamage(CBaseEntity* pVictim, CHL2_Player* pAttacker)
{
	int damage = sk_plr_dmg_katana.GetInt();

	bool foundEnemy = m_kvEnemyResist.FindEntry(MAKE_STRING(pVictim->GetClassname()));

	if (foundEnemy)
	{
		bool applyDamageResist = false;

		CNPC_CombineAce* pAce = dynamic_cast<CNPC_CombineAce*>(pVictim);
		if (pAce)
		{
			if (pAce->m_bBulletResistanceBroken)
			{
				applyDamageResist = true;
			}
		}
		else
		{
			CNPC_Advisor* pAdvisor = dynamic_cast<CNPC_Advisor*>(pVictim);
			if (pAdvisor)
			{
				if (pAdvisor->m_bBulletResistanceBroken)
				{
					applyDamageResist = true;
				}
			}
		}

		if (applyDamageResist)
		{
			damage = damage * sk_katana_enemy_damageresistance.GetFloat();
		}
	}

	if (!pAttacker)
		return damage;

	if (pAttacker->IsCharging())
	{
		damage = damage * sv_katana_charge_damagebonus.GetFloat();
	}

	return damage;
}

float CWeaponKatana::GetDamageForActivity(Activity hitActivity)
{ 
	float damage = sk_plr_dmg_katana.GetFloat();
	CHL2_Player* pPlayer = ToHL2Player(GetOwner());

	if (pPlayer && pPlayer->IsCharging())
	{
		damage = damage * sv_katana_charge_damagebonus.GetFloat();
	}

	return damage;
}

bool CWeaponKatana::CanHolster(void)
{
	CHL2_Player* pPlayer = ToHL2Player(GetOwner());

	//this is dumb. why does it bug out when we get the 357??
	if ((pPlayer && pPlayer->IsCharging()) || (g_pGameRules->isInBullettime && m_iKillMultiplierCount > 0))
	{
		return false;
	}

	return BaseClass::CanHolster();
}

void CWeaponKatana::SecondaryAttack(void)
{ 
	CHL2_Player* pPlayer = ToHL2Player(GetOwner());

	if (pPlayer)
	{
		if (pPlayer->IsCharging())
			return;

		pPlayer->DoCharge();
	}
}

bool CWeaponKatana::Holster(CBaseCombatWeapon* pSwitchingTo)
{
	if (!g_pGameRules->isInBullettime)
	{
		m_bKillMultiplier = false;
		m_bHitMaxKillMultiplier = false;
		ResetKillMultiplier();
	}

	/*
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (pPlayer)
	{
		m_nSkin = 0;

		CBaseViewModel* pVM = pPlayer->GetViewModel();

		if (pVM)
		{
			pVM->m_nSkin = 0;
		}
	}*/

	return BaseClass::Holster(pSwitchingTo);
}

void CWeaponKatana::ItemPostFrame(void)
{
	if (!g_pGameRules->isInBullettime)
	{
		ResetKillMultiplier();
		m_bKillMultiplier = false;
		m_bHitMaxKillMultiplier = false;
	}
	else
	{
		if (m_iKills >= sv_katana_killmultiplier_maxtimestogivebonus.GetInt())
		{
			m_bKillMultiplier = false;
		}
		else
		{
			if (m_flLastKill < gpGlobals->curtime && m_iKillMultiplierCount > 0)
			{
				m_iKillMultiplierCount = 0;
			}
		}
	}

	if ((m_iKills < sv_katana_killmultiplier_maxtimestogivebonus.GetInt() && m_iKillMultiplierCount > 0 && g_pGameRules->isInBullettime) ||
		!g_pGameRules->isInBullettime)
	{
		if (m_flLastKill < gpGlobals->curtime && !m_bKillMultiplier)
		{
			ResetKillMultiplier();
			m_bKillMultiplier = true;
		}
	}

	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (pPlayer)
	{
		//wash it off if we're in water.
		if (pPlayer->GetWaterLevel() >= 3)
		{
			m_nSkin = 0;

			CBaseViewModel* pVM = pPlayer->GetViewModel();

			if (pVM)
			{
				pVM->m_nSkin = 0;
			}
		}
	}

	BaseClass::ItemPostFrame();
}

void CWeaponKatana::ResetKillMultiplier(void)
{
	m_iKillMultiplierCount = 0;
	m_iKills = 0;
	m_iLastSuitCharge = 0;
}

void CWeaponKatana::ActivateKillMultiplier(CBaseEntity* pVictim, CHL2_Player* pAttacker)
{
	if (!pAttacker)
		return;

	if (pVictim && !pVictim->IsAlive() && IsKillMultiplierEnabled())
	{
		if (m_iKills < sv_katana_killmultiplier_maxtimestogivebonus.GetInt())
		{
			m_iKills++;

			if (m_iKillMultiplierCount < sv_katana_killmultiplier_maxmultiplier.GetInt())
			{
				m_iKillMultiplierCount++;
				pAttacker->TakeHealth((pVictim->GetMaxHealth() * 0.5) * m_iKillMultiplierCount, DMG_GENERIC);
				m_flLastKill = gpGlobals->curtime + sv_katana_killmultiplier_postdelay.GetFloat();
				const char* hintMultiplier = CFmtStr("%ix!", m_iKillMultiplierCount);
				pAttacker->ShowLevelMessage(hintMultiplier);

				if (m_iLastSuitCharge < sv_katana_killmultiplier_maxmultiplier.GetInt())
				{
					m_iLastSuitCharge++;
					pAttacker->SuitPower_Charge(sv_katana_killmultiplier_suitpower.GetFloat() * m_iKillMultiplierCount);
				}
			}

			//if we capped the max ONCE, set m_bHitMaxKillMultiplier to true so the hl2_player can keep the speed boost for the duration of bullettime.
			if (m_iKillMultiplierCount >= sv_katana_killmultiplier_maxmultiplier.GetInt())
			{
				m_bHitMaxKillMultiplier = true;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponKatana::ImpactEffect(trace_t& traceHit)
{
	// See if we hit water (we don't do the other impact effects in this case)
	if (ImpactWater(traceHit.startpos, traceHit.endpos))
		return;

	//FIXME: need new decals
	UTIL_ImpactTrace(&traceHit, DMG_SLASH);
}