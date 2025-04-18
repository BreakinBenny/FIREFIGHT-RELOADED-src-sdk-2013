//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: A shotgun.
//
//			Primary attack: single barrel shot.
//			Secondary attack: double barrel shot.
//
//=============================================================================//

#include "cbase.h"
#include "npcevent.h"
#include "basehlcombatweapon_shared.h"
#include "basecombatcharacter.h"
#include "ai_basenpc.h"
#include "player.h"
#include "gamerules.h"		// For g_pGameRules
#include "in_buttons.h"
#include "soundent.h"
#include "vstdlib/random.h"
#include "gamestats.h"
#include "npc_combine.h"
#include "npc_citizen17.h"
#include "ammodef.h"
#include "rumble_shared.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar sk_auto_reload_time;
extern ConVar sk_plr_num_shotgun_pellets;

class CWeaponXM1014 : public CBaseHLCombatWeapon
{
	DECLARE_DATADESC();
public:
	DECLARE_CLASS( CWeaponXM1014, CBaseHLCombatWeapon );

	DECLARE_SERVERCLASS();

public:
	void	Precache( void );

	int CapabilitiesGet( void ) { return bits_CAP_WEAPON_RANGE_ATTACK1; }

	virtual const Vector& GetBulletSpread( void )
	{
		static Vector vitalAllyCone = VECTOR_CONE_4DEGREES;
		static Vector cone = VECTOR_CONE_7DEGREES;
		static Vector ironsightCone = VECTOR_CONE_5DEGREES;

		if( GetOwner() && (GetOwner()->Classify() == CLASS_PLAYER_ALLY_VITAL) )
		{
			// Give Alyx's shotgun blasts more a more directed punch. She needs
			// to be at least as deadly as she would be with her pistol to stay interesting (sjb)
			return vitalAllyCone;
		}

		if (IsIronsighted())
		{
			return ironsightCone;
		}

		return cone;
	}

	virtual int				GetMinBurst() { return 4; }
	virtual int				GetMaxBurst() { return 7; }

	virtual float			GetMinRestTime();
	virtual float			GetMaxRestTime();

	virtual float			GetFireRate( void );

	bool StartReload( void );
	bool Reload( void );
	void FillClip( void );
	void FinishReload( void );
	void CheckHolsterReload( void );
//	void WeaponIdle( void );
	void ItemHolsterFrame( void );
	void ItemPostFrame( void );
	void PrimaryAttack( void );
	void DryFire( void );

	void FireNPCPrimaryAttack( CBaseCombatCharacter *pOperator, bool bUseWeaponAngles, bool secondary );
	void Operator_ForceNPCFire( CBaseCombatCharacter  *pOperator, bool bSecondary );
	void Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );

	DECLARE_ACTTABLE();

	CWeaponXM1014(void);
};

IMPLEMENT_SERVERCLASS_ST(CWeaponXM1014, DT_WeaponXM1014)
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS( weapon_xm1014, CWeaponXM1014 );
PRECACHE_WEAPON_REGISTER(weapon_xm1014);

BEGIN_DATADESC( CWeaponXM1014 )
END_DATADESC()

