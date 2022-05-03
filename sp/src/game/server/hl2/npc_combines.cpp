//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: This is the soldier version of the combine, analogous to the HL1 grunt.
//
//=============================================================================//

#include "cbase.h"
#include "ai_hull.h"
#include "ai_motor.h"
#include "npc_combines.h"
#include "bitstring.h"
#include "engine/IEngineSound.h"
#include "soundent.h"
#include "ndebugoverlay.h"
#include "npcevent.h"
#include "hl2/hl2_player.h"
#include "game.h"
#include "ammodef.h"
#include "explode.h"
#include "ai_memory.h"
#include "Sprite.h"
#include "soundenvelope.h"
#include "weapon_physcannon.h"
#include "hl2_gamerules.h"
#include "gameweaponmanager.h"
#include "vehicle_base.h"
#include "gib.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar	sk_combine_s_health( "sk_combine_s_health","0");
ConVar	sk_combine_s_kick( "sk_combine_s_kick","0");
ConVar	sk_combine_guard_health("sk_combine_guard_health", "0");
ConVar	sk_combine_guard_kick("sk_combine_guard_kick", "0");

extern ConVar sk_plr_dmg_buckshot;	
extern ConVar sk_plr_num_shotgun_pellets;

//Whether or not the combine should spawn health on death
ConVar	combine_spawn_health( "combine_spawn_health", "1" );
ConVar	combine_soldier_spawnwithgrenades("combine_soldier_spawnwithgrenades", "1", FCVAR_ARCHIVE);

LINK_ENTITY_TO_CLASS(npc_combine_s, CNPC_CombineS);
LINK_ENTITY_TO_CLASS(npc_combine_e, CNPC_CombineS);
LINK_ENTITY_TO_CLASS(npc_combine_p, CNPC_CombineS);
LINK_ENTITY_TO_CLASS(npc_combine_shot, CNPC_CombineS);

#define AE_SOLDIER_BLOCK_PHYSICS		20 // trying to block an incoming physics object

