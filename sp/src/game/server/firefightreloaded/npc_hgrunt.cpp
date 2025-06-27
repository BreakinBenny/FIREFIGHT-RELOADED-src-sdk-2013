/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
//=========================================================
// hgrunt
//=========================================================

//=========================================================
// Hit groups!	
//=========================================================
/*

  1 - Head
  2 - Stomach
  3 - Gun

*/


#include	"cbase.h"
#include	"beam_shared.h"
#include    "npc_hgrunt.h"
#include	"ai_default.h"
#include	"ai_task.h"
#include	"ai_schedule.h"
#include	"ai_node.h"
#include	"ai_hull.h"
#include	"ai_hint.h"
#include	"ai_memory.h"
#include	"ai_route.h"
#include	"ai_motor.h"
#include	"soundent.h"
#include	"game.h"
#include	"npcevent.h"
#include	"entitylist.h"
#include	"activitylist.h"
#include	"animation.h"
#include	"engine/IEngineSound.h"
#include	"ammodef.h"
#include	"basecombatweapon.h"
#include	"scripted.h"
#include    "grenade_frag.h"
#include    "grenade_ar2.h"
#include	"hl2_gamerules.h"
#include	"te_effect_dispatch.h"

GruntSentences_t CHGruntSentenceLoader::HGruntSentenceMap[] =
{
	{ SENT_GREN, "GREN" }, // grenade scared grunt
	{ SENT_ALERT, "ALERT" }, // sees player
	{ SENT_MONST, "MONST" }, // sees monster
	{ SENT_COVER, "COVER" }, // running to cover
	{ SENT_THROW, "THROW"}, // about to throw grenade
	{ SENT_CHARGE, "CHARGE" }, // running out to get the enemy
	{ SENT_TAUNT, "TAUNT" }, // say rude things
	{ SENT_TAUNT_EASTEREGG, "TAUNT_EASTEREGG"}, // MY ASS IS HEAVY
	{ SENT_CHECK, "CHECK"}, // SQUAD. CHECK. IN.
	{ SENT_QUEST, "QUEST"}, // Who is this guy, "Freeman"?
	{ SENT_IDLE, "IDLE"}, // standing doing nothin'
	{ SENT_CLEAR, "CLEAR"}, // all clear
	{ SENT_ANSWER, "ANSWER"}, // I just know he's been killing my buddies.
	{ SENT_TAUNT_PLAYERHURT, "TAUNT_PLAYERHURT" } // YOU GOT NOTHIN'
};

const char* CHGruntSentenceLoader::GetNameForSentence(int iTeamNumber)
{
	const char* name = "NONE";

	for (int i = 0; i < ARRAYSIZE(HGruntSentenceMap); i++)
	{
		if (HGruntSentenceMap[i].id == iTeamNumber)
		{
			name = HGruntSentenceMap[i].name;
			break;
		}
	}

	return name;
}

enum eGruntPrefixes
{
	PREFIX_HUMAN,
	PREFIX_ROBOT
};

const char* pGruntPrefixes[] =
{
	"HG", // normal ass joe
	"RG" // crazy fuckin' robot body
};

ConVar	sk_hgrunt_health( "sk_hgrunt_health","0");
ConVar	sk_hgrunt_robot_health("sk_hgrunt_robot_health", "0");
ConVar	sk_hgrunt_limp_health("sk_hgrunt_limp_health", "20");
ConVar  sk_hgrunt_kick ( "sk_hgrunt_kick", "0" );
ConVar  sk_hgrunt_shotgun_pellets ( "sk_hgrunt_shotgun_pellets", "0" );
ConVar  sk_hgrunt_grenade_speed ( "sk_hgrunt_grenade_speed", "0" );
ConVar  sk_hgrunt_eastereggtaunt_prob ( "sk_hgrunt_eastereggtaunt_prob", "30" );
ConVar  sk_hgrunt_smg_magsize("sk_hgrunt_smg_magsize", "36");
ConVar  sk_hgrunt_shotgun_magsize("sk_hgrunt_shotgun_magsize", "8");

ConVar hgrunt_hurttaunt("hgrunt_hurttaunt", "1", FCVAR_ARCHIVE, "If true, the hgrunt will taunt at the player if the player's health is under the value of hgrunt_hurttaunt_healthvalue.");
ConVar hgrunt_hurttaunt_frequency("hgrunt_hurttaunt_frequency", "30", FCVAR_NONE, "Changes the frequency of the hgrunt's taunt. Lower values = higher frequency. Higher values = lower frequency.");
ConVar hgrunt_hurttaunt_healthvalue("hgrunt_hurttaunt_healthvalue", "20", FCVAR_NONE, "Sets the minimal amount of health the player needs in order for the hgrunt to taunt at it.");

extern ConVar    sk_npc_dmg_smg1_grenade;

int ACT_GRUNT_LAUNCH_GRENADE;
int ACT_GRUNT_TOSS_GRENADE;
int ACT_GRUNT_MP5_STANDING;
int ACT_GRUNT_MP5_CROUCHING;
int ACT_GRUNT_SHOTGUN_STANDING;
int ACT_GRUNT_SHOTGUN_CROUCHING;

LINK_ENTITY_TO_CLASS( npc_hgrunt, CHGrunt );
LINK_ENTITY_TO_CLASS(npc_hgrunt_friendly, CHGrunt);
LINK_ENTITY_TO_CLASS(npc_hgrunt_robot, CHGrunt);
LINK_ENTITY_TO_CLASS(npc_hgrunt_friendly_robot, CHGrunt);

//---------------------------------------------------------
// Save/Restore
//---------------------------------------------------------
BEGIN_DATADESC( CHGrunt )
	DEFINE_FIELD( m_flNextGrenadeCheck, FIELD_TIME ),
	DEFINE_FIELD( m_flNextPainTime, FIELD_TIME ),
	DEFINE_FIELD( m_flCheckAttackTime, FIELD_FLOAT ),
	DEFINE_FIELD( m_vecTossVelocity, FIELD_VECTOR ),
	DEFINE_FIELD( m_iLastGrenadeCondition, FIELD_INTEGER ),
	DEFINE_FIELD( m_fStanding, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_fFirstEncounter, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_iClipSize, FIELD_INTEGER ),
	DEFINE_FIELD( m_voicePitch, FIELD_INTEGER ),
	DEFINE_FIELD( m_iSentence, FIELD_INTEGER ),
	DEFINE_FIELD(m_fIsFriendly, FIELD_BOOLEAN),
	DEFINE_KEYFIELD( m_iWeapons, FIELD_INTEGER, "weapons" ),
	DEFINE_KEYFIELD( m_SquadName, FIELD_STRING, "netname" ),
    
	DEFINE_FIELD( m_flLastEnemySightTime, FIELD_TIME ),
	DEFINE_FIELD( m_flTalkWaitTime, FIELD_TIME ),

	DEFINE_FIELD(m_fGruntQuestion, FIELD_INTEGER),
	DEFINE_FIELD(m_iSquadIndex, FIELD_INTEGER),
END_DATADESC()

//=========================================================
// Speak Sentence - say your cued up sentence.
//
// Some grunt sentences (take cover and charge) rely on actually
// being able to execute the intended action. It's really lame
// when a grunt says 'COVER ME' and then doesn't move. The problem
// is that the sentences were played when the decision to TRY
// to move to cover was made. Now the sentence is played after 
// we know for sure that there is a valid path. The schedule
// may still fail but in most cases, well after the grunt has 
// started moving.
//=========================================================
void CHGrunt::SpeakSentence( void )
{
	if (!FOkToSpeak())
		return;

	int prefixType = PREFIX_HUMAN;
	float volume = 0.75f;

	if (HasSpawnFlags(SF_GRUNT_ROBOT))
	{
		prefixType = PREFIX_ROBOT;
		volume = 1.0f;
	}

	const char* szPrefix = pGruntPrefixes[prefixType];

	const char* szSentType = CHGruntSentenceLoader::GetNameForSentence(m_iSentence);

	char sentence[128];
	Q_snprintf(sentence, sizeof(sentence), "%s_%s", szPrefix, szSentType);

	DevMsg("Saying %s\n", sentence);
	if (SENTENCEG_PlayRndSz(edict(), sentence, volume, SNDLVL_TALKING, 0, m_voicePitch) != -1)
	{
		JustSpoke();
	}
	else
	{
		DevWarning("Sentence %s not found\n", sentence);
	}
}

void CHGrunt::SetAndSpeakSentence(int sentenceGroup)
{
	m_iSentence = sentenceGroup;
	SpeakSentence();
}

//=========================================================
// IRelationship - overridden because Alien Grunts are 
// Human Grunt's nemesis.
//=========================================================
int CHGrunt::IRelationPriority( CBaseEntity *pTarget )
{
	//I hate alien grunts more than anything.
	if ( pTarget->Classify() == CLASS_ALIEN_MILITARY )
	{
		if ( FClassnameIs( pTarget, "npc_agrunt" ) )
		{
			 return ( BaseClass::IRelationPriority ( pTarget ) + 1 );
		}
	}

	return BaseClass::IRelationPriority( pTarget );
}

