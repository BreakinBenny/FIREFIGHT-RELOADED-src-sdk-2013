//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "npcevent.h"
#include "basehlcombatweapon_shared.h"
#include "basecombatcharacter.h"
#include "ai_basenpc.h"
#include "player.h"
#include "gamerules.h"
#include "in_buttons.h"
#include "soundent.h"
#include "game.h"
#include "vstdlib/random.h"
#include "engine/IEngineSound.h"
#include "IEffects.h"
#include "te_effect_dispatch.h"
#include "Sprite.h"
#include "SpriteTrail.h"
#include "beam_shared.h"
#include "rumble_shared.h"
#include "gamestats.h"
#include "decals.h"
#include "hl2_player.h"
#include "func_break.h"
#include "func_breakablesurf.h"
#include "stickybolt.h"
#include "ammodef.h"
#include "weapon_crossbow.h"

#ifdef PORTAL
	#include "portal_util_shared.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar sk_plr_dmg_crossbow;
extern ConVar sk_npc_dmg_crossbow;

ConVar	chargebow_charge_time("chargebow_charge_time", "3", FCVAR_ARCHIVE);
ConVar	chargebow_uncharged_velocity_reduction("chargebow_uncharged_velocity_reduction", "0.5", FCVAR_ARCHIVE);

//-----------------------------------------------------------------------------
// CWeaponChargebow
//-----------------------------------------------------------------------------

class CWeaponChargebow : public CBaseHLCombatWeapon
{
	DECLARE_CLASS( CWeaponChargebow, CBaseHLCombatWeapon );
public:

	CWeaponChargebow( void );
	
	virtual void	Precache( void );
	virtual void	PrimaryAttack( void );
	virtual bool	Deploy( void );
	virtual void	Drop( const Vector &vecVelocity );
	virtual bool	Holster( CBaseCombatWeapon *pSwitchingTo = NULL );
	virtual bool	Reload( void );
	virtual void	ItemPostFrame( void );
	virtual void	Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );
	virtual bool	SendWeaponAnim( int iActivity );
	
	bool	ShouldDisplayHUDHint() { return true; }
    
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();
	DECLARE_ACTTABLE();

private:
	void	FireBolt( void );
	void	DoLoadEffect( void );

private:

	bool				m_bMustReload;
	bool				m_bCharged;
	bool				m_fDrawbackFinished;
	float				m_fChargeTime;
	bool				m_bPlayedChargeSound;
	float				m_fLastChargeEffectTime;
};

acttable_t CWeaponChargebow::m_acttable[] =
{
	{ ACT_HL2MP_IDLE, ACT_HL2MP_IDLE_CHARGEBOW, false },
	{ ACT_HL2MP_RUN, ACT_HL2MP_RUN_CHARGEBOW, false },
	{ ACT_HL2MP_IDLE_CROUCH, ACT_HL2MP_IDLE_CROUCH_CHARGEBOW, false },
	{ ACT_HL2MP_WALK_CROUCH, ACT_HL2MP_WALK_CROUCH_CHARGEBOW, false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK, ACT_HL2MP_GESTURE_RANGE_ATTACK_CHARGEBOW, false },
	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_CHARGEBOW,				false },
	{ ACT_HL2MP_JUMP, ACT_HL2MP_JUMP_CHARGEBOW, false },
	{ ACT_RANGE_ATTACK1, ACT_RANGE_ATTACK_SHOTGUN, false },
};

IMPLEMENT_ACTTABLE(CWeaponChargebow);

LINK_ENTITY_TO_CLASS( weapon_chargebow, CWeaponChargebow );

PRECACHE_WEAPON_REGISTER( weapon_chargebow );

IMPLEMENT_SERVERCLASS_ST(CWeaponChargebow, DT_WeaponChargebow)
END_SEND_TABLE()

BEGIN_DATADESC( CWeaponChargebow )

	DEFINE_FIELD( m_bMustReload,	FIELD_BOOLEAN ),
	DEFINE_FIELD(m_fDrawbackFinished, FIELD_BOOLEAN),
	DEFINE_FIELD(m_bCharged, FIELD_BOOLEAN),
	DEFINE_FIELD(m_fChargeTime, FIELD_TIME),
	DEFINE_FIELD(m_fLastChargeEffectTime, FIELD_TIME),