extern Activity ACT_WALK_EASY;
extern Activity ACT_WALK_MARCH;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNPC_CombineS::Spawn( void )
{
	if (FClassnameIs(this, "npc_combine_e"))
	{
		AddSpawnFlags(SF_COMBINE_S_ELITE);
	}
	else if (FClassnameIs(this, "npc_combine_p"))
	{
		AddSpawnFlags(SF_COMBINE_S_PRISONGUARD);
	}
	else if (FClassnameIs(this, "npc_combine_shot"))
	{
		AddSpawnFlags(SF_COMBINE_S_SHOTGUNNER);
	}

	Precache();

	SetModel(GetSoldierModel());

	bool grenadeoverride = false;
	//change manhack number
	if (m_pAttributes != NULL)
	{
		grenadeoverride = m_pAttributes->GetBool("grenade_override");
		if (grenadeoverride && combine_soldier_spawnwithgrenades.GetBool())
		{
			int grenades = m_pAttributes->GetInt("grenade_count");
			m_iNumGrenades = grenades;
		}
	}

	//Give him a random amount of grenades on spawn
	if (!grenadeoverride && combine_soldier_spawnwithgrenades.GetBool())
	{
		if (g_pGameRules->IsSkillLevel(SKILL_HARD))
		{
			m_iNumGrenades = random->RandomInt(2, 3);
		}
		else if (g_pGameRules->IsSkillLevel(SKILL_VERYHARD))
		{
			m_iNumGrenades = random->RandomInt(4, 6);
		}
		else if (g_pGameRules->IsSkillLevel(SKILL_NIGHTMARE))
		{
			m_iNumGrenades = random->RandomInt(8, 12);
		}
		else
		{
			m_iNumGrenades = random->RandomInt(0, 2);
		}
	}

	m_fIsAce = false;
	m_iUseMarch = false;

	if (HasSpawnFlags(SF_COMBINE_S_ELITE))
	{
		m_fIsElite = true;
		SetHealth(sk_combine_guard_health.GetFloat());
		SetMaxHealth(sk_combine_guard_health.GetFloat());
		SetKickDamage(sk_combine_guard_kick.GetFloat());
	}
	else
	{
		m_fIsElite = false;
		SetHealth(sk_combine_s_health.GetFloat());
		SetMaxHealth(sk_combine_s_health.GetFloat());
		SetKickDamage(sk_combine_s_kick.GetFloat());
	}

	CapabilitiesAdd( bits_CAP_ANIMATEDFACE );
	CapabilitiesAdd( bits_CAP_MOVE_SHOOT );
	CapabilitiesAdd(bits_CAP_MOVE_JUMP);
	CapabilitiesAdd( bits_CAP_DOORS_GROUP );

	BaseClass::Spawn();

	/*
#if HL2_EPISODIC
	if (m_iUseMarch && !HasSpawnFlags(SF_NPC_START_EFFICIENT))
	{
		Msg( "Soldier %s is set to use march anim, but is not an efficient AI. The blended march anim can only be used for dead-ahead walks!\n", GetDebugName() );
	}
#endif
	*/
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  :
// Output :
//-----------------------------------------------------------------------------
void CNPC_CombineS::Precache()
{
	PrecacheModel(GetSoldierModel());
	PrecacheModel(GetGibModel(APPENDAGE_DECAP_BODY));

	//GIBS!
	PrecacheModel(GetGibModel(APPENDAGE_HEAD));
	PrecacheModel(GetGibModel(APPENDAGE_TORSO));
	PrecacheModel(GetGibModel(APPENDAGE_PELVIS));
	PrecacheModel(GetGibModel(APPENDAGE_ARML));
	PrecacheModel(GetGibModel(APPENDAGE_ARMR));
	PrecacheModel(GetGibModel(APPENDAGE_LEGL));
	PrecacheModel(GetGibModel(APPENDAGE_LEGR));

	UTIL_PrecacheOther( "item_healthvial" );
	UTIL_PrecacheOther( "weapon_frag" );
	UTIL_PrecacheOther("item_ammo_ar2_altfire");
	UTIL_PrecacheOther("item_ammo_smg1_grenade");

	BaseClass::Precache();
}

const char* CNPC_CombineS::GetSoldierModel()
{
	if (HasSpawnFlags(SF_COMBINE_S_ELITE))
	{
		return "models/combine_super_soldier.mdl";
	}
	else if (HasSpawnFlags(SF_COMBINE_S_SHOTGUNNER))
	{
		return "models/combine_soldier_shotgunner.mdl";
	}
	else if (HasSpawnFlags(SF_COMBINE_S_PRISONGUARD))
	{
		return "models/combine_soldier_prisonguard.mdl";
	}
	else
	{
		return "models/combine_soldier.mdl";
	}
}

const char* CNPC_CombineS::GetGibModel(appendage_t appendage)
{
	switch (appendage)
	{
		case APPENDAGE_HEAD:
			if (HasSpawnFlags(SF_COMBINE_S_ELITE))
			{
				return "models/gibs/soldier_elite_head.mdl";
			}
			else if (HasSpawnFlags(SF_COMBINE_S_SHOTGUNNER))
			{
				return "models/gibs/soldier_shotgunner_head.mdl";
			}
			else if (HasSpawnFlags(SF_COMBINE_S_PRISONGUARD))
			{
				return "models/gibs/soldier_prisonguard_head.mdl";
			}
			else
			{
				return "models/gibs/soldier_head.mdl";
			}
			break;
		case APPENDAGE_TORSO:
			if (HasSpawnFlags(SF_COMBINE_S_ELITE))
			{
				return "models/gibs/soldier_elite_torso.mdl";
			}
			else if (HasSpawnFlags(SF_COMBINE_S_SHOTGUNNER))
			{
				return "models/gibs/soldier_shotgunner_torso.mdl";
			}
			else if (HasSpawnFlags(SF_COMBINE_S_PRISONGUARD))
			{
				return "models/gibs/soldier_prisonguard_torso.mdl";
			}
			else
			{
				return "models/gibs/soldier_torso.mdl";
			}
			break;
		case APPENDAGE_PELVIS:
			if (HasSpawnFlags(SF_COMBINE_S_ELITE))
			{
				return "models/gibs/soldier_elite_pelvis.mdl";
			}
			else if (HasSpawnFlags(SF_COMBINE_S_SHOTGUNNER))
			{
				return "models/gibs/soldier_shotgunner_pelvis.mdl";
			}
			else if (HasSpawnFlags(SF_COMBINE_S_PRISONGUARD))
			{
				return "models/gibs/soldier_prisonguard_pelvis.mdl";
			}
			else
			{
				return "models/gibs/soldier_pelvis.mdl";
			}
			break;
		case APPENDAGE_ARML:
			if (HasSpawnFlags(SF_COMBINE_S_ELITE))
			{
				return "models/gibs/soldier_elite_left_arm.mdl";
			}
			else if (HasSpawnFlags(SF_COMBINE_S_SHOTGUNNER))
			{
				return "models/gibs/soldier_shotgunner_left_arm.mdl";
			}
			else if (HasSpawnFlags(SF_COMBINE_S_PRISONGUARD))
			{
				return "models/gibs/soldier_prisonguard_left_arm.mdl";
			}
			else
			{
				return "models/gibs/soldier_left_arm.mdl";
			}
			break;
		case APPENDAGE_ARMR:
			if (HasSpawnFlags(SF_COMBINE_S_ELITE))
			{
				return "models/gibs/soldier_elite_right_arm.mdl";
			}
			else if (HasSpawnFlags(SF_COMBINE_S_SHOTGUNNER))
			{
				return "models/gibs/soldier_shotgunner_right_arm.mdl";
			}
			else if (HasSpawnFlags(SF_COMBINE_S_PRISONGUARD))
			{
				return "models/gibs/soldier_prisonguard_right_arm.mdl";
			}
			else
			{
				return "models/gibs/soldier_right_arm.mdl";
			}
			break;
		case APPENDAGE_LEGR:
			if (HasSpawnFlags(SF_COMBINE_S_ELITE))
			{
				return "models/gibs/soldier_elite_right_leg.mdl";
			}
			else if (HasSpawnFlags(SF_COMBINE_S_SHOTGUNNER))
			{
				return "models/gibs/soldier_shotgunner_right_leg.mdl";
			}
			else if (HasSpawnFlags(SF_COMBINE_S_PRISONGUARD))
			{
				return "models/gibs/soldier_prisonguard_right_leg.mdl";
			}
			else
			{
				return "models/gibs/soldier_right_leg.mdl";
			}
			break;
		case APPENDAGE_LEGL:
			if (HasSpawnFlags(SF_COMBINE_S_ELITE))
			{
				return "models/gibs/soldier_elite_left_leg.mdl";
			}
			else if (HasSpawnFlags(SF_COMBINE_S_SHOTGUNNER))
			{
				return "models/gibs/soldier_shotgunner_left_leg.mdl";
			}
			else if (HasSpawnFlags(SF_COMBINE_S_PRISONGUARD))
			{
				return "models/gibs/soldier_prisonguard_left_leg.mdl";
			}
			else
			{
				return "models/gibs/soldier_left_leg.mdl";
			}
			break;
		default:
		case APPENDAGE_DECAP_BODY:
			if (HasSpawnFlags(SF_COMBINE_S_ELITE))
			{
				return "models/gibs/combine_super_soldier_beheaded.mdl";
			}
			else if (HasSpawnFlags(SF_COMBINE_S_SHOTGUNNER))
			{
				return "models/gibs/combine_shotgunner_beheaded.mdl";
			}
			else if (HasSpawnFlags(SF_COMBINE_S_PRISONGUARD))
			{
				return "models/gibs/combine_prisonguard_beheaded.mdl";
			}
			else
			{
				return "models/gibs/combine_soldier_beheaded.mdl";
			}
			break;
	}

	return "";
}

bool CNPC_CombineS::CorpseDecapitate(const CTakeDamageInfo& info)
{
	bool gibs = true;
	if (m_pAttributes != NULL)
	{
		gibs = m_pAttributes->GetBool("gibs");
	}

	if (!(g_Language.GetInt() == LANGUAGE_GERMAN || UTIL_IsLowViolence()) && g_fr_headshotgore.GetBool() && gibs)
	{
		if ((info.GetDamageType() & (DMG_SNIPER | DMG_BUCKSHOT)) && !(info.GetDamageType() & DMG_NEVERGIB))
		{
			SetModel(GetGibModel(APPENDAGE_DECAP_BODY));

			if (m_pAttributes != NULL)
			{
				m_pAttributes->SwitchEntityModel(this, "body_decap_model", STRING(this->GetModelName()));
				m_pAttributes->SwitchEntityColor(this, "new_color");
			}

			DispatchParticleEffect("smod_headshot_r", PATTACH_POINT_FOLLOW, this, "bloodspurt", true);
			SpawnBlood(GetAbsOrigin(), g_vecAttackDir, BloodColor(), info.GetDamage());
			CGib::SpawnSpecificStickyGibs(this, 6, 150, 450, "models/gibs/pgib_p3.mdl", 6);
			CGib::SpawnSpecificStickyGibs(this, 6, 150, 450, "models/gibs/pgib_p4.mdl", 6);
			EmitSound("Gore.Headshot");
			m_bNoDeathSound = true;
			m_iHealth = 0;
			Event_Killed(info);
			g_pGameRules->iHeadshotCount += 1;
			// Handle all clients
			for (int i = 1; i <= gpGlobals->maxClients; i++)
			{
				CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);

				if (pPlayer != NULL)
				{
					if (g_fr_economy.GetBool())
					{
						pPlayer->AddMoney(7);
					}
					if (!g_fr_classic.GetBool())
					{
						pPlayer->AddXP(9);
					}
				}
			}

			return true;
		}
		else if ((info.GetDamageType() & (DMG_SLASH)) && !(info.GetDamageType() & DMG_NEVERGIB))
		{
			SetModel(GetGibModel(APPENDAGE_DECAP_BODY));

			if (m_pAttributes != NULL)
			{
				m_pAttributes->SwitchEntityModel(this, "body_decap_model", STRING(this->GetModelName()));
				m_pAttributes->SwitchEntityColor(this, "new_color");
			}

			DispatchParticleEffect("smod_blood_decap_r", PATTACH_POINT_FOLLOW, this, "bloodspurt", true);
			SpawnBlood(GetAbsOrigin(), g_vecAttackDir, BloodColor(), info.GetDamage());
			CBaseEntity* pHeadGib = CGib::SpawnSpecificSingleGib(this, 150, 450, GetGibModel(APPENDAGE_HEAD), 6);

			if (pHeadGib)
			{
				if (m_pAttributes != NULL)
				{
					m_pAttributes->SwitchEntityModel(pHeadGib, "head_gib_model", STRING(pHeadGib->GetModelName()));
					m_pAttributes->SwitchEntityColor(pHeadGib, "new_color");
				}
			}

			CGib::SpawnSpecificStickyGibs(this, 3, 150, 450, "models/gibs/pgib_p4.mdl", 6);
			EmitSound("Gore.Headshot");
			m_bNoDeathSound = true;
			m_iHealth = 0;
			Event_Killed(info);
			// Handle all clients
			for (int i = 1; i <= gpGlobals->maxClients; i++)
			{
				CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);

				if (pPlayer != NULL)
				{
					if (g_fr_economy.GetBool())
					{
						pPlayer->AddMoney(7);
					}
					if (!g_fr_classic.GetBool())
					{
						pPlayer->AddXP(9);
					}
				}
			}

			return true;
		}
	}

	return false;
}