acttable_t	CWeaponXM1014::m_acttable[] = 
{
	{ ACT_IDLE,						ACT_IDLE_SMG1,					true },	// FIXME: hook to shotgun unique

	{ ACT_RANGE_ATTACK1,			ACT_RANGE_ATTACK_SHOTGUN,			true },
	{ ACT_RELOAD,					ACT_RELOAD_SHOTGUN,					false },
	{ ACT_WALK,						ACT_WALK_RIFLE,						true },
	{ ACT_IDLE_ANGRY,				ACT_IDLE_ANGRY_SHOTGUN,				true },

// Readiness activities (not aiming)
	{ ACT_IDLE_RELAXED,				ACT_IDLE_SHOTGUN_RELAXED,		false },//never aims
	{ ACT_IDLE_STIMULATED,			ACT_IDLE_SHOTGUN_STIMULATED,	false },
	{ ACT_IDLE_AGITATED,			ACT_IDLE_SHOTGUN_AGITATED,		false },//always aims

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

	{ ACT_WALK_AIM,					ACT_WALK_AIM_SHOTGUN,				true },
	{ ACT_WALK_CROUCH,				ACT_WALK_CROUCH_RIFLE,				true },
	{ ACT_WALK_CROUCH_AIM, ACT_WALK_CROUCH_AIM_RIFLE, true },
	{ ACT_RUN,						ACT_RUN_RIFLE,						true },
	{ ACT_RUN_AIM,					ACT_RUN_AIM_SHOTGUN,				true },
	{ ACT_RUN_CROUCH,				ACT_RUN_CROUCH_RIFLE,				true },
	{ ACT_RUN_CROUCH_AIM,			ACT_RUN_CROUCH_AIM_RIFLE,			true },
	{ ACT_GESTURE_RANGE_ATTACK1,	ACT_GESTURE_RANGE_ATTACK_SHOTGUN,	true },
	{ ACT_RANGE_ATTACK1_LOW,		ACT_RANGE_ATTACK_SHOTGUN_LOW,		true },
	{ ACT_RELOAD_LOW,				ACT_RELOAD_SHOTGUN_LOW,				false },
	{ ACT_GESTURE_RELOAD,			ACT_GESTURE_RELOAD_SHOTGUN,			false },
	{ ACT_HL2MP_IDLE,				ACT_HL2MP_IDLE_SHOTGUN,				false },
	{ ACT_HL2MP_RUN,				ACT_HL2MP_RUN_SHOTGUN,				false },
	{ ACT_HL2MP_IDLE_CROUCH,		ACT_HL2MP_IDLE_CROUCH_SHOTGUN,		false },
	{ ACT_HL2MP_WALK_CROUCH,		ACT_HL2MP_WALK_CROUCH_SHOTGUN,		false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,		ACT_HL2MP_GESTURE_RANGE_ATTACK_SHOTGUN,		false },
	{ ACT_HL2MP_GESTURE_RELOAD,		ACT_HL2MP_GESTURE_RELOAD_SHOTGUN,	false },
	{ ACT_HL2MP_JUMP,				ACT_HL2MP_JUMP_SHOTGUN,				false },
	{ ACT_RANGE_ATTACK1,			ACT_RANGE_ATTACK_SHOTGUN,			false },
};

IMPLEMENT_ACTTABLE(CWeaponXM1014);