//=========================================================
// GibMonster - make gun fly through the air.
//=========================================================
void CHGrunt::Event_Killed( const CTakeDamageInfo &info )
{
	Vector	vecGunPos;
	QAngle	vecGunAngles;

	GetAttachment( "0", vecGunPos, vecGunAngles );

	// switch to body group with no gun.
	SetBodygroup( GUN_GROUP, GUN_NONE );

	// If the gun would drop into a wall, spawn it at our origin
	if( UTIL_PointContents( vecGunPos ) & CONTENTS_SOLID )
	{
		vecGunPos = GetAbsOrigin();
	}

	// now spawn a gun.
	if (FBitSet( m_iWeapons, HGRUNT_SHOTGUN ))
	{
		 DropItem( "weapon_shotgun", vecGunPos, vecGunAngles );
	}
	else
	{
		 DropItem( "weapon_mp5", vecGunPos, vecGunAngles );
	}

	if (FBitSet( m_iWeapons, HGRUNT_GRENADELAUNCHER ))
	{
		DropItem( "item_ammo_smg1_grenade", BodyTarget( GetAbsOrigin() ), vecGunAngles );
	}

	if (FBitSet(m_iWeapons, HGRUNT_HANDGRENADE))
	{
		DropItem("weapon_frag", BodyTarget(GetAbsOrigin()), vecGunAngles);
	}

	CBasePlayer* pPlayer = ToBasePlayer(info.GetAttacker());

	if (pPlayer != NULL)
	{
		CHalfLife2* pHL2GameRules = static_cast<CHalfLife2*>(g_pGameRules);

		// Attempt to drop health
		if (pHL2GameRules->NPC_ShouldDropHealth(pPlayer))
		{
			DropItem("item_healthvial", WorldSpaceCenter() + RandomVector(-4, 4), RandomAngle(0, 360));
			pHL2GameRules->NPC_DroppedHealth();
		}
	}

	BaseClass::Event_Killed( info );
}

//=========================================================
// someone else is talking - don't speak
//=========================================================
bool CHGrunt::FOkToSpeak( void )
{
	// if someone else is talking, don't speak
	if ( gpGlobals->curtime <= m_flTalkWaitTime )
		return false;

	if ( m_spawnflags & SF_NPC_GAG )
	{
		if ( m_NPCState != NPC_STATE_COMBAT )
		{
			// no talking outside of combat if gagged.
			return true;
		}
	}

	return true;
}

//=========================================================
//=========================================================
void CHGrunt::JustSpoke( void )
{
	// most sentences have a time of 3.5 or below.
	m_flTalkWaitTime = gpGlobals->curtime + 3.5f;
}

//=========================================================
// PrescheduleThink - this function runs after conditions
// are collected and before scheduling code is run.
//=========================================================
void CHGrunt::PrescheduleThink ( void )
{
	BaseClass::PrescheduleThink();
	
	if ( m_pSquad && GetEnemy() != NULL )
	{
		CHGrunt* pSquadLeader = NULL;

		if (m_pSquad->GetLeader())
		{
			pSquadLeader = (CHGrunt*)m_pSquad->GetLeader()->MyNPCPointer();
		}
		else
		{
			//No leader? we're the leader.
			pSquadLeader = this;
		}

		if (pSquadLeader)
		{
			if (HasCondition(COND_SEE_ENEMY))
			{
				// update the squad's last enemy sighting time.
				pSquadLeader->m_flLastEnemySightTime = gpGlobals->curtime;
			}
			else
			{
				if (gpGlobals->curtime - pSquadLeader->m_flLastEnemySightTime > 5)
				{
					// been a while since we've seen the enemy
					pSquadLeader->GetEnemies()->MarkAsEluded(GetEnemy());
				}
			}
		}
	}
}