bool CNPC_CombineS::CorpseGib(const CTakeDamageInfo& info)
{
	bool gibs = true;
	if (m_pAttributes != NULL)
	{
		gibs = m_pAttributes->GetBool("gibs");
	}

	if (!(g_Language.GetInt() == LANGUAGE_GERMAN || UTIL_IsLowViolence()) && info.GetDamageType() & (DMG_BLAST) && !(info.GetDamageType() & (DMG_DISSOLVE)) && !PlayerHasMegaPhysCannon() && gibs)
	{
		if (IsCurSchedule(SCHED_NPC_FREEZE))
		{
			// We're frozen; don't die.
			return false;
		}

		Vector vecDamageDir = info.GetDamageForce();
		SpawnBlood(GetAbsOrigin(), g_vecAttackDir, BloodColor(), info.GetDamage());
		DispatchParticleEffect("smod_blood_gib_r", GetAbsOrigin(), GetAbsAngles(), this);
		EmitSound("Gore.Headshot");
		float flFadeTime = 25.0;

		CBaseEntity* pHeadGib = CGib::SpawnSpecificSingleGib(this, 750, 1500, GetGibModel(APPENDAGE_HEAD), flFadeTime);

		if (pHeadGib)
		{
			if (m_pAttributes != NULL)
			{
				m_pAttributes->SwitchEntityModel(pHeadGib, "head_gib_model", STRING(pHeadGib->GetModelName()));
				m_pAttributes->SwitchEntityColor(pHeadGib, "new_color");
			}
		}

		Vector vecRagForce;
		vecRagForce.x = random->RandomFloat(-400, 400);
		vecRagForce.y = random->RandomFloat(-400, 400);
		vecRagForce.z = random->RandomFloat(0, 250);

		Vector vecRagDmgForce = (vecRagForce + vecDamageDir);

		CBaseEntity* pLeftArmGib = CreateRagGib(this, GetGibModel(APPENDAGE_ARML), GetAbsOrigin(), GetAbsAngles(), vecRagDmgForce, flFadeTime, IsOnFire());
		if (pLeftArmGib)
		{
			color32 color = pLeftArmGib->GetRenderColor();
			pLeftArmGib->SetRenderColor(color.r, color.g, color.b, color.a);

			if (m_pAttributes != NULL)
			{
				m_pAttributes->SwitchEntityModel(pLeftArmGib, "left_arm_gib_model", STRING(pLeftArmGib->GetModelName()));
				m_pAttributes->SwitchEntityColor(pLeftArmGib, "new_color");
			}
		}

		CBaseEntity* pRightArmGib = CreateRagGib(this, GetGibModel(APPENDAGE_ARMR), GetAbsOrigin(), GetAbsAngles(), vecRagDmgForce, flFadeTime, IsOnFire());
		if (pRightArmGib)
		{
			color32 color = pRightArmGib->GetRenderColor();
			pRightArmGib->SetRenderColor(color.r, color.g, color.b, color.a);

			if (m_pAttributes != NULL)
			{
				m_pAttributes->SwitchEntityModel(pRightArmGib, "right_arm_gib_model", STRING(pRightArmGib->GetModelName()));
				m_pAttributes->SwitchEntityColor(pRightArmGib, "new_color");
			}
		}

		CBaseEntity* pTorsoGib = CreateRagGib(this, GetGibModel(APPENDAGE_TORSO), GetAbsOrigin(), GetAbsAngles(), vecRagDmgForce, flFadeTime, IsOnFire());
		if (pTorsoGib)
		{
			color32 color = pTorsoGib->GetRenderColor();
			pTorsoGib->SetRenderColor(color.r, color.g, color.b, color.a);

			if (m_pAttributes != NULL)
			{
				m_pAttributes->SwitchEntityModel(pTorsoGib, "torso_gib_model", STRING(pTorsoGib->GetModelName()));
				m_pAttributes->SwitchEntityColor(pTorsoGib, "new_color");
			}
		}

		CBaseEntity* pPelvisGib = CreateRagGib(this, GetGibModel(APPENDAGE_PELVIS), GetAbsOrigin(), GetAbsAngles(), vecRagDmgForce, flFadeTime, IsOnFire());
		if (pPelvisGib)
		{
			color32 color = pPelvisGib->GetRenderColor();
			pPelvisGib->SetRenderColor(color.r, color.g, color.b, color.a);

			if (m_pAttributes != NULL)
			{
				m_pAttributes->SwitchEntityModel(pPelvisGib, "pelvis_gib_model", STRING(pPelvisGib->GetModelName()));
				m_pAttributes->SwitchEntityColor(pPelvisGib, "new_color");
			}
		}

		CBaseEntity* pLeftLegGib = CreateRagGib(this, GetGibModel(APPENDAGE_LEGL), GetAbsOrigin(), GetAbsAngles(), vecRagDmgForce, flFadeTime, IsOnFire());
		if (pLeftLegGib)
		{
			color32 color = pLeftLegGib->GetRenderColor();
			pLeftLegGib->SetRenderColor(color.r, color.g, color.b, color.a);

			if (m_pAttributes != NULL)
			{
				m_pAttributes->SwitchEntityModel(pLeftLegGib, "left_leg_gib_model", STRING(pLeftLegGib->GetModelName()));
				m_pAttributes->SwitchEntityColor(pLeftLegGib, "new_color");
			}
		}

		CBaseEntity* pRightLegGib = CreateRagGib(this, GetGibModel(APPENDAGE_LEGR), GetAbsOrigin(), GetAbsAngles(), vecRagDmgForce, flFadeTime, IsOnFire());
		if (pRightLegGib)
		{
			color32 color = pRightLegGib->GetRenderColor();
			pRightLegGib->SetRenderColor(color.r, color.g, color.b, color.a);

			if (m_pAttributes != NULL)
			{
				m_pAttributes->SwitchEntityModel(pRightLegGib, "right_leg_gib_model", STRING(pRightLegGib->GetModelName()));
				m_pAttributes->SwitchEntityColor(pRightLegGib, "new_color");
			}
		}

		//now add smaller gibs.
		CGib::SpawnSpecificStickyGibs(this, 3, 750, 1500, "models/gibs/pgib_p3.mdl", flFadeTime);
		CGib::SpawnSpecificStickyGibs(this, 3, 750, 1500, "models/gibs/pgib_p4.mdl", flFadeTime);

		Vector forceVector = CalcDamageForceVector(info);

		// Drop any weapon that I own
		if (m_hActiveWeapon)
		{
			if (VPhysicsGetObject())
			{
				Vector weaponForce = forceVector * VPhysicsGetObject()->GetInvMass();
				Weapon_Drop(m_hActiveWeapon, NULL, &weaponForce);
			}
			else
			{
				Weapon_Drop(m_hActiveWeapon);
			}
		}

		Wake(false);
		m_lifeState = LIFE_DYING;
		CleanupOnDeath(info.GetAttacker());
		StopLoopingSounds();
		DeathSound(info);
		SetCondition(COND_LIGHT_DAMAGE);
		SetIdealState(NPC_STATE_DEAD);
		SetState(NPC_STATE_DEAD);

		// tell owner ( if any ) that we're dead.This is mostly for NPCMaker functionality.
		CBaseEntity* pOwner = GetOwnerEntity();
		if (pOwner)
		{
			if (pOwner->KilledNotice(this))
			{
				SetOwnerEntity(NULL);
			}
		}

		if (info.GetAttacker()->IsPlayer())
		{
			((CSingleplayRules*)GameRules())->NPCKilled(this, info);
		}

		UTIL_Remove(this);
		SetThink(NULL);
		return true;
	}

	return false;
}