END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponChargebow::CWeaponChargebow( void )
{
	m_bReloadsSingly	= true;
	m_bFiresUnderwater	= true;
	m_bAltFiresUnderwater = true;
	m_bMustReload		= false;
	m_bCharged			= false;
	m_fChargeTime = gpGlobals->curtime + FLT_MAX;
	m_fDrawbackFinished = false;
	m_bPlayedChargeSound = false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponChargebow::Precache( void )
{
	UTIL_PrecacheOther( "chargebow_bolt" );

	PrecacheScriptSound( "Weapon_Crossbow.BoltHitBody" );
	PrecacheScriptSound( "Weapon_Crossbow.BoltHitWorld" );
	PrecacheScriptSound( "Weapon_Crossbow.BoltSkewer" );

	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponChargebow::PrimaryAttack( void )
{
	CBaseCombatCharacter* pOwner = GetOwner();

	if (pOwner == NULL)
	{
		return;
	}

	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());;

	if (!pPlayer)
		return;

	pPlayer->RumbleEffect(RUMBLE_CROWBAR_SWING, 0, RUMBLE_FLAG_RESTART);

	// Note that this is a primary attack and prepare the grenade attack to pause.
	SendWeaponAnim(ACT_VM_PULLBACK);

	// Put both of these off indefinitely. We do not know how long
	// the player will hold the grenade.
	//m_flTimeWeaponIdle = FLT_MAX;
	m_flNextPrimaryAttack = FLT_MAX;

	//start the charge timer.
	m_fChargeTime = gpGlobals->curtime + chargebow_charge_time.GetFloat();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CWeaponChargebow::Reload( void )
{
	if ( BaseClass::Reload() )
	{
		m_bMustReload = false;
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponChargebow::ItemPostFrame( void )
{
	if ( m_bMustReload && HasWeaponIdleTimeElapsed() )
	{
		Reload();
	}

	CBasePlayer* pOwner = ToBasePlayer(GetOwner());

	if (gpGlobals->curtime > m_fChargeTime)
	{
		// Shoot some sparks
		if (gpGlobals->curtime > m_fLastChargeEffectTime)
		{
			if (pOwner)
			{
				pOwner->RumbleEffect(RUMBLE_PORTAL_PLACEMENT_FAILURE, 0, RUMBLE_FLAG_RESTART);
			}
			DoLoadEffect();
			m_fLastChargeEffectTime = gpGlobals->curtime + 0.3f;
		}

		if (!m_bPlayedChargeSound)
		{
			WeaponSound(SPECIAL1);
			m_bPlayedChargeSound = true;
		}
		m_bCharged = true;
	}

	if (m_fDrawbackFinished)
	{
		if (pOwner)
		{
			if (!(pOwner->m_nButtons & IN_ATTACK))
			{
				FireBolt();

				// Signal a reload
				m_bMustReload = true;

				SetWeaponIdleTime(gpGlobals->curtime + SequenceDuration(ACT_VM_PRIMARYATTACK));

				CBasePlayer* pPlayer = ToBasePlayer(GetOwner());
				if (pPlayer)
				{
					m_iPrimaryAttacks++;
					gamestats->Event_WeaponFired(pPlayer, true, GetClassname());
				}
				m_fDrawbackFinished = false;
				m_fChargeTime = gpGlobals->curtime + FLT_MAX;
				m_bPlayedChargeSound = false;
				m_bCharged = false;
			}
		}
	}

	BaseClass::ItemPostFrame();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponChargebow::FireBolt( void )
{
	if ( m_iClip1 <= 0 )
	{
		if ( !m_bFireOnEmpty )
		{
			Reload();
		}
		else
		{
			WeaponSound( EMPTY );
			m_flNextPrimaryAttack = 0.15;
		}

		return;
	}

	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	
	if ( pOwner == NULL )
		return;

	Vector vecAiming	= pOwner->GetAutoaimVector( 0 );
	Vector vecSrc		= pOwner->Weapon_ShootPosition();

	QAngle angAiming;
	VectorAngles( vecAiming, angAiming );

#if defined(HL2_EPISODIC)
	// !!!HACK - the other piece of the Alyx crossbow bolt hack for Outland_10 (see ::BoltTouch() for more detail)
	if( FStrEq(STRING(gpGlobals->mapname), "ep2_outland_10") )
	{
		trace_t tr;
		UTIL_TraceLine( vecSrc, vecSrc + vecAiming * 24.0f, MASK_SOLID, pOwner, COLLISION_GROUP_NONE, &tr );

		if( tr.m_pEnt != NULL && tr.m_pEnt->Classify() == CLASS_PLAYER_ALLY_VITAL )
		{
			// If Alyx is right in front of the player, make sure the bolt starts outside of the player's BBOX, or the bolt
			// will instantly collide with the player after the owner of the bolt is switched to Alyx in ::BoltTouch(). We 
			// avoid this altogether by making it impossible for the bolt to collide with the player.
			vecSrc += vecAiming * 24.0f;
		}
	}
#endif

	CCrossbowBolt *pBolt = CCrossbowBolt::BoltCreate( vecSrc, angAiming, pOwner, BOLT_CHARGEBOW, m_bCharged);

	if (pBolt)
	{
		if (pOwner->GetWaterLevel() == 3)
		{
			pBolt->SetAbsVelocity((vecAiming * BOLT_WATER_VELOCITY) * (!m_bCharged ? chargebow_uncharged_velocity_reduction.GetFloat() : 1));
		}
		else
		{
			pBolt->SetAbsVelocity((vecAiming * BOLT_AIR_VELOCITY) * (!m_bCharged ? chargebow_uncharged_velocity_reduction.GetFloat() : 1));
		}
	}

	m_iClip1--;

	pOwner->ViewPunch( QAngle( -2, 0, 0 ) );

	WeaponSound( SINGLE );
	WeaponSound( SPECIAL2 );

	CSoundEnt::InsertSound( SOUND_COMBAT, GetAbsOrigin(), 200, 0.2 );

	SendWeaponAnim( ACT_VM_PRIMARYATTACK );

	if ( !m_iClip1 && pOwner->GetAmmoCount( m_iPrimaryAmmoType ) <= 0 )
	{
		// HEV suit - indicate out of ammo condition
		pOwner->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
	}

	m_flNextPrimaryAttack = m_flNextSecondaryAttack	= gpGlobals->curtime + 0.75;

	DoLoadEffect();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CWeaponChargebow::Deploy( void )
{
	m_fChargeTime = gpGlobals->curtime + FLT_MAX;
	m_fDrawbackFinished = false;
	m_bPlayedChargeSound = false;
	m_bCharged = false;

	if ( m_iClip1 <= 0 )
	{
		return DefaultDeploy( (char*)GetViewModel(), (char*)GetWorldModel(), ACT_CROSSBOW_DRAW_UNLOADED, (char*)GetAnimPrefix() );
	}

	return BaseClass::Deploy();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pSwitchingTo - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CWeaponChargebow::Holster( CBaseCombatWeapon *pSwitchingTo )
{
	m_fChargeTime = gpGlobals->curtime + FLT_MAX;
	m_fDrawbackFinished = false;
	m_bPlayedChargeSound = false;
	m_bCharged = false;
	return BaseClass::Holster( pSwitchingTo );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponChargebow::DoLoadEffect( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );

	if ( pOwner == NULL )
		return;

	CBaseViewModel *pViewModel = pOwner->GetViewModel();

	if ( pViewModel == NULL )
		return;

	CEffectData	data;

	data.m_nEntIndex = pViewModel->entindex();
	data.m_vOrigin = GetAbsOrigin();
	data.m_nAttachmentIndex = 1;

	DispatchEffect( "CrossbowLoad", data );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pEvent - 
//			*pOperator - 
//-----------------------------------------------------------------------------
void CWeaponChargebow::Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	switch( pEvent->event )
	{
		case EVENT_WEAPON_SEQUENCE_FINISHED:
			SendWeaponAnim(ACT_VM_PULLBACK_HIGH);
			if (pPlayer)
			{
				pPlayer->RumbleEffect(RUMBLE_CROWBAR_SWING, 0, RUMBLE_FLAG_STOP);
			}
			m_fDrawbackFinished = true;
			break;
		default:
			BaseClass::Operator_HandleAnimEvent( pEvent, pOperator );
			break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Set the desired activity for the weapon and its viewmodel counterpart
// Input  : iActivity - activity to play
//-----------------------------------------------------------------------------
bool CWeaponChargebow::SendWeaponAnim( int iActivity )
{
	int newActivity = iActivity;

	// The last shot needs a non-loaded activity
	if ( ( newActivity == ACT_VM_IDLE ) && ( m_iClip1 <= 0 ) )
	{
		newActivity = ACT_VM_FIDGET;
	}

	//For now, just set the ideal activity and be done with it
	return BaseClass::SendWeaponAnim( newActivity );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponChargebow::Drop( const Vector &vecVelocity )
{
	m_fChargeTime = gpGlobals->curtime + FLT_MAX;
	m_fDrawbackFinished = false;
	m_bPlayedChargeSound = false;
	m_bCharged = false;
	BaseClass::Drop( vecVelocity );
}