//=========================================================
// FCanCheckAttacks - this is overridden for human grunts
// because they can throw/shoot grenades when they can't see their
// target and the base class doesn't check attacks if the monster
// cannot see its enemy.
//
// !!!BUGBUG - this gets called before a 3-round burst is fired
// which means that a friendly can still be hit with up to 2 rounds. 
// ALSO, grenades will not be tossed if there is a friendly in front,
// this is a bad bug. Friendly machine gun fire avoidance
// will unecessarily prevent the throwing of a grenade as well.
//=========================================================
bool CHGrunt::FCanCheckAttacks( void )
{
	if ( !HasCondition(COND_TOO_CLOSE_TO_ATTACK) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

//=========================================================
// CheckMeleeAttack1
//=========================================================
int CHGrunt::MeleeAttack1Conditions ( float flDot, float flDist )
{
	if (flDist > 64)
		return COND_TOO_FAR_TO_ATTACK;
	else if (flDot < 0.7)
		return COND_NOT_FACING_ATTACK;
	
	return COND_CAN_MELEE_ATTACK1;
}

//=========================================================
// CheckRangeAttack1 - overridden for HGrunt, cause 
// FCanCheckAttacks() doesn't disqualify all attacks based
// on whether or not the enemy is occluded because unlike
// the base class, the HGrunt can attack when the enemy is
// occluded (throw grenade over wall, etc). We must 
// disqualify the machine gun attack if the enemy is occluded.
//=========================================================
int CHGrunt::RangeAttack1Conditions ( float flDot, float flDist )
{
	if ( !HasCondition( COND_ENEMY_OCCLUDED ) && flDist <= 2048 && flDot >= 0.5)
	{
		trace_t	tr;

		if ( flDist <= 64 )
		{
			// kick, but don't shoot at them.
			return COND_NONE;
		}

		Vector vecSrc = Weapon_ShootPosition();

		UTIL_TraceLine( GetAbsOrigin() + GetViewOffset(), GetEnemy()->BodyTarget(GetAbsOrigin() + GetViewOffset()), MASK_SHOT, this, COLLISION_GROUP_NONE, &tr);

		if ( tr.fraction == 1.0 || tr.m_pEnt == GetEnemy() )
		{
			return COND_CAN_RANGE_ATTACK1;
		}
	}

	return COND_NONE;
}

int CHGrunt::RangeAttack2Conditions( float flDot, float flDist  )
{
	m_iLastGrenadeCondition = GetGrenadeConditions( flDot, flDist );
	return m_iLastGrenadeCondition;
}

void CHGrunt::ClearAttackConditions( void )
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

int CHGrunt::GetGrenadeConditions( float flDot, float flDist  )
{
	if ( !FBitSet( m_iWeapons, ( HGRUNT_HANDGRENADE | HGRUNT_GRENADELAUNCHER ) ) )
		  return COND_NONE;

	// assume things haven't changed too much since last time
	if (gpGlobals->curtime < m_flNextGrenadeCheck )
		return m_iLastGrenadeCondition;
	
	if ( m_flGroundSpeed != 0 )
		return COND_NONE;

	CBaseEntity *pEnemy = GetEnemy();
	
	if (!pEnemy)
		return COND_NONE;
	
	Vector flEnemyLKP = GetEnemyLKP();
	if ( !(pEnemy->GetFlags() & FL_ONGROUND) && pEnemy->GetWaterLevel() == 0 && flEnemyLKP.z > (GetAbsOrigin().z + WorldAlignMaxs().z)  )
	{
		//!!!BUGBUG - we should make this check movetype and make sure it isn't FLY? Players who jump a lot are unlikely to 
		// be grenaded.
		// don't throw grenades at anything that isn't on the ground!
		return COND_NONE;
	}
	
	Vector vecTarget;

	if (FBitSet( m_iWeapons, HGRUNT_HANDGRENADE))
	{
		// find feet
		if ( random->RandomInt( 0,1 ) )
		{
			// magically know where they are
			pEnemy->CollisionProp()->NormalizedToWorldSpace( Vector( 0.5f, 0.5f, 0.0f ), &vecTarget );
		}
		else
		{
			// toss it to where you last saw them
			vecTarget = flEnemyLKP;
		}
	}
	else
	{
		// find target
		// vecTarget = GetEnemy()->BodyTarget( GetAbsOrigin() );
		vecTarget = GetEnemy()->GetAbsOrigin() + (GetEnemy()->BodyTarget( GetAbsOrigin() ) - GetEnemy()->GetAbsOrigin());
		// estimate position
		if ( HasCondition( COND_SEE_ENEMY))
		{
			vecTarget = vecTarget + ((vecTarget - GetAbsOrigin()).Length() / sk_hgrunt_grenade_speed.GetFloat()) * GetEnemy()->GetAbsVelocity();
		}
	}

	// are any of my squad members near the intended grenade impact area?
	if ( m_pSquad )
	{
		if ( m_pSquad->SquadMemberInRange( vecTarget, 256 ) )
		{
			// crap, I might blow my own guy up. Don't throw a grenade and don't check again for a while.
			m_flNextGrenadeCheck = gpGlobals->curtime + 1; // one full second.
			return COND_NONE;
		}
	}
	
	if ( ( vecTarget - GetAbsOrigin() ).Length2D() <= 256 )
	{
		// crap, I don't want to blow myself up
		m_flNextGrenadeCheck = gpGlobals->curtime + 1; // one full second.
		return COND_NONE;
	}

		
	if (FBitSet( m_iWeapons, HGRUNT_HANDGRENADE))
	{
		Vector vGunPos = Weapon_ShootPosition();
		Vector vecToss = VecCheckToss( this, vGunPos, vecTarget, -1, 0.5, false );

		if ( vecToss != vec3_origin )
		{
			m_vecTossVelocity = vecToss;

			// don't check again for a while.
			m_flNextGrenadeCheck = gpGlobals->curtime + 0.3; // 1/3 second.

			return COND_CAN_RANGE_ATTACK2;
		}
		else
		{
			// don't check again for a while.
			m_flNextGrenadeCheck = gpGlobals->curtime + 1; // one full second.

			return COND_NONE;
		}
	}
	else
	{
		Vector vGunPos = Weapon_ShootPosition();
		Vector vecToss = VecCheckThrow( this, vGunPos, vecTarget, sk_hgrunt_grenade_speed.GetFloat(), 0.5 );

		if ( vecToss != vec3_origin )
		{
			m_vecTossVelocity = vecToss;

			// don't check again for a while.
			m_flNextGrenadeCheck = gpGlobals->curtime + 0.3; // 1/3 second.

			return COND_CAN_RANGE_ATTACK2;
		}
		else
		{
			// don't check again for a while.
			m_flNextGrenadeCheck = gpGlobals->curtime + 1; // one full second.

			return COND_NONE;
		}
	}
}

//=========================================================
// TraceAttack - make sure we're not taking it in the helmet
//=========================================================
void CHGrunt::TraceAttack( const CTakeDamageInfo &inputInfo, const Vector &vecDir, trace_t *ptr, CDmgAccumulator* pAccumulator)
{
	CTakeDamageInfo info = inputInfo;

	// check for helmet shot
	if (ptr->hitgroup == 11)
	{
		// make sure we're wearing one
		if ( GetBodygroup( 1 ) == HEAD_GRUNT && (info.GetDamageType() & (DMG_BULLET | DMG_SLASH | DMG_BLAST | DMG_CLUB)))
		{
			// absorb damage
			info.SetDamage( info.GetDamage() - 20 );
			if ( info.GetDamage() <= 0 )
				info.SetDamage( 0.01 );
		}
		// it's head shot anyways
		ptr->hitgroup = HITGROUP_HEAD;
	}
	BaseClass::TraceAttack( info, vecDir, ptr, pAccumulator);

}

//=========================================================
// TakeDamage - overridden for the grunt because the grunt
// needs to forget that he is in cover if he's hurt. (Obviously
// not in a safe place anymore).
//=========================================================
int CHGrunt::OnTakeDamage_Alive( const CTakeDamageInfo &inputInfo )
{
	Forget( bits_MEMORY_INCOVER );

	return BaseClass::OnTakeDamage_Alive ( inputInfo );
}

//=========================================================
// SetYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
float CHGrunt::MaxYawSpeed( void )
{
	float flYS;

	switch ( GetActivity() )
	{
	case ACT_IDLE:	
	case ACT_RUN:	
		flYS = 75;	
		break;
	case ACT_RANGE_ATTACK1:	
	case ACT_RANGE_ATTACK2:	
	case ACT_MELEE_ATTACK1:	
	case ACT_MELEE_ATTACK2:	
		flYS = 60;	
		break;
	case ACT_WALK:
	case ACT_TURN_LEFT:
	case ACT_TURN_RIGHT:	
		flYS = 90;
		break;
	case ACT_GLIDE:
	case ACT_FLY:
		flYS = 15;
		break;
	default:
		flYS = 45;
		break;
	}

	return flYS;
}

void CHGrunt::IdleSound( void )
{
	if ( m_fGruntQuestion || random->RandomInt( 0,1 ) )
	{
		if (!m_fGruntQuestion)
		{
			// ask question or make statement
			switch ( random->RandomInt( 0,2 ) )
			{
				case 0: // check in
					m_fGruntQuestion = 1;
					SetAndSpeakSentence(SENT_CHECK);
					break;
				case 1: // question
					m_fGruntQuestion = 2;
					SetAndSpeakSentence(SENT_QUEST);
					break;
				case 2: // statement
					SetAndSpeakSentence(SENT_IDLE);
					break;
			}
		}
		else
		{
			switch (m_fGruntQuestion)
			{
				case 1: // check in
					SetAndSpeakSentence(SENT_CLEAR);
					break;
				case 2: // question 
					SetAndSpeakSentence(SENT_ANSWER);
					break;
			}

			m_fGruntQuestion = 0;
		}
	}
}

//=========================================================
// CheckAmmo - overridden for the grunt because he actually
// uses ammo! (base class doesn't)
//=========================================================
void CHGrunt::CheckAmmo ( void )
{
	if ( m_cAmmoLoaded <= 0 )
    {
	 	 SetCondition( COND_NO_PRIMARY_AMMO );
    }
}

//-----------------------------------------------------------------------------
// Purpose: 
//
//
//-----------------------------------------------------------------------------
Class_T	CHGrunt::Classify(void)
{
	if (m_fIsFriendly)
		return CLASS_PLAYER_ALLY;

	return CLASS_HUMAN_MILITARY;
}

//=========================================================
//=========================================================
CBaseEntity *CHGrunt::Kick( void )
{
	trace_t tr;

	Vector forward;
	AngleVectors( GetAbsAngles(), &forward );
	Vector vecStart = GetAbsOrigin();
	vecStart.z += WorldAlignSize().z * 0.5;
	Vector vecEnd = vecStart + (forward * 70);

	UTIL_TraceHull( vecStart, vecEnd, Vector(-16,-16,-18), Vector(16,16,18), MASK_SHOT_HULL, this, COLLISION_GROUP_NONE, &tr );
	
	if ( tr.m_pEnt )
	{
		CBaseEntity *pEntity = tr.m_pEnt;
		return pEntity;
	}

	return NULL;
}

//=========================================================
// GetGunPosition	return the end of the barrel
//=========================================================
Vector CHGrunt::Weapon_ShootPosition( void )
{
	if ( m_fStanding )
    {
		return GetAbsOrigin() + Vector( 0, 0, 60 );
    }
	else
    {
		return GetAbsOrigin() + Vector( 0, 0, 48 );
    }
}

//=========================================================
// Shoot
//=========================================================
void CHGrunt::Shoot (int bulletnum, Vector cone)
{
	if ( GetEnemy() == NULL )
		return;
	
	Vector vecShootOrigin = Weapon_ShootPosition();
	Vector vecShootDir = GetShootEnemyDir( vecShootOrigin );

	Vector forward, right, up;
	AngleVectors( GetAbsAngles(), &forward, &right, &up );

	FireBullets(bulletnum, vecShootOrigin, vecShootDir, cone, 2048, m_iAmmoType);

	CEffectData data;
	data.m_nEntIndex = entindex();
	data.m_nAttachmentIndex = LookupAttachment("0");
	data.m_flScale = 1.0f;
	if (FBitSet(m_iWeapons, HGRUNT_SHOTGUN))
	{
		data.m_fFlags = MUZZLEFLASH_SHOTGUN;
	}
	else
	{
		data.m_fFlags = MUZZLEFLASH_SMG1;
	}
	DispatchEffect("MuzzleFlash", data);

	if (g_fr_npc_muzzlesmoke.GetBool())
	{
		DispatchParticleEffect("weapon_muzzle_smoke", PATTACH_POINT_FOLLOW, this, LookupAttachment("0"));
	}

	//redundant?
	DoMuzzleFlash();
	
	m_cAmmoLoaded--;// take away a bullet!

	QAngle angDir;
	VectorAngles(vecShootDir, angDir);

	float curPitch = GetPoseParameter("XR");
	float newPitch = curPitch + UTIL_AngleDiff(UTIL_ApproachAngle(angDir.x, curPitch, 60), curPitch);

	SetPoseParameter("XR", -newPitch);
}

//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//=========================================================
void CHGrunt::HandleAnimEvent( animevent_t *pEvent )
{
	Vector	vecShootDir;
	Vector	vecShootOrigin;

	switch( pEvent->event )
	{
		case HGRUNT_AE_RELOAD:
		{
			CPASAttenuationFilter filter( this );
			EmitSound( filter, entindex(), "HGrunt.Reload" );

			m_cAmmoLoaded = m_iClipSize;
			ClearCondition( COND_NO_PRIMARY_AMMO);
		}
		break;

		case HGRUNT_AE_GREN_TOSS:
		{
			Vector vecSpin;
			vecSpin.x = random->RandomFloat(-1000.0, 1000.0);
			vecSpin.y = random->RandomFloat(-1000.0, 1000.0);
			vecSpin.z = random->RandomFloat(-1000.0, 1000.0);

			Fraggrenade_Create(GetAbsOrigin() + Vector(0,0,60), vec3_angle, m_vecTossVelocity, vecSpin, this, 3.5, true );

			m_iLastGrenadeCondition =  COND_NONE;
			m_flNextGrenadeCheck = gpGlobals->curtime + 6;// wait six seconds before even looking again to see if a grenade can be thrown.

			Msg( "Tossing a grenade to flush you out!\n"	);
		}
		break;

		case HGRUNT_AE_GREN_LAUNCH:
		{
			CPASAttenuationFilter filter2( this );
			EmitSound( filter2, entindex(), "HGrunt.GrenadeLaunch" );
			
			Vector vecSrc = Weapon_ShootPosition();
		
			CGrenadeAR2 * m_pMyGrenade = (CGrenadeAR2*)Create( "grenade_ar2", vecSrc, vec3_angle, this );
			m_pMyGrenade->SetAbsVelocity( m_vecTossVelocity );
			m_pMyGrenade->SetLocalAngularVelocity(RandomAngle(-400, 400)); //tumble in air
			m_pMyGrenade->SetMoveType(MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_BOUNCE);
			m_pMyGrenade->SetThrower( this );
            m_pMyGrenade->SetGravity(0.5); // lower gravity since grenade is aerodynamic and engine doesn't know it.
			m_pMyGrenade->SetDamage(sk_npc_dmg_smg1_grenade.GetFloat() );

			if (g_pGameRules->IsSkillLevel(SKILL_HARD))
			{
				m_flNextGrenadeCheck = gpGlobals->curtime + RandomFloat(2,3);
			}
			else if (g_pGameRules->IsSkillLevel(SKILL_VERYHARD))
			{
				m_flNextGrenadeCheck = gpGlobals->curtime + RandomFloat(1.5,2);
			}
			else if (g_pGameRules->IsSkillLevel(SKILL_NIGHTMARE))
			{
				m_flNextGrenadeCheck = gpGlobals->curtime + RandomFloat(1,1.5);
			}
			else
			{
				m_flNextGrenadeCheck = gpGlobals->curtime + 6;// wait six seconds before even looking again to see if a grenade can be thrown.
			}

			m_iLastGrenadeCondition =  COND_NONE;

			Msg( "Using grenade launcher to flush you out!\n"	);
		}
		break;

		case HGRUNT_AE_GREN_DROP:
		{
			Vector vecSpin;
			vecSpin.x = random->RandomFloat(-1000.0, 1000.0);
			vecSpin.y = random->RandomFloat(-1000.0, 1000.0);
			vecSpin.z = random->RandomFloat(-1000.0, 1000.0);

            Fraggrenade_Create(Weapon_ShootPosition(), vec3_angle, m_vecTossVelocity, vecSpin, this, 3.5, true );

			m_iLastGrenadeCondition =  COND_NONE;
			Msg( "Dropping a grenade!\n"	);
		}
		break;

		case HGRUNT_AE_BURST1:
		{
			if ( FBitSet( m_iWeapons, HGRUNT_9MMAR ) )
			{
				Shoot(1, VECTOR_CONE_10DEGREES);

				CPASAttenuationFilter filter3( this );
				// the first round of the three round burst plays the sound and puts a sound in the world sound list.
				EmitSound( filter3, entindex(), "HGrunt.9MM" );
			}
			else
			{
				Shoot(sk_hgrunt_shotgun_pellets.GetFloat(), VECTOR_CONE_15DEGREES);

				CPASAttenuationFilter filter4( this );
				EmitSound( filter4, entindex(), "HGrunt.Shotgun" );
			}
		
			CSoundEnt::InsertSound ( SOUND_COMBAT, GetAbsOrigin(), 384, 0.3 );
		}
		break;

		case HGRUNT_AE_BURST2:
		case HGRUNT_AE_BURST3:
			Shoot(1, VECTOR_CONE_10DEGREES);
			break;

		case HGRUNT_AE_KICK:
		{
			CBaseEntity *pHurt = Kick();

			if ( pHurt )
			{
				// SOUND HERE!
				Vector forward, up;
				AngleVectors( GetAbsAngles(), &forward, NULL, &up );

				if ( pHurt->GetFlags() & ( FL_NPC | FL_CLIENT ) )
					 pHurt->ViewPunch( QAngle( 15, 0, 0) );

				// Don't give velocity or damage to the world
				if( pHurt->entindex() > 0 )
				{
					pHurt->ApplyAbsVelocityImpulse( forward * 100 + up * 50 );

					CTakeDamageInfo info( this, this, sk_hgrunt_kick.GetFloat(), DMG_CLUB );
					CalculateMeleeDamageForce( &info, forward, pHurt->GetAbsOrigin() );
					pHurt->TakeDamage( info );
				}			
			}
		}
		break;

		case HGRUNT_AE_CAUGHT_ENEMY:
		{
			SetAndSpeakSentence(SENT_ALERT);
		}

		default:
			BaseClass::HandleAnimEvent( pEvent );
			break;
	}
}

//=========================================================
// Spawn
//=========================================================
void CHGrunt::Spawn()
{
	if (FClassnameIs(this, "npc_hgrunt_friendly"))
	{
		AddSpawnFlags(SF_GRUNT_FRIENDLY);
	}
	else if (FClassnameIs(this, "npc_hgrunt_robot"))
	{
		AddSpawnFlags(SF_GRUNT_ROBOT);
	}
	else if (FClassnameIs(this, "npc_hgrunt_friendly_robot"))
	{
		AddSpawnFlags(SF_GRUNT_FRIENDLY);
		AddSpawnFlags(SF_GRUNT_ROBOT);
	}

	Precache( );

	if (HasSpawnFlags(SF_GRUNT_ROBOT))
	{
		SetModel("models/g_hgrunt.mdl");
	}
	else
	{
		SetModel("models/hgrunt.mdl");
	}

	BaseClass::Spawn();

	SetHullType(HULL_HUMAN);
	SetHullSizeNormal();

	SetSolid( SOLID_BBOX );
	AddSolidFlags( FSOLID_NOT_STANDABLE );
	SetMoveType( MOVETYPE_STEP );
	if (HasSpawnFlags(SF_GRUNT_ROBOT))
	{
		m_bloodColor = BLOOD_COLOR_MECH;
	}
	else
	{
		m_bloodColor = BLOOD_COLOR_RED;
	}
	ClearEffects();
	if (HasSpawnFlags(SF_GRUNT_ROBOT))
	{
		m_iHealth = sk_hgrunt_robot_health.GetFloat();
	}
	else
	{
		m_iHealth = sk_hgrunt_health.GetFloat();
	}
	
	if (HasSpawnFlags(SF_GRUNT_ROBOT))
	{
		m_flFieldOfView = VIEW_FIELD_WIDE;// indicates the width of this monster's forward view cone ( as a dotproduct result )
	}
	else
	{
		m_flFieldOfView = 0.2;// indicates the width of this monster's forward view cone ( as a dotproduct result )
	}
	m_NPCState			= NPC_STATE_NONE;
	m_flNextGrenadeCheck = gpGlobals->curtime + 1;
	m_flNextPainTime	= gpGlobals->curtime;
	m_iSentence			= -1;

	CapabilitiesClear();
	CapabilitiesAdd ( bits_CAP_SQUAD | bits_CAP_TURN_HEAD | bits_CAP_DOORS_GROUP | bits_CAP_MOVE_GROUND );

	CapabilitiesAdd(bits_CAP_INNATE_RANGE_ATTACK1 );

	// Innate range attack for grenade
	CapabilitiesAdd(bits_CAP_INNATE_RANGE_ATTACK2 );
	// Innate range attack for kicking
	CapabilitiesAdd(bits_CAP_INNATE_MELEE_ATTACK1 );

	NPCInit();

	if (HasSpawnFlags(SF_GRUNT_FRIENDLY))
	{
		BecomeFriendly();
	}
			
	m_fFirstEncounter	= true;// this is true when the grunt spawns, because he hasn't encountered an enemy yet.

	m_HackedGunPos = Vector ( 0, 0, 55 );

	if ( m_iWeapons == 0)
	{
		// initialize to original values
		m_iWeapons = HGRUNT_9MMAR | HGRUNT_HANDGRENADE;
		// pev->weapons = HGRUNT_SHOTGUN;
		// pev->weapons = HGRUNT_9MMAR | HGRUNT_GRENADELAUNCHER;
	}

	if (FBitSet( m_iWeapons, HGRUNT_SHOTGUN ))
	{
		SetBodygroup( GUN_GROUP, GUN_SHOTGUN );
		m_iClipSize		= sk_hgrunt_shotgun_magsize.GetInt();
	}
	else
	{
		m_iClipSize		= sk_hgrunt_smg_magsize.GetInt();
	}
	m_cAmmoLoaded		= m_iClipSize;

	if ( random->RandomInt( 0, 99 ) < 80)
		m_nSkin = 0;	// light skin
	else
		m_nSkin = 1;	// dark skin

	if (FBitSet( m_iWeapons, HGRUNT_SHOTGUN ))
	{
		SetBodygroup( HEAD_GROUP, HEAD_SHOTGUN);
	}
	else if (FBitSet( m_iWeapons, HGRUNT_GRENADELAUNCHER ))
	{
		SetBodygroup( HEAD_GROUP, HEAD_M203 );
		m_nSkin = 1; // alway dark skin
	}

	m_flTalkWaitTime = 0;

	//HACK
	m_iSquadIndex = 0;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CHGrunt::NPCThink()
{
	BaseClass::NPCThink();

	for (int i = 1; i <= gpGlobals->maxClients; i++)
	{
		CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);

		if (pPlayer != NULL)
		{
			if (hgrunt_hurttaunt.GetBool() && !pPlayer->IsDead())
			{
				//wait before we pick a taunt.
				if (FOkToSpeak())
				{
					if (random->RandomInt(0, hgrunt_hurttaunt_frequency.GetInt()) == 0 && pPlayer->GetHealth() < hgrunt_hurttaunt_healthvalue.GetInt())
					{
						TauntEnemy(true);
					}
				}
			}
		}
	}
}

void CHGrunt::BecomeFriendly()
{
	m_fIsFriendly = true;
	AssignKilllogTeams(TEAM_BLUE);
	CapabilitiesAdd(bits_CAP_NO_HIT_PLAYER | bits_CAP_FRIENDLY_DMG_IMMUNE);
}

void CHGrunt::FriendlyEscortCheck()
{
	if (!m_fIsFriendly)
		return;

	if (m_FollowBehavior.GetFollowTarget())
		return;

	//escort any player that's nearby
	CBasePlayer* pPlayer = UTIL_GetNearestPlayer(GetAbsOrigin());
	if (pPlayer)
	{
		AI_FollowParams_t params;
		params.formation = AIF_SIDEKICK;
		m_FollowBehavior.SetParameters(params);
		m_FollowBehavior.SetFollowTarget(pPlayer);
	}
}

void CHGrunt::LoadInitAttributes()
{
	if (m_pAttributes != NULL)
	{
		if (m_pAttributes->GetBool("is_ally"))
		{
			AddSpawnFlags(SF_GRUNT_FRIENDLY);
		}
	}

	BaseClass::LoadInitAttributes();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CHGrunt::CreateBehaviors()
{
	AddBehavior(&m_FollowBehavior);

	return BaseClass::CreateBehaviors();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHGrunt::GatherConditions(void)
{
	FriendlyEscortCheck();

	BaseClass::GatherConditions();
}

//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CHGrunt::Precache()
{
	m_iAmmoType = GetAmmoDef()->Index("MP5Ammo");

	if (HasSpawnFlags(SF_GRUNT_ROBOT))
	{
		PrecacheModel("models/g_hgrunt.mdl");
	}
	else
	{
		PrecacheModel("models/hgrunt.mdl");
	}

	// get voice pitch
	if ( random->RandomInt(0,1))
		m_voicePitch = 109 + random->RandomInt(0,7);
	else
		m_voicePitch = 100;

	PrecacheScriptSound( "HGrunt.Reload" );
	PrecacheScriptSound( "HGrunt.GrenadeLaunch" );
	PrecacheScriptSound( "HGrunt.9MM" );
	PrecacheScriptSound( "HGrunt.Shotgun" );
	if (HasSpawnFlags(SF_GRUNT_ROBOT))
	{
		PrecacheScriptSound("HGrunt.Pain_Robot");
		PrecacheScriptSound("HGrunt.Die_Robot");
	}
	else
	{
		PrecacheScriptSound("HGrunt.Pain");
		PrecacheScriptSound("HGrunt.Die");
	}

	BaseClass::Precache();

	UTIL_PrecacheOther( "npc_grenade_frag" );
	UTIL_PrecacheOther( "grenade_ar2" );
	UTIL_PrecacheOther("weapon_shotgun");
	UTIL_PrecacheOther("weapon_mp5");
	UTIL_PrecacheOther("item_ammo_smg1_grenade");
	UTIL_PrecacheOther("weapon_frag");
	UTIL_PrecacheOther("item_healthvial");
}	

//=========================================================
// start task
//=========================================================
void CHGrunt::StartTask ( const Task_t *pTask )
{
	switch ( pTask->iTask )
	{
	case TASK_GRUNT_SELECT_IDEAL_ATTACK_SEQUENCE:
		if (m_fStanding)
		{
			SetIdealActivity(ACT_IDLE_ANGRY);
		}
		else
		{
			SetIdealActivity(ACT_CROUCH);
		}

		TaskComplete();
		break;

	case TASK_GRUNT_SUPPRESS_HANDSIGNAL:
		SetIdealActivity(ACT_SIGNAL2);
		TaskComplete();
		break;
	
	case TASK_WALK_PATH:
	case TASK_RUN_PATH:
		// grunt no longer assumes he is covered if he moves
		Forget( bits_MEMORY_INCOVER );
		BaseClass ::StartTask( pTask );
		break;

	case TASK_RELOAD:
		SetIdealActivity( ACT_RELOAD );
		break;

	case TASK_GRUNT_FACE_TOSS_DIR:
		break;

	case TASK_FACE_IDEAL:
	case TASK_FACE_ENEMY:
		BaseClass::StartTask( pTask );
		if (GetMoveType() == MOVETYPE_FLYGRAVITY)
		{
			SetIdealActivity( ACT_GLIDE );
		}
		break;

	default: 
		BaseClass::StartTask( pTask );
		break;
	}
}

//=========================================================
// RunTask
//=========================================================
void CHGrunt::RunTask( const Task_t *pTask )
{
	switch ( pTask->iTask )
	{
	case TASK_GRUNT_FACE_TOSS_DIR:
		{
			// project a point along the toss vector and turn to face that point.
			GetMotor()->SetIdealYawToTargetAndUpdate( GetAbsOrigin() + m_vecTossVelocity * 64, AI_KEEP_YAW_SPEED );

			if ( FacingIdeal() )
			{
				TaskComplete();
			}
			break;
		}
	case TASK_GRUNT_SUPPRESS_HANDSIGNAL:
		{
			CBaseEntity* pTarget;

			if (GetTarget() && pTask->iTask == TASK_PLAY_SEQUENCE_FACE_TARGET)
				pTarget = GetTarget();
			else
				pTarget = GetEnemy();
			if (pTarget)
			{
				GetMotor()->SetIdealYawAndUpdate(pTarget->GetAbsOrigin() - GetLocalOrigin(), AI_KEEP_YAW_SPEED);
			}

			if (IsActivityFinished())
			{
				TaskComplete();
			}
			break;
		}
	default:
		{
			BaseClass::RunTask( pTask );
			break;
		}
	}
}

//=========================================================
// PainSound
//=========================================================
void CHGrunt::PainSound( const CTakeDamageInfo &info )
{
	if ( gpGlobals->curtime > m_flNextPainTime )
	{
		CPASAttenuationFilter filter( this );
		if (HasSpawnFlags(SF_GRUNT_ROBOT))
		{
			EmitSound(filter, entindex(), "HGrunt.Pain_Robot");
		}
		else
		{
			EmitSound(filter, entindex(), "HGrunt.Pain");
		}

		m_flNextPainTime = gpGlobals->curtime + 1;
	}
}

//=========================================================
// DeathSound 
//=========================================================
void CHGrunt::DeathSound( const CTakeDamageInfo &info )
{
	CPASAttenuationFilter filter( this, ATTN_IDLE );
	if (HasSpawnFlags(SF_GRUNT_ROBOT))
	{
		EmitSound(filter, entindex(), "HGrunt.Die_Robot");
	}
	else
	{
		EmitSound(filter, entindex(), "HGrunt.Die");
	}
}

//=========================================================
// SetActivity 
//=========================================================
Activity CHGrunt::NPC_TranslateActivity( Activity NewActivity )
{
	switch ( NewActivity)
	{
	case ACT_RANGE_ATTACK1:
		// grunt is either shooting standing or shooting crouched
		if (FBitSet( m_iWeapons, HGRUNT_9MMAR))
		{
			if ( m_fStanding )
			{
				// get aimable sequence
				return (Activity)ACT_GRUNT_MP5_STANDING;
			}
			else
			{
				// get crouching shoot
				return (Activity)ACT_GRUNT_MP5_CROUCHING;
			}
		}
		else
		{
			if ( m_fStanding )
			{
				// get aimable sequence
				return (Activity)ACT_GRUNT_SHOTGUN_STANDING;
			}
			else
			{
				// get crouching shoot
				return (Activity)ACT_GRUNT_SHOTGUN_CROUCHING;
			}
		}
		break;
	case ACT_RANGE_ATTACK2:
		// grunt is going to a secondary long range attack. This may be a thrown 
		// grenade or fired grenade, we must determine which and pick proper sequence
 		if ( m_iWeapons & HGRUNT_HANDGRENADE )
		{
			// get toss anim
			return (Activity)ACT_GRUNT_TOSS_GRENADE;
		}
		else
		{
			// get launch anim
			return (Activity)ACT_GRUNT_LAUNCH_GRENADE;
		}
		break;
	case ACT_RUN:
		if ( m_iHealth <= sk_hgrunt_limp_health.GetInt() && !HasSpawnFlags(SF_GRUNT_ROBOT))
		{
			// limp!
			return ACT_RUN_HURT;
		}
		else
		{
			return NewActivity;
		}
		break;
	case ACT_WALK:
		if ( m_iHealth <= sk_hgrunt_limp_health.GetInt() && !HasSpawnFlags(SF_GRUNT_ROBOT))
		{
			// limp!
			return ACT_WALK_HURT;
		}
		else
		{
			return NewActivity;
		}
		break;
	case ACT_IDLE:
		if ( m_NPCState == NPC_STATE_COMBAT )
		{
			NewActivity = ACT_IDLE_ANGRY;
		}
		
		break;
	}

	return BaseClass::NPC_TranslateActivity( NewActivity );
}

void CHGrunt::TauntEnemy(bool bHurtTaunt)
{
	if (bHurtTaunt)
	{
		if (random->RandomInt(1, sk_hgrunt_eastereggtaunt_prob.GetInt()) == sk_hgrunt_eastereggtaunt_prob.GetInt())
		{
			SetAndSpeakSentence(SENT_TAUNT_EASTEREGG);
		}
		else
		{
			SetAndSpeakSentence(SENT_TAUNT_PLAYERHURT);
		}
	}
	else
	{
		if (random->RandomInt(0, 1))
		{
			if (random->RandomInt(1, sk_hgrunt_eastereggtaunt_prob.GetInt()) == sk_hgrunt_eastereggtaunt_prob.GetInt())
			{
				SetAndSpeakSentence(SENT_TAUNT_EASTEREGG);
			}
			else
			{
				SetAndSpeakSentence(SENT_TAUNT);
			}
		}
	}
}

int CHGrunt::SelectSchedule( void )
{
	if (FOkToSpeak())
	{
		m_iSentence = -1;
	}

	// flying? If PRONE, barnacle has me. IF not, it's assumed I am rapelling. 
	if ( GetMoveType() == MOVETYPE_FLYGRAVITY && m_NPCState != NPC_STATE_PRONE )
	{
		if (GetFlags() & FL_ONGROUND)
		{
			// just landed
			SetMoveType( MOVETYPE_STEP );
			SetGravity( 1.0 );
			return SCHED_GRUNT_REPEL_LAND;
		}
		else
		{
			// repel down a rope, 
			if ( m_NPCState == NPC_STATE_COMBAT )
				return SCHED_GRUNT_REPEL_ATTACK;
			else
				return SCHED_GRUNT_REPEL;
		}
	}

	if (!BehaviorSelectSchedule())
	{
		//If we are in idle, try to find the enemy by walking.
		if (GetMoveType() != MOVETYPE_FLYGRAVITY && !m_fIsFriendly && (m_NPCState == NPC_STATE_IDLE || m_NPCState == NPC_STATE_ALERT))
		{
			return SCHED_PATROL_WALK_LOOP;
		}
	}

	// grunts place HIGH priority on running away from danger sounds.
	if ( HasCondition ( COND_HEAR_DANGER ) )
	{
		// dangerous sound nearby!
				
		//!!!KELLY - currently, this is the grunt's signal that a grenade has landed nearby,
		// and the grunt should find cover from the blast
		// good place for "SHIT!" or some other colorful verbal indicator of dismay.
		// It's not safe to play a verbal order here "Scatter", etc cause 
		// this may only affect a single individual in a squad. 
				
		SetAndSpeakSentence(SENT_GREN);

		return SCHED_TAKE_COVER_FROM_BEST_SOUND;
	}

	switch	( m_NPCState )
	{
	case NPC_STATE_COMBAT:
		{
// dead enemy
			if ( HasCondition( COND_ENEMY_DEAD ) )
			{
				// call base class, all code to handle dead enemies is centralized there.
				return BaseClass::SelectSchedule();
			}

// new enemy
			if ( HasCondition( COND_NEW_ENEMY) )
			{
				if ( m_pSquad )
				{
					if (m_pSquad->GetLeader())
					{
						CHGrunt* pSquadLeader = (CHGrunt*)m_pSquad->GetLeader()->MyNPCPointer();
						if (pSquadLeader)
						{
							pSquadLeader->GetEnemies()->MarkAsNotEluded(GetEnemy());
						}
					}

					if ( !m_pSquad->IsLeader( this ) )
					{
						return SCHED_TAKE_COVER_FROM_ENEMY;
					}
					else 
					{
						//!!!KELLY - the leader of a squad of grunts has just seen the player or a 
						// monster and has made it the squad's enemy. You
						// can check pev->flags for FL_CLIENT to determine whether this is the player
						// or a monster. He's going to immediately start
						// firing, though. If you'd like, we can make an alternate "first sight" 
						// schedule where the leader plays a handsign anim
						// that gives us enough time to hear a short sentence or spoken command
						// before he starts pluggin away.
						if ((GetEnemy() != NULL) && (GetEnemy()->IsPlayer()) || (GetEnemy()->Classify() == CLASS_PLAYER_NPC))
						{
							SetAndSpeakSentence(SENT_ALERT);
						}
						else if ((GetEnemy() != NULL) && (IsEntityAlien(GetEnemy())))
						{
							SetAndSpeakSentence(SENT_MONST);
						}
						
						if ( HasCondition ( COND_CAN_RANGE_ATTACK1 ) )
						{
							return SCHED_GRUNT_SUPPRESS;
						}
						else
						{
							return SCHED_GRUNT_ESTABLISH_LINE_OF_FIRE;
						}
					}
				}
			}
// no ammo
			else if ( HasCondition ( COND_NO_PRIMARY_AMMO ) )
			{
				//!!!KELLY - this individual just realized he's out of bullet ammo. 
				// He's going to try to find cover to run to and reload, but rarely, if 
				// none is available, he'll drop and reload in the open here. 
				return SCHED_GRUNT_COVER_AND_RELOAD;
			}
			
// damaged just a little
			else if ( HasCondition( COND_LIGHT_DAMAGE ) )
			{
				// if hurt:
				// 90% chance of taking cover
				// 10% chance of flinch.
				int iPercent = random->RandomInt(0,99);

				int percentage = (HasSpawnFlags(SF_GRUNT_ROBOT) ? 50 : 90);

				if ( iPercent <= percentage && GetEnemy() != NULL )
				{
					// only try to take cover if we actually have an enemy!

					//!!!KELLY - this grunt was hit and is going to run to cover.
					if (random->RandomInt(0, 1))
					{
						SetAndSpeakSentence(SENT_COVER);
					}
					return SCHED_TAKE_COVER_FROM_ENEMY;
				}
				else
				{
					return SCHED_SMALL_FLINCH;
				}
			}
// can kick
			else if ( HasCondition( COND_CAN_MELEE_ATTACK1 ) )
			{
				return SCHED_MELEE_ATTACK1;
			}
// can grenade launch

			else if ( FBitSet( m_iWeapons, HGRUNT_GRENADELAUNCHER) && HasCondition ( COND_CAN_RANGE_ATTACK2 ) && OccupyStrategySlotRange( SQUAD_SLOT_GRENADE1, SQUAD_SLOT_GRENADE2 ) )
			{
				// shoot a grenade if you can
				return SCHED_RANGE_ATTACK2;
			}
// can shoot
			else if ( HasCondition ( COND_CAN_RANGE_ATTACK1 ) )
			{
				if ( m_pSquad )
				{
					if ( m_pSquad->GetLeader() != NULL )
					{
					
						CAI_BaseNPC *pSquadLeader = m_pSquad->GetLeader()->MyNPCPointer();
						
						// if the enemy has eluded the squad and a squad member has just located the enemy
						// and the enemy does not see the squad member, issue a call to the squad to waste a 
						// little time and give the player a chance to turn.
						if ( pSquadLeader && pSquadLeader->EnemyHasEludedMe() && !HasCondition ( COND_ENEMY_FACING_ME ) )
						{
							return SCHED_GRUNT_FOUND_ENEMY;
						}
					}
				}

				if ( OccupyStrategySlotRange ( SQUAD_SLOT_CHARGE1, SQUAD_SLOT_CHARGE2 ) )
				{
					// try to take an available CHARGE slot
					return SCHED_RANGE_ATTACK1;
				}
				else if ( HasCondition ( COND_CAN_RANGE_ATTACK2 ) && OccupyStrategySlotRange( SQUAD_SLOT_GRENADE1, SQUAD_SLOT_GRENADE2 ) )
				{
					// throw a grenade if can and no CHARGE slots are available
					return SCHED_RANGE_ATTACK2;
				}
				else
				{
					// hide!
					return SCHED_TAKE_COVER_FROM_ENEMY;
				}
			}
// can't see enemy
			else if ( HasCondition( COND_ENEMY_OCCLUDED ) )
			{
				if ( HasCondition( COND_CAN_RANGE_ATTACK2 ) && OccupyStrategySlotRange( SQUAD_SLOT_GRENADE1, SQUAD_SLOT_GRENADE2 ) )
				{
					//!!!KELLY - this grunt is about to throw or fire a grenade at the player. Great place for "fire in the hole"  "frag out" etc
					SetAndSpeakSentence(SENT_THROW);
					return SCHED_RANGE_ATTACK2;
				}
				else if ( OccupyStrategySlotRange ( SQUAD_SLOT_CHARGE1, SQUAD_SLOT_CHARGE2 ) )
				{
					//!!!KELLY - grunt cannot see the enemy and has just decided to 
					// charge the enemy's position. 
					if (random->RandomInt(0, 1))
					{
						SetAndSpeakSentence(SENT_CHARGE);
					}

					return SCHED_GRUNT_ESTABLISH_LINE_OF_FIRE;
				}
				else
				{
					//!!!KELLY - grunt is going to stay put for a couple seconds to see if
					// the enemy wanders back out into the open, or approaches the
					// grunt's covered position. Good place for a taunt, I guess?
					TauntEnemy();
					return SCHED_STANDOFF;
				}
			}
			
			if ( HasCondition( COND_SEE_ENEMY ) && !HasCondition ( COND_CAN_RANGE_ATTACK1 ) )
			{
				return SCHED_GRUNT_ESTABLISH_LINE_OF_FIRE;
			}
		}
		case NPC_STATE_ALERT:
			if ( HasCondition( COND_ENEMY_DEAD ))
			{
				if (SelectWeightedSequence(ACT_VICTORY_DANCE) != ACTIVITY_NOT_AVAILABLE)
				{
					// Scan around for new enemies
					return SCHED_VICTORY_DANCE;
				}
			}
			break;
	}

	return BaseClass::SelectSchedule();
}

int CHGrunt::TranslateSchedule( int scheduleType )
{
	if ( scheduleType == SCHED_CHASE_ENEMY_FAILED )
	{
		 return SCHED_ESTABLISH_LINE_OF_FIRE;
	}

	switch	( scheduleType )
	{
		case SCHED_TAKE_COVER_FROM_ENEMY:
			{
				if ( m_pSquad )
				{
					if ( g_pGameRules->GetSkillLevel() >= SKILL_HARD && HasCondition( COND_CAN_RANGE_ATTACK2 ) && OccupyStrategySlotRange( SQUAD_SLOT_GRENADE1, SQUAD_SLOT_GRENADE2 ) )
					{
						SetAndSpeakSentence(SENT_THROW);
						return SCHED_GRUNT_TOSS_GRENADE_COVER;
					}
					else
					{
						SetAndSpeakSentence(SENT_COVER);
			  			return SCHED_GRUNT_TAKECOVER;
					}
				}
				else
				{
					if (m_fFirstEncounter)
					{
						SetAndSpeakSentence(SENT_ALERT);
					}
					else
					{
						SetAndSpeakSentence(SENT_COVER);
					}

					if ( random->RandomInt(0,1) )
					{
						return SCHED_GRUNT_TAKECOVER;
					}
					else
					{
						return SCHED_GRUNT_DROP_GRENADE_COVER;
					}
				}
			}
		case SCHED_GRUNT_TAKECOVER_FAILED:
			{
				if ( HasCondition( COND_CAN_RANGE_ATTACK1 ) && OccupyStrategySlotRange( SQUAD_SLOT_ATTACK1, SQUAD_SLOT_ATTACK2 ) )
				{
					return SCHED_RANGE_ATTACK1;
				}

				return SCHED_FAIL;
			}
			break;
	
		case SCHED_RANGE_ATTACK1:
			{
				// randomly stand or crouch
				if ( random->RandomInt( 0,9 ) == 0)
				{
					m_fStanding = random->RandomInt( 0, 1 ) != 0;
				}

				if (m_pSquad)
				{
					if (m_pSquad->IsLeader(this))
					{
						//taunt if we're the leader.
						TauntEnemy();
					}
				}

				return SCHED_GRUNT_RANGE_ATTACK1;
			}

		case SCHED_RANGE_ATTACK2:
			{
				return SCHED_GRUNT_RANGE_ATTACK2;
			}
		case SCHED_VICTORY_DANCE:
			{
				if ( m_pSquad )
				{
					if ( !m_pSquad->IsLeader( this ) )
					{
						return SCHED_GRUNT_ELOF_FAIL;
					}
				}

				return SCHED_VICTORY_DANCE;
			}
		case SCHED_GRUNT_SUPPRESS:
			{
				if ( GetEnemy()->IsPlayer() && m_fFirstEncounter )
				{
					m_fFirstEncounter = false; // after first encounter, leader won't issue handsigns anymore when he has a new enemy
				}

				return SCHED_GRUNT_SUPPRESS;
			}
		case SCHED_FAIL:
			{
				return SCHED_GRUNT_ELOF_FAIL;
			}
		case SCHED_GRUNT_REPEL:
			{
				Vector vecVel = GetAbsVelocity();
				if ( vecVel.z > -128 )
				{
					vecVel.z -= 32;
					SetAbsVelocity( vecVel );
				}

				return SCHED_GRUNT_REPEL;
			}
		case SCHED_GRUNT_REPEL_ATTACK:
			{
				Vector vecVel = GetAbsVelocity();
				if ( vecVel.z > -128 )
				{
					vecVel.z -= 32;
					SetAbsVelocity( vecVel );
				}

				return SCHED_GRUNT_REPEL_ATTACK;
			}
		default:
			{
				return BaseClass::TranslateSchedule( scheduleType );
			}
	}
}

//=========================================================
//
// SquadRecruit(), get some monsters of my classification and
// link them as a group.  returns the group size
//
//=========================================================
int CHGrunt::SquadRecruit( int searchRadius, int maxMembers )
{
	int squadCount;
	int iMyClass = Classify();// cache this monster's class

	if (IsInSquad() && GetSquad()->NumMembers(false) > 1)
		return 0;

	if ( maxMembers < 2 )
		 return 0;

	// I am my own leader
	squadCount = 1;
	
	CBaseEntity *pEntity = NULL;

	if ( m_SquadName != NULL_STRING )
	{
		// I have a netname, so unconditionally recruit everyone else with that name.
		pEntity = gEntList.FindEntityByClassname( pEntity, GetClassname() );

		while ( pEntity )
		{
			CHGrunt *pRecruit = (CHGrunt*)pEntity->MyNPCPointer();

			if ( pRecruit )
			{
				if (pRecruit != this && pRecruit->IsAlive() && !pRecruit->m_hCine)
				{
					// minimum protection here against user error.in worldcraft. 
					if ( pRecruit->m_SquadName != NULL_STRING && FStrEq( STRING( m_SquadName ), STRING( pRecruit->m_SquadName ) ) )
					{
						pRecruit->InitSquad();
						squadCount++;
					}
				}
			}
	
			pEntity = gEntList.FindEntityByClassname( pEntity, GetClassname());
		}

		return squadCount;
	}
	else
	{
		char szSquadName[64];
		Q_snprintf( szSquadName, sizeof( szSquadName ), "squad%d\n", m_iSquadIndex );

		m_SquadName = MAKE_STRING( szSquadName );

		while ( ( pEntity = gEntList.FindEntityInSphere( pEntity, GetAbsOrigin(), searchRadius ) ) != NULL )
		{
			if ( !FClassnameIs ( pEntity, GetClassname()) )
				  continue;

			CHGrunt *pRecruit = (CHGrunt*)pEntity->MyNPCPointer();

			if ( pRecruit && pRecruit != this && pRecruit->IsAlive() && !pRecruit->m_hCine )
			{
				// Can we recruit this guy?
				if (pRecruit->Classify() == iMyClass && FClassnameIs(this, pRecruit->GetClassname()))
				{
					trace_t tr;
					UTIL_TraceLine( GetAbsOrigin() + GetViewOffset(), pRecruit->GetAbsOrigin() + GetViewOffset(), MASK_NPCSOLID_BRUSHONLY, pRecruit, COLLISION_GROUP_NONE, &tr );// try to hit recruit with a traceline.

					if ( tr.fraction == 1.0 )
					{
						//We're ready to recruit people, so start a squad if I don't have one.
						if ( !m_pSquad )
						{
							InitSquad();
						}

						pRecruit->m_SquadName = m_SquadName;

						pRecruit->CapabilitiesAdd ( bits_CAP_SQUAD );
						pRecruit->InitSquad();

						squadCount++;
					}
				}
			}
		}

		if ( squadCount > 1 )
		{
			 m_iSquadIndex++;
		}
	}

	return squadCount;
}

void CHGrunt::StartNPC ( void )
{
	if ( !m_pSquad )
	{
		if ( m_SquadName != NULL_STRING )
		{
			// if I have a groupname, I can only recruit if I'm flagged as leader
			if ( !(GetSpawnFlags() & SF_GRUNT_LEADER) )
			{
				BaseClass::StartNPC();
				return;
			}
		}
		
		int iSquadSize = SquadRecruit(1024, 4);

		if (iSquadSize)
		{
			Msg("Squad of %d %s formed\n", iSquadSize, GetClassname());
		}
	}

	BaseClass::StartNPC();

	if ( m_pSquad && m_pSquad->IsLeader( this ) )
	{
		SetBodygroup( 1, 1 ); // UNDONE: truly ugly hack
		m_nSkin = 0;
	}
}

//------------------------------------------------------------------------------
//
// Schedules
//
//------------------------------------------------------------------------------
AI_BEGIN_CUSTOM_NPC( npc_hgrunt, CHGrunt )

	DECLARE_ACTIVITY( ACT_GRUNT_LAUNCH_GRENADE )
	DECLARE_ACTIVITY( ACT_GRUNT_TOSS_GRENADE )
	DECLARE_ACTIVITY( ACT_GRUNT_MP5_STANDING );
	DECLARE_ACTIVITY( ACT_GRUNT_MP5_CROUCHING );
	DECLARE_ACTIVITY( ACT_GRUNT_SHOTGUN_STANDING );
	DECLARE_ACTIVITY( ACT_GRUNT_SHOTGUN_CROUCHING );

	DECLARE_CONDITION( COND_GRUNT_NOFIRE )
	
	DECLARE_TASK( TASK_GRUNT_FACE_TOSS_DIR )
	DECLARE_TASK(TASK_GRUNT_SELECT_IDEAL_ATTACK_SEQUENCE)
	DECLARE_TASK(TASK_GRUNT_SUPPRESS_HANDSIGNAL)
		
	DECLARE_SQUADSLOT( SQUAD_SLOT_GRENADE1 )
	DECLARE_SQUADSLOT( SQUAD_SLOT_GRENADE2 )
	DECLARE_SQUADSLOT( SQUAD_SLOT_CHARGE1 )
	DECLARE_SQUADSLOT( SQUAD_SLOT_CHARGE2 )

	//=========================================================
	// > SCHED_GRUNT_ELOF_FAIL
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_GRUNT_ELOF_FAIL,
	
		"	Tasks"
		"		TASK_STOP_MOVING		0"
		"		TASK_SET_ACTIVITY		ACTIVITY:ACT_IDLE"
		"		TASK_WAIT				0"
		"		TASK_WAIT_PVS			0"
		"	"
		"	Interrupts"
		"		COND_CAN_RANGE_ATTACK1"
		"		COND_CAN_MELEE_ATTACK1"
	)
	
	//=========================================================
	// > SCHED_GRUNT_ESTABLISH_LINE_OF_FIRE
	// Establish line of fire - move to a position that allows
	// the grunt to attack.
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_GRUNT_ESTABLISH_LINE_OF_FIRE,
	
		"	Tasks"
		"		TASK_SET_FAIL_SCHEDULE		SCHEDULE:SCHED_GRUNT_ELOF_FAIL"
		"		TASK_GET_PATH_TO_ENEMY		0"
		"		TASK_RUN_PATH				0"
		"		TASK_WAIT_FOR_MOVEMENT		0"
		"	"
		"	Interrupts"
		"		COND_NEW_ENEMY"
		"		COND_ENEMY_DEAD"
		"		COND_LIGHT_DAMAGE"
		"		COND_HEAVY_DAMAGE"
		"		COND_CAN_RANGE_ATTACK1"
		"		COND_CAN_MELEE_ATTACK1"
		"		COND_CAN_RANGE_ATTACK2"
		"		COND_CAN_MELEE_ATTACK2"
		"		COND_HEAR_DANGER"
	)
	
	//=========================================================
	// > SCHED_GRUNT_FOUND_ENEMY
	// Grunt established sight with an enemy
	// that was hiding from the squad.
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_GRUNT_FOUND_ENEMY,
	
		"	Tasks"
		"		TASK_STOP_MOVING				0"
		"		TASK_FACE_ENEMY					0"
		"		TASK_PLAY_SEQUENCE_FACE_ENEMY	ACTIVITY:ACT_SIGNAL1"
		"	"
		"	Interrupts"
		"		COND_HEAR_DANGER"
	)
	
	//=========================================================
	// > SCHED_GRUNT_SUPPRESS
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_GRUNT_SUPPRESS,
	
		"	Tasks"
		"		TASK_STOP_MOVING			0"
		"		TASK_FACE_IDEAL				0"
		"		TASK_GRUNT_SUPPRESS_HANDSIGNAL	0"
		"		TASK_SET_SCHEDULE			SCHEDULE:SCHED_GRUNT_RANGE_ATTACK1"
		"	"
		"	Interrupts"
		"		COND_ENEMY_DEAD"
		"		COND_LIGHT_DAMAGE"
		"		COND_HEAVY_DAMAGE"
		"		COND_GRUNT_NOFIRE"
		"		COND_NO_PRIMARY_AMMO"
		"		COND_HEAR_DANGER"
	)
	
	//=========================================================
	// > SCHED_GRUNT_TAKECOVER
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_GRUNT_TAKECOVER,
	
		"	Tasks"
		"		TASK_STOP_MOVING			0"
		"		TASK_SET_FAIL_SCHEDULE		SCHEDULE:SCHED_GRUNT_TAKECOVER_FAILED"
		"		TASK_WAIT					0.2"
		"		TASK_FIND_COVER_FROM_ENEMY	0"
		"		TASK_RUN_PATH				0"
		"		TASK_WAIT_FOR_MOVEMENT		0"
		"		TASK_REMEMBER				MEMORY:INCOVER"
		"		TASK_SET_SCHEDULE			SCHEDULE:SCHED_STANDOFF"
		"	"
		"	Interrupts"
	)
	
	
	//=========================================================
	// > SCHED_GRUNT_DROP_GRENADE_COVER
	// drop grenade then run to cover.
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_GRUNT_DROP_GRENADE_COVER,
	
		"	Tasks"
		"		TASK_STOP_MOVING						0"
		"		TASK_FIND_COVER_FROM_ENEMY				99"
		"		TASK_FIND_FAR_NODE_COVER_FROM_ENEMY		384"
		"		TASK_PLAY_SEQUENCE						ACTIVITY:ACT_SPECIAL_ATTACK1"
		"		TASK_CLEAR_MOVE_WAIT					0"
		"		TASK_RUN_PATH							0"
		"		TASK_WAIT_FOR_MOVEMENT					0"
		"		TASK_SET_SCHEDULE						SCHEDULE:SCHED_STANDOFF"
		"	"
		"	Interrupts"
	)
		
	//=========================================================
	// > SCHED_GRUNT_TOSS_GRENADE_COVER
	// drop grenade then run to cover.
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_GRUNT_TOSS_GRENADE_COVER,
	
		"	Tasks"
		"		TASK_FACE_ENEMY				0"
		"		TASK_RANGE_ATTACK2			0"
		"		TASK_SET_SCHEDULE			SCHEDULE:SCHED_GRUNT_TAKECOVER"
		"	"
		"	Interrupts"
	)
	
	//=========================================================
	// > SCHED_GRUNT_COVER_AND_RELOAD
	// Grunt reload schedule
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_GRUNT_COVER_AND_RELOAD,
	
		"	Tasks"
		"		TASK_STOP_MOVING				0"
		"		TASK_SET_FAIL_SCHEDULE			SCHEDULE:SCHED_RELOAD"
		"		TASK_FIND_COVER_FROM_ENEMY		0"
		"		TASK_RUN_PATH					0"
		"		TASK_WAIT_FOR_MOVEMENT			0"
		"		TASK_REMEMBER					MEMORY:INCOVER"
		"		TASK_RELOAD						0"
		"	"
		"	Interrupts"
		"		COND_HEAVY_DAMAGE"
		"		COND_HEAR_DANGER"
	)
	
	//=========================================================
	// > SCHED_GRUNT_SWEEP
	// Do a turning sweep of the area
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_GRUNT_SWEEP,
	
		"	Tasks"
		"		TASK_TURN_LEFT			179"
		"		TASK_WAIT				1"
		"		TASK_TURN_LEFT			179"
		"		TASK_WAIT				1"
		"	"
		"	Interrupts"
		"		COND_NEW_ENEMY"
		"		COND_LIGHT_DAMAGE"
		"		COND_HEAVY_DAMAGE"
		"		COND_CAN_RANGE_ATTACK1"
		"		COND_CAN_RANGE_ATTACK2"
		"		COND_HEAR_WORLD"
		"		COND_HEAR_DANGER"
		"		COND_HEAR_PLAYER"
	)
	
	//=========================================================
	// > SCHED_GRUNT_RANGE_ATTACK1
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_GRUNT_RANGE_ATTACK1,
	
		"	Tasks"
		"		TASK_STOP_MOVING					0"
		"		TASK_GRUNT_SELECT_IDEAL_ATTACK_SEQUENCE		0"
		"		TASK_RANGE_ATTACK1					0"
		"		TASK_FACE_ENEMY						0"
		"		TASK_RANGE_ATTACK1					0"
		"		TASK_FACE_ENEMY						0"
		"		TASK_RANGE_ATTACK1					0"
		"		TASK_FACE_ENEMY						0"
		"		TASK_RANGE_ATTACK1					0"
		"	"
		"	Interrupts"
		"		COND_NEW_ENEMY"
		"		COND_ENEMY_DEAD"
		"		COND_HEAVY_DAMAGE"
		"		COND_ENEMY_OCCLUDED"
		"		COND_HEAR_DANGER"
		"		COND_GRUNT_NOFIRE"
		"		COND_NO_PRIMARY_AMMO"
	)
	
	//=========================================================
	// > SCHED_GRUNT_RANGE_ATTACK2
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_GRUNT_RANGE_ATTACK2,
	
		"	Tasks"
		"		TASK_STOP_MOVING			0"
		"		TASK_GRUNT_FACE_TOSS_DIR	0"
		"		TASK_PLAY_SEQUENCE			ACTIVITY:ACT_RANGE_ATTACK2"
		"		TASK_SET_SCHEDULE			SCHEDULE:SCHED_STANDOFF" // don't run immediately after throwing grenade.
		"	"
		"	Interrupts"
	)
	
	//=========================================================
	// > SCHED_GRUNT_REPEL
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_GRUNT_REPEL,
	
		"	Tasks"
		"		TASK_STOP_MOVING			0"
		"		TASK_FACE_IDEAL				0"
		"		TASK_PLAY_SEQUENCE			ACTIVITY:ACT_GLIDE"
		"	"
		"	Interrupts"
		"		COND_SEE_ENEMY"
		"		COND_NEW_ENEMY"
		"		COND_LIGHT_DAMAGE"
		"		COND_HEAVY_DAMAGE"
		"		COND_HEAR_DANGER"
		"		COND_HEAR_PLAYER"
		"		COND_HEAR_COMBAT"
	)
	
	//=========================================================
	// > SCHED_GRUNT_REPEL_ATTACK
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_GRUNT_REPEL_ATTACK,
	
		"	Tasks"
		"		TASK_STOP_MOVING			0"
		"		TASK_FACE_ENEMY				0"
		"		TASK_PLAY_SEQUENCE			ACTIVITY:ACT_FLY"
		"	"
		"	Interrupts"
		"		COND_ENEMY_OCCLUDED"
	)
	
	//=========================================================
	// > SCHED_GRUNT_REPEL_LAND
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_GRUNT_REPEL_LAND,
	
		"	Tasks"
		"		TASK_STOP_MOVING					0"
		"		TASK_PLAY_SEQUENCE					ACTIVITY:ACT_LAND"
		"		TASK_GET_PATH_TO_LASTPOSITION		0"
		"		TASK_RUN_PATH						0"
		"		TASK_WAIT_FOR_MOVEMENT				0"
		"		TASK_CLEAR_LASTPOSITION				0"
		"	"
		"	Interrupts"
		"		COND_SEE_ENEMY"
		"		COND_NEW_ENEMY"
		"		COND_LIGHT_DAMAGE"
		"		COND_HEAVY_DAMAGE"
		"		COND_HEAR_DANGER"
		"		COND_HEAR_COMBAT"
		"		COND_HEAR_PLAYER"
	)
	
	//=========================================================
	// > SCHED_GRUNT_TAKE_COVER_FAILED
	//=========================================================
	DEFINE_SCHEDULE
	(
		SCHED_GRUNT_TAKECOVER_FAILED,
		"	Tasks"
		"	Interrupts"
	)

AI_END_CUSTOM_NPC()