void CNPC_CombineS::DeathSound( const CTakeDamageInfo &info )
{
	// NOTE: The response system deals with this at the moment
	if ( GetFlags() & FL_DISSOLVING )
		return;

	if (m_bNoDeathSound)
		return;

	if (IsOnFire())
		return;

	GetSentences()->Speak( "COMBINE_DIE", SENTENCE_PRIORITY_INVALID, SENTENCE_CRITERIA_ALWAYS ); 
}


//-----------------------------------------------------------------------------
// Purpose: Soldiers use CAN_RANGE_ATTACK2 to indicate whether they can throw
//			a grenade. Because they check only every half-second or so, this
//			condition must persist until it is updated again by the code
//			that determines whether a grenade can be thrown, so prevent the 
//			base class from clearing it out. (sjb)
//-----------------------------------------------------------------------------
void CNPC_CombineS::ClearAttackConditions( )
{
	bool fCanRangeAttack2 = HasCondition( COND_CAN_RANGE_ATTACK2 );

	// Call the base class.
	BaseClass::ClearAttackConditions();

	if( fCanRangeAttack2 )
	{
		// We don't allow the base class to clear this condition because we
		// don't sense for it every frame.
		SetCondition( COND_CAN_RANGE_ATTACK2 );
	}
}

void CNPC_CombineS::PrescheduleThink( void )
{
	/*//FIXME: This doesn't need to be in here, it's all debug info
	if( HasCondition( COND_HEAR_PHYSICS_DANGER ) )
	{
		// Don't react unless we see the item!!
		CSound *pSound = NULL;

		pSound = GetLoudestSoundOfType( SOUND_PHYSICS_DANGER );

		if( pSound )
		{
			if( FInViewCone( pSound->GetSoundReactOrigin() ) )
			{
				DevMsg( "OH CRAP!\n" );
				NDebugOverlay::Line( EyePosition(), pSound->GetSoundReactOrigin(), 0, 0, 255, false, 2.0f );
			}
		}
	}
	*/

	BaseClass::PrescheduleThink();
}