void CWeaponXM1014::Precache(void)
{
	CBaseCombatWeapon::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pOperator - 
//-----------------------------------------------------------------------------
void CWeaponXM1014::FireNPCPrimaryAttack(CBaseCombatCharacter* pOperator, bool bUseWeaponAngles, bool bSecondary)
{
	Vector vecShootOrigin, vecShootDir;
	CAI_BaseNPC* npc = pOperator->MyNPCPointer();
	ASSERT(npc != NULL);
	WeaponSound(SINGLE_NPC);

	pOperator->DoMuzzleFlash();

	m_iClip1 = m_iClip1 - 1;

	if (bUseWeaponAngles)
	{
		QAngle	angShootDir;
		GetAttachment(LookupAttachment("muzzle"), vecShootOrigin, angShootDir);
		AngleVectors(angShootDir, &vecShootDir);
	}
	else
	{
		vecShootOrigin = pOperator->Weapon_ShootPosition();
		vecShootDir = npc->GetActualShootTrajectory(vecShootOrigin);
	}

	if (GetWpnData().m_bUseMuzzleSmoke)
	{
		if (g_fr_npc_muzzlesmoke.GetBool())
		{
			DispatchParticleEffect("weapon_muzzle_smoke", PATTACH_POINT_FOLLOW, this, "muzzle");
		}
	}

	CAmmoDef* def = GetAmmoDef();

	FireBulletsInfo_t info;
	info.m_iShots = 6;
	info.m_vecSrc = vecShootOrigin;
	info.m_vecDirShooting = vecShootDir;
	info.m_vecSpread = GetBulletSpread();
	info.m_flDistance = MAX_TRACE_LENGTH;
	info.m_iTracerFreq = 0;
	info.m_iAmmoType = m_iPrimaryAmmoType;

	int dmgType = def->DamageType(info.m_iAmmoType);

	info.m_nDamageFlags = (dmgType &= ~DMG_SNIPER);

	pOperator->FireBullets(info);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponXM1014::Operator_ForceNPCFire( CBaseCombatCharacter *pOperator, bool bSecondary )
{
	// Ensure we have enough rounds in the clip
	m_iClip1 += 1;
	FireNPCPrimaryAttack( pOperator, true, bSecondary);
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  :
// Output :
//-----------------------------------------------------------------------------
void CWeaponXM1014::Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	switch( pEvent->event )
	{
		case EVENT_WEAPON_SHOTGUN_FIRE:
		{
			FireNPCPrimaryAttack( pOperator, false, false);
		}
		break;
		default:
			CBaseCombatWeapon::Operator_HandleAnimEvent( pEvent, pOperator );
			break;
	}
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
float CWeaponXM1014::GetMinRestTime()
{
	return 0.1;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
float CWeaponXM1014::GetMaxRestTime()
{
	return 0.25;
}

//-----------------------------------------------------------------------------
// Purpose: Time between successive shots in a burst. Also returned for EP2
//			with an eye to not messing up Alyx in EP1.
//-----------------------------------------------------------------------------
float CWeaponXM1014::GetFireRate()
{
	if (IsIronsighted())
	{
		return 0.35f;
	}

	return 0.25f;
}

//-----------------------------------------------------------------------------
// Purpose: Override so only reload one shell at a time
// Input  :
// Output :
//-----------------------------------------------------------------------------
bool CWeaponXM1014::StartReload( void )
{
	CBaseCombatCharacter *pOwner  = GetOwner();
	
	if ( pOwner == NULL )
		return false;

	if (pOwner->GetAmmoCount(m_iPrimaryAmmoType) <= 0)
		return false;

	if (m_iClip1 >= GetMaxClip1())
		return false;

	int j = MIN(1, pOwner->GetAmmoCount(m_iPrimaryAmmoType));

	if (j <= 0)
		return false;

	SendWeaponAnim( ACT_SHOTGUN_RELOAD_START );

	// Make shotgun shell visible
	SetBodygroup(1,0);

	pOwner->m_flNextAttack = gpGlobals->curtime;
	m_flNextPrimaryAttack = gpGlobals->curtime + SequenceDuration();

	m_bInReload = true;

	DisableIronsights();

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Override so only reload one shell at a time
// Input  :
// Output :
//-----------------------------------------------------------------------------
bool CWeaponXM1014::Reload( void )
{
	// Check that StartReload was called first
	if (!m_bInReload)
	{
		Warning("ERROR: Shotgun Reload called incorrectly!\n");
	}

	CBaseCombatCharacter *pOwner  = GetOwner();
	
	if ( pOwner == NULL )
		return false;

	if (pOwner->GetAmmoCount(m_iPrimaryAmmoType) <= 0)
		return false;

	if (m_iClip1 >= GetMaxClip1())
		return false;

	int j = MIN(1, pOwner->GetAmmoCount(m_iPrimaryAmmoType));

	if (j <= 0)
		return false;

	FillClip();
	// Play reload on different channel as otherwise steals channel away from fire sound
	WeaponSound(RELOAD);
	SendWeaponAnim( ACT_VM_RELOAD );

	CBasePlayer* pPlayer = ((CBasePlayer*)pOwner);

	if (pPlayer)
	{
		pPlayer->RumbleEffect(RUMBLE_STOP_ALL, 0, RUMBLE_FLAGS_NONE);
	}

	pOwner->m_flNextAttack = gpGlobals->curtime;
	m_flNextPrimaryAttack = gpGlobals->curtime + 0.6f; //SequenceDuration();

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Play finish reload anim and fill clip
// Input  :
// Output :
//-----------------------------------------------------------------------------
void CWeaponXM1014::FinishReload( void )
{
	CBaseCombatCharacter *pOwner  = GetOwner();
	
	if ( pOwner == NULL )
		return;

	m_bInReload = false;

	// Finish reload animation
	SendWeaponAnim( ACT_SHOTGUN_RELOAD_FINISH );

	pOwner->m_flNextAttack = gpGlobals->curtime;
	m_flNextPrimaryAttack = gpGlobals->curtime + SequenceDuration();
}

//-----------------------------------------------------------------------------
// Purpose: Play finish reload anim and fill clip
// Input  :
// Output :
//-----------------------------------------------------------------------------
void CWeaponXM1014::FillClip( void )
{
	CBaseCombatCharacter *pOwner  = GetOwner();
	
	if ( pOwner == NULL )
		return;

	// Add them to the clip
	if ( pOwner->GetAmmoCount( m_iPrimaryAmmoType ) > 0 )
	{
		if ( Clip1() < GetMaxClip1() )
		{
			m_iClip1++;

			if (pOwner->IsPlayer())
			{
				CBasePlayer* pPlayer = ToBasePlayer(pOwner);

				if (pPlayer && pPlayer->m_iPerkInfiniteAmmo != 1)
				{
					pOwner->RemoveAmmo(1, m_iPrimaryAmmoType);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//
//
//-----------------------------------------------------------------------------
void CWeaponXM1014::DryFire( void )
{
	WeaponSound(EMPTY);
	SendWeaponAnim( ACT_VM_DRYFIRE );
	
	m_flNextPrimaryAttack = gpGlobals->curtime + GetFireRate();
}

//-----------------------------------------------------------------------------
// Purpose: 
//
//
//-----------------------------------------------------------------------------
void CWeaponXM1014::PrimaryAttack( void )
{
	// Only the player fires this way so we can cast
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	if (!pPlayer)
	{
		return;
	}

	// MUST call sound before removing a round from the clip of a CMachineGun
	WeaponSound(SINGLE);

	pPlayer->DoMuzzleFlash();
	pPlayer->RumbleEffect(RUMBLE_SHOTGUN_SINGLE, 0, RUMBLE_FLAG_RESTART);

	SendWeaponAnim( ACT_VM_PRIMARYATTACK );

	// player "shoot" animation
	pPlayer->SetAnimation( PLAYER_ATTACK1 );

	if (GetWpnData().m_bUseMuzzleSmoke)
	{
		if (g_fr_plr_muzzlesmoke.GetBool())
		{
			DispatchParticleEffect("weapon_muzzle_smoke", PATTACH_POINT_FOLLOW, pPlayer->GetViewModel(), "muzzle");
		}
	}

	// Don't fire again until fire animation has completed
	m_flNextPrimaryAttack = gpGlobals->curtime + GetFireRate();
	m_iClip1 -= 1;

	Vector	vecSrc		= pPlayer->Weapon_ShootPosition( );
	Vector	vecAiming	= pPlayer->GetAutoaimVector( AUTOAIM_SCALE_DEFAULT );	

	pPlayer->SetMuzzleFlashTime( gpGlobals->curtime + 1.0 );
	
	CAmmoDef* def = GetAmmoDef();

	// Fire the bullets, and force the first shot to be perfectly accuracy
	FireBulletsInfo_t info;
	info.m_iShots = 6;
	info.m_vecSrc = vecSrc;
	info.m_vecDirShooting = vecAiming;
	info.m_vecSpread = GetBulletSpread();
	info.m_flDistance = MAX_TRACE_LENGTH;
	info.m_iTracerFreq = 0;
	info.m_iAmmoType = m_iPrimaryAmmoType;
	info.m_nFlags = FIRE_BULLETS_FIRST_SHOT_ACCURATE;
	info.m_bPrimaryAttack = true;

	int dmgType = def->DamageType(info.m_iAmmoType);

	info.m_nDamageFlags = !IsIronsighted() ? (dmgType &= ~DMG_SNIPER) : dmgType;
	pPlayer->FireBullets(info);
	
	pPlayer->ViewPunch( QAngle( random->RandomFloat( -2, -1 ), random->RandomFloat( -2, 2 ), 0 ) );

	CSoundEnt::InsertSound( SOUND_COMBAT, GetAbsOrigin(), SOUNDENT_VOLUME_SHOTGUN, 0.2, GetOwner() );

	if (!m_iClip1 && pPlayer->GetAmmoCount(m_iPrimaryAmmoType) <= 0)
	{
		// HEV suit - indicate out of ammo condition
		pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0); 
	}

	m_iPrimaryAttacks++;
	gamestats->Event_WeaponFired( pPlayer, true, GetClassname() );
}
	
//-----------------------------------------------------------------------------
// Purpose: Override so shotgun can do mulitple reloads in a row
//-----------------------------------------------------------------------------
void CWeaponXM1014::ItemPostFrame( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if (!pOwner)
	{
		return;
	}

	ConVar *viewmodel_lower_on_sprint = cvar->FindVar("viewmodel_lower_on_sprint");

	if (!bLowered && (pOwner->m_nButtons & IN_SPEED) && !IsIronsighted() && viewmodel_lower_on_sprint->GetBool())
	{
		bLowered = true;
		SendWeaponAnim(ACT_VM_IDLE_LOWERED);
		m_fLoweredReady = gpGlobals->curtime;
		m_flNextPrimaryAttack = m_fLoweredReady;
	}
	else if (bLowered && !(pOwner->m_nButtons & IN_SPEED) && !IsIronsighted() && viewmodel_lower_on_sprint->GetBool())
	{
		bLowered = false;
		SendWeaponAnim(ACT_VM_IDLE);
		m_fLoweredReady = gpGlobals->curtime;
		m_flNextPrimaryAttack = m_fLoweredReady;
	}

	if (bLowered && !IsIronsighted() && viewmodel_lower_on_sprint->GetBool())
	{
		if (gpGlobals->curtime > m_fLoweredReady)
		{
			bLowered = true;
			SendWeaponAnim(ACT_VM_IDLE_LOWERED);
			m_fLoweredReady = gpGlobals->curtime;
			m_flNextPrimaryAttack = m_fLoweredReady;
		}
		return;
	}
	else if (bLowered && !IsIronsighted() && viewmodel_lower_on_sprint->GetBool())
	{
		if (gpGlobals->curtime > m_fLoweredReady)
		{
			bLowered = false;
			SendWeaponAnim(ACT_VM_IDLE);
			m_fLoweredReady = gpGlobals->curtime;
			m_flNextPrimaryAttack = m_fLoweredReady;
		}
		return;
	}

	if (m_bInReload)
	{
		// If I'm primary firing and have one round stop reloading and fire
		if ((pOwner->m_nButtons & IN_ATTACK ) && (m_iClip1 >=1))
		{
			m_bInReload		= false;
		}
		else if (m_flNextPrimaryAttack <= gpGlobals->curtime)
		{
			// If out of ammo end reload
			if (pOwner->GetAmmoCount(m_iPrimaryAmmoType) <=0)
			{
				FinishReload();
				return;
			}
			// If clip not full reload again
			if (m_iClip1 < GetMaxClip1())
			{
				Reload();
				return;
			}
			// Clip full, stop reloading
			else
			{
				FinishReload();
				return;
			}
		}
	}
    
	if ( (pOwner->m_nButtons & IN_ATTACK) && m_flNextPrimaryAttack <= gpGlobals->curtime)
	{
		if ( (m_iClip1 <= 0 && UsesClipsForAmmo1()) || ( !UsesClipsForAmmo1() && !pOwner->GetAmmoCount(m_iPrimaryAmmoType) ) )
		{
			if (!pOwner->GetAmmoCount(m_iPrimaryAmmoType))
			{
				DryFire();
			}
			else
			{
				StartReload();
			}
		}
		// Fire underwater?
		else if (pOwner->GetWaterLevel() == 3 && m_bFiresUnderwater == false)
		{
			WeaponSound(EMPTY);
			m_flNextPrimaryAttack = gpGlobals->curtime + 0.2;
			return;
		}
		else
		{
			// If the firing button was just pressed, reset the firing time
			CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
			if ( pPlayer && pPlayer->m_afButtonPressed & IN_ATTACK )
			{
				 m_flNextPrimaryAttack = gpGlobals->curtime;
			}
			PrimaryAttack();
			return;
		}
	}

	if ( pOwner->m_nButtons & IN_RELOAD && UsesClipsForAmmo1() && !m_bInReload ) 
	{
		// reload when reload is pressed, or if no buttons are down and weapon is empty.
		StartReload();
	}
	else 
	{
		// no fire buttons down
		m_bFireOnEmpty = false;

		if ( !HasAnyAmmo() && m_flNextPrimaryAttack < gpGlobals->curtime ) 
		{
			// weapon isn't useable, switch.
			if ( !(GetWeaponFlags() & ITEM_FLAG_NOAUTOSWITCHEMPTY) && pOwner->SwitchToNextBestWeapon( this ) )
			{
				m_flNextPrimaryAttack = gpGlobals->curtime + 0.3;
				return;
			}
		}
		else
		{
			// weapon is useable. Reload if empty and weapon has waited as long as it has to after firing
			if ( m_iClip1 <= 0 && !(GetWeaponFlags() & ITEM_FLAG_NOAUTORELOAD) && m_flNextPrimaryAttack < gpGlobals->curtime )
			{
				if (StartReload())
				{
					// if we've successfully started to reload, we're done
					return;
				}
			}
		}

		if (m_flNextPrimaryAttack < gpGlobals->curtime)
		{
			WeaponIdle();
		}
		return;
	}
}



//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponXM1014::CWeaponXM1014( void )
{
	m_bReloadsSingly = true;

	m_fMinRange1		= 0.0;
	m_fMaxRange1		= 500;
	m_fMinRange2		= 0.0;
	m_fMaxRange2		= 200;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponXM1014::ItemHolsterFrame( void )
{
	// Must be player held
	if ( GetOwner() && GetOwner()->IsPlayer() == false )
		return;

	// We can't be active
	if ( GetOwner()->GetActiveWeapon() == this )
		return;

	// If it's been longer than three seconds, reload
	if ( ( gpGlobals->curtime - m_flHolsterTime ) > sk_auto_reload_time.GetFloat() )
	{
		// Reset the timer
		m_flHolsterTime = gpGlobals->curtime;
	
		if ( GetOwner() == NULL )
			return;

		if ( m_iClip1 == GetMaxClip1() )
			return;

		// Just load the clip with no animations
		int ammoFill = MIN( (GetMaxClip1() - m_iClip1), GetOwner()->GetAmmoCount( GetPrimaryAmmoType() ) );
		
		GetOwner()->RemoveAmmo( ammoFill, GetPrimaryAmmoType() );
		m_iClip1 += ammoFill;
	}
}

//==================================================
// Purpose: 
//==================================================
/*
void CWeaponXM1014::WeaponIdle( void )
{
	//Only the player fires this way so we can cast
	CBasePlayer *pPlayer = GetOwner()

	if ( pPlayer == NULL )
		return;

	//If we're on a target, play the new anim
	if ( pPlayer->IsOnTarget() )
	{
		SendWeaponAnim( ACT_VM_IDLE_ACTIVE );
	}
}
*/
