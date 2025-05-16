//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:	'weapon' what lets the player controll the rollerbuddy.
//
// $Revision: $
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "basehlcombatweapon.h"
#include "npcevent.h"
#include "basecombatcharacter.h"
#include "ai_basenpc.h"
#include "player.h"
#include "entitylist.h"
#include "ndebugoverlay.h"
#include "soundent.h"
#include "engine/IEngineSound.h"
#include "rotorwash.h"
#include "npc_manhack.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar	sv_infinite_manhacks("sv_infinite_manhacks", "0", FCVAR_ARCHIVE);

class CWeaponManhackThrower: public CBaseHLCombatWeapon
{
	DECLARE_CLASS( CWeaponManhackThrower, CBaseHLCombatWeapon );
public:
	DECLARE_SERVERCLASS();
	DECLARE_ACTTABLE();
	DECLARE_DATADESC();
	CWeaponManhackThrower();

	void				Spawn( void );
	void				Precache( void );

	void				SpawnManhack(void);

	float GetFireRate(void)
	{
		return 0.5f;
	}
    
	void				PrimaryAttack( void );
	bool				Reload( void );
	bool				DecrementAmmo( CBaseCombatCharacter *pOwner );

private:
	// check a throw from vecSrc.  If not valid, move the position back along the line to vecEye
	void	CheckThrowPosition(CBasePlayer* pPlayer, const Vector& vecEye, Vector& vecSrc);
};

IMPLEMENT_SERVERCLASS_ST(CWeaponManhackThrower, DT_WeaponManhackThrower)
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS( weapon_manhackthrower, CWeaponManhackThrower );
PRECACHE_WEAPON_REGISTER(weapon_manhackthrower);

BEGIN_DATADESC(CWeaponManhackThrower)
END_DATADESC()

acttable_t CWeaponManhackThrower::m_acttable[] =
{
    { ACT_RANGE_ATTACK1,	ACT_HL2MP_GESTURE_RANGE_ATTACK_MANHAC, true },
	{ ACT_HL2MP_IDLE,		ACT_HL2MP_IDLE_MANHAC,	false },
	{ ACT_HL2MP_RUN,		ACT_HL2MP_RUN_MANHAC,	false },
	{ ACT_HL2MP_IDLE_CROUCH,	ACT_HL2MP_IDLE_CROUCH_MANHAC,	false },
	{ ACT_HL2MP_WALK_CROUCH,	ACT_HL2MP_WALK_CROUCH_MANHAC,	false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,	ACT_HL2MP_GESTURE_RANGE_ATTACK_MANHAC,	false },
	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_MANHAC,			false },
	{ ACT_HL2MP_JUMP,		ACT_HL2MP_JUMP_MANHAC,	false },
};

IMPLEMENT_ACTTABLE(CWeaponManhackThrower);

CWeaponManhackThrower::CWeaponManhackThrower()
{
}

void CWeaponManhackThrower::Spawn( )
{
	BaseClass::Spawn();
	Precache( );
}

void CWeaponManhackThrower::Precache( void )
{
	BaseClass::Precache();
	UTIL_PrecacheOther( "npc_manhack_weapon" );
}

#define MANHACK_THROW_RADIUS	4.0f // inches

// check a throw from vecSrc.  If not valid, move the position back along the line to vecEye
void CWeaponManhackThrower::CheckThrowPosition(CBasePlayer* pPlayer, const Vector& vecEye, Vector& vecSrc)
{
	trace_t tr;

	UTIL_TraceHull(vecEye, vecSrc, -Vector(MANHACK_THROW_RADIUS + 2, MANHACK_THROW_RADIUS + 2, MANHACK_THROW_RADIUS + 2), Vector(MANHACK_THROW_RADIUS + 2, MANHACK_THROW_RADIUS + 2, MANHACK_THROW_RADIUS + 2),
		pPlayer->PhysicsSolidMaskForEntity(), pPlayer, pPlayer->GetCollisionGroup(), &tr);

	if (tr.DidHit())
	{
		vecSrc = tr.endpos;
	}
}

void CWeaponManhackThrower::SpawnManhack(void)
{
	CBasePlayer* pOwner = ToBasePlayer(GetOwner());
	if (!pOwner)
		return;

	CNPC_Manhack* pManhack = dynamic_cast<CNPC_Manhack*>(CreateEntityByName("npc_manhack_weapon"));
	if (pManhack)
	{
		Vector	vecEye = pOwner->EyePosition();
		Vector	vForward, vRight;

		pOwner->EyeVectors(&vForward, &vRight, NULL);
		Vector vecSrc = vecEye + vForward * 18.0f + vRight * 8.0f;
		CheckThrowPosition(pOwner, vecEye, vecSrc);
		//	vForward[0] += 0.1f;
		vForward[2] += 0.1f;

		Vector vecThrow;
		pOwner->GetVelocity(&vecThrow, NULL);
		vecThrow += vForward * 450;

		AngularImpulse angImp = AngularImpulse(600, random->RandomInt(-1200, 1200), 0);

		pManhack->SetName(AllocPooledString("spawnedManhack"));
		pManhack->m_bDisableInitAttributes = true;
		pManhack->m_bNoRemove = true;
		pManhack->SetOwnerEntity(pOwner);
		pManhack->SetLocalOrigin(vecSrc);
		pManhack->SetLocalAngles(vec3_angle);
		pManhack->AddSpawnFlags((SF_MANHACK_PACKED_UP | SF_NPC_FADE_CORPSE));
		pManhack->Spawn();

		IPhysicsObject* pPhysicsObject = pManhack->VPhysicsGetObject();
		if (pPhysicsObject)
		{
			pPhysicsObject->AddVelocity(&vecThrow, &angImp);
		}

#ifdef GLOWS_ENABLE
		Vector allyColor = Vector(26, 77, 153);
		pManhack->m_bImportantOutline = true;
		pManhack->GiveOutline(allyColor);
#endif

		WeaponSound(SPECIAL1);
	}

	DecrementAmmo(pOwner);
}

bool CWeaponManhackThrower::Reload( void )
{
	WeaponIdle();
	return true;
}

void CWeaponManhackThrower::PrimaryAttack( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if ( !pPlayer )
		return;

	WeaponSound(SINGLE);
	SendWeaponAnim(ACT_VM_THROW);
	pPlayer->SetAnimation(PLAYER_ATTACK1);

	//spawn it NOW.
	SpawnManhack();

	m_flNextPrimaryAttack = gpGlobals->curtime + GetFireRate();

	//sequence duration dictates turret spawn.
}

bool CWeaponManhackThrower::DecrementAmmo( CBaseCombatCharacter *pOwner )
{
	if (!sv_infinite_manhacks.GetBool())
	{
		pOwner->RemoveAmmo(1, m_iPrimaryAmmoType);

		if (pOwner->GetAmmoCount(m_iPrimaryAmmoType) <= 0)
		{
			pOwner->SwitchToNextBestWeapon(this);
			return false;
		}
	}
    
    return true;
}