//-----------------------------------------------------------------------------
// Purpose: Allows for modification of the interrupt mask for the current schedule.
//			In the most cases the base implementation should be called first.
//-----------------------------------------------------------------------------
void CNPC_CombineS::BuildScheduleTestBits( void )
{
	//Interrupt any schedule with physics danger (as long as I'm not moving or already trying to block)
	if ( m_flGroundSpeed == 0.0 && !IsCurSchedule( SCHED_FLINCH_PHYSICS ) )
	{
		SetCustomInterruptCondition( COND_HEAR_PHYSICS_DANGER );
	}

	BaseClass::BuildScheduleTestBits();
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  :
// Output :
//-----------------------------------------------------------------------------
int CNPC_CombineS::SelectSchedule ( void )
{
	return BaseClass::SelectSchedule();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
float CNPC_CombineS::GetHitgroupDamageMultiplier( int iHitGroup, const CTakeDamageInfo &info )
{
	switch( iHitGroup )
	{
	case HITGROUP_HEAD:
		// Soldiers take double headshot damage
		if (CorpseDecapitate(info))
		{
			//we're dead by this point, lol
			return BaseClass::GetHitgroupDamageMultiplier(iHitGroup, info);
		}
		else
		{
			if (HasSpawnFlags(SF_COMBINE_S_ELITE))
			{
				return 1.5f;
			}
			else
			{
				return 2.0f;
			}
		}
	}

	return BaseClass::GetHitgroupDamageMultiplier( iHitGroup, info );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CNPC_CombineS::HandleAnimEvent( animevent_t *pEvent )
{
	switch( pEvent->event )
	{
	case AE_SOLDIER_BLOCK_PHYSICS:
		DevMsg( "BLOCKING!\n" );
		m_fIsBlocking = true;
		break;

	default:
		BaseClass::HandleAnimEvent( pEvent );
		break;
	}
}

void CNPC_CombineS::OnChangeActivity( Activity eNewActivity )
{
	// Any new sequence stops us blocking.
	m_fIsBlocking = false;

	BaseClass::OnChangeActivity( eNewActivity );

	if (m_iUseMarch)
	{
		SetPoseParameter("casual", 1.0);
	}
}

void CNPC_CombineS::OnListened()
{
	BaseClass::OnListened();

	if ( HasCondition( COND_HEAR_DANGER ) && HasCondition( COND_HEAR_PHYSICS_DANGER ) )
	{
		if ( HasInterruptCondition( COND_HEAR_DANGER ) )
		{
			ClearCondition( COND_HEAR_PHYSICS_DANGER );
		}
	}

	// debugging to find missed schedules
#if 0
	if ( HasCondition( COND_HEAR_DANGER ) && !HasInterruptCondition( COND_HEAR_DANGER ) )
	{
		DevMsg("Ignore danger in %s\n", GetCurSchedule()->GetName() );
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &info - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
void CNPC_CombineS::Event_Killed( const CTakeDamageInfo &info )
{
	if (CorpseGib(info))
	{
		return;
	}

	// Don't bother if we've been told not to, or the player has a megaphyscannon
	if ( combine_spawn_health.GetBool() == false || PlayerHasMegaPhysCannon() )
	{
		BaseClass::Event_Killed( info );
		return;
	}

	CBasePlayer *pPlayer = ToBasePlayer( info.GetAttacker() );

	if ( !pPlayer )
	{
		CPropVehicleDriveable *pVehicle = dynamic_cast<CPropVehicleDriveable *>( info.GetAttacker() ) ;
		if ( pVehicle && pVehicle->GetDriver() && pVehicle->GetDriver()->IsPlayer() )
		{
			pPlayer = assert_cast<CBasePlayer *>( pVehicle->GetDriver() );
		}
	}

	if ( pPlayer != NULL )
	{
		if (m_hActiveWeapon && HasSpawnFlags(SF_COMBINE_S_ELITE))
		{
			// Elites drop alt-fire ammo, so long as they weren't killed by dissolving.
#ifdef HL2_EPISODIC
			if (HasSpawnFlags(SF_COMBINE_NO_AR2DROP) == false)
#endif
			{
				CBaseEntity* pItem = NULL;

				if (FClassnameIs(GetActiveWeapon(), "weapon_ar2"))
				{
					pItem = DropItem("item_ammo_ar2_altfire", WorldSpaceCenter() + RandomVector(-4, 4), RandomAngle(0, 360));
				}
				else if (FClassnameIs(GetActiveWeapon(), "weapon_smg1"))
				{
					pItem = DropItem("item_ammo_smg1_grenade", WorldSpaceCenter() + RandomVector(-4, 4), RandomAngle(0, 360));
				}

				if (pItem)
				{
					IPhysicsObject* pObj = pItem->VPhysicsGetObject();

					if (pObj)
					{
						Vector			vel = RandomVector(-64.0f, 64.0f);
						AngularImpulse	angImp = RandomAngularImpulse(-300.0f, 300.0f);

						vel[2] = 0.0f;
						pObj->AddVelocity(&vel, &angImp);
					}

					if (info.GetDamageType() & DMG_DISSOLVE)
					{
						CBaseAnimating* pAnimating = dynamic_cast<CBaseAnimating*>(pItem);

						if (pAnimating)
						{
							pAnimating->Dissolve(NULL, gpGlobals->curtime, false, ENTITY_DISSOLVE_NORMAL);
						}
					}
					else
					{
						WeaponManager_AddManaged(pItem);
					}
				}
			}
		}

		CHalfLife2 *pHL2GameRules = static_cast<CHalfLife2 *>(g_pGameRules);

		// Attempt to drop health
		if ( pHL2GameRules->NPC_ShouldDropHealth( pPlayer ) )
		{
			DropItem( "item_healthvial", WorldSpaceCenter()+RandomVector(-4,4), RandomAngle(0,360) );
			pHL2GameRules->NPC_DroppedHealth();
		}
		
		if ( HasSpawnFlags( SF_COMBINE_NO_GRENADEDROP ) == false )
		{
			// Attempt to drop a grenade
			if ( pHL2GameRules->NPC_ShouldDropGrenade( pPlayer ) )
			{
				DropItem( "weapon_frag", WorldSpaceCenter()+RandomVector(-4,4), RandomAngle(0,360) );
				pHL2GameRules->NPC_DroppedGrenade();
			}
		}
	}

	BaseClass::Event_Killed( info );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &info - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CNPC_CombineS::IsLightDamage( const CTakeDamageInfo &info )
{
	return BaseClass::IsLightDamage( info );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &info - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CNPC_CombineS::IsHeavyDamage( const CTakeDamageInfo &info )
{
	// Combine considers AR2 fire to be heavy damage
	if ( info.GetAmmoType() == GetAmmoDef()->Index("AR2") )
		return true;

	// 357 rounds are heavy damage
	if ( info.GetAmmoType() == GetAmmoDef()->Index("357") )
		return true;

	// Shotgun blasts where at least half the pellets hit me are heavy damage
	if ( info.GetDamageType() & DMG_BUCKSHOT )
	{
		int iHalfMax = sk_plr_dmg_buckshot.GetFloat() * sk_plr_num_shotgun_pellets.GetInt() * 0.5;
		if ( info.GetDamage() >= iHalfMax )
			return true;
	}

	// Rollermine shocks
	if( (info.GetDamageType() & DMG_SHOCK) && hl2_episodic.GetBool() )
	{
		return true;
	}

	return BaseClass::IsHeavyDamage( info );
}

#if HL2_EPISODIC
//-----------------------------------------------------------------------------
// Purpose: Translate base class activities into combot activites
//-----------------------------------------------------------------------------
Activity CNPC_CombineS::NPC_TranslateActivity( Activity eNewActivity )
{
	// If the special ep2_outland_05 "use march" flag is set, use the more casual marching anim.
	if ( m_iUseMarch && eNewActivity == ACT_WALK )
	{
		eNewActivity = ACT_WALK_MARCH;
	}

	return BaseClass::NPC_TranslateActivity( eNewActivity );
}


//---------------------------------------------------------
// Save/Restore
//---------------------------------------------------------
BEGIN_DATADESC( CNPC_CombineS )

	DEFINE_KEYFIELD( m_iUseMarch, FIELD_INTEGER, "usemarch" ),

END_DATADESC()
#endif