//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef NPC_HGRUNT_H
#define NPC_HGRUNT_H
#ifdef _WIN32
#pragma once
#endif

#include    "ai_basenpc.h"
#include    "ai_squad.h"
#include	"ai_behavior_follow.h"
#include	"ai_moveprobe.h"
#include	"ai_senses.h"
#include	"ai_speech.h"
#include	"ai_task.h"
#include	"ai_default.h"
#include	"ai_schedule.h"
#include	"ai_hull.h"
#include	"ai_baseactor.h"
#include	"ai_waypoint.h"
#include	"ai_link.h"
#include	"ai_hint.h"
#include	"ai_squadslot.h"
#include	"ai_squad.h"
#include	"ai_tacticalservices.h"

#define SF_GRUNT_LEADER	( 1 << 5  )
#define SF_GRUNT_FRIENDLY ( 1 << 6  )
#define SF_GRUNT_ROBOT ( 1 << 15  )

//=========================================================
// monster-specific DEFINE's
//=========================================================
#define HGRUNT_9MMAR				( 1 << 0)
#define HGRUNT_HANDGRENADE			( 1 << 1)
#define HGRUNT_GRENADELAUNCHER		( 1 << 2)
#define HGRUNT_SHOTGUN				( 1 << 3)

typedef struct
{
	int id;
	int body;
} GruntBodygroups_t;

enum eBodygroupTypes
{
	HEAD_GROUP = 1,
	GUN_GROUP
};

enum eHeadBodygroupTypes
{
	HEAD_GRUNT,
	HEAD_COMMANDER,
	HEAD_SHOTGUN,
	HEAD_M203
};

enum eWeaponBodygroupTypes
{
	GUN_MP5,
	GUN_SHOTGUN,
	GUN_NONE
};

class CHGruntBodygroupLoader
{
public:
	static GruntBodygroups_t HGruntWeaponBodygroupMap[];
	static GruntBodygroups_t HGruntHeadBodygroupMap[];

	static void SwitchBodygroupForWeapon(CBaseAnimating* pent, int body);
	static void SwitchBodygroupForHead(CBaseAnimating* pent, int body);
};

//=========================================================
// Monster's Anim Events Go Here
//=========================================================
#define		HGRUNT_AE_RELOAD		( 2 )
#define		HGRUNT_AE_KICK			( 3 )
#define		HGRUNT_AE_BURST1		( 4 )
#define		HGRUNT_AE_BURST2		( 5 ) 
#define		HGRUNT_AE_BURST3		( 6 ) 
#define		HGRUNT_AE_GREN_TOSS		( 7 )
#define		HGRUNT_AE_GREN_LAUNCH	( 8 )
#define		HGRUNT_AE_GREN_DROP		( 9 )
#define		HGRUNT_AE_CAUGHT_ENEMY	( 10) // grunt established sight with an enemy (player only) that had previously eluded the squad.
#define		HGRUNT_AE_DROP_GUN		( 11) // grunt (probably dead) is dropping his mp5.

//=========================================================
// monster-specific schedule types
//=========================================================
enum
{ 
	SCHED_GRUNT_SUPPRESS = LAST_SHARED_SCHEDULE,
	SCHED_GRUNT_ESTABLISH_LINE_OF_FIRE,
	SCHED_GRUNT_COVER_AND_RELOAD,
	SCHED_GRUNT_SWEEP,
	SCHED_GRUNT_FOUND_ENEMY,
	SCHED_GRUNT_REPEL,
	SCHED_GRUNT_REPEL_ATTACK,
	SCHED_GRUNT_REPEL_LAND,
	SCHED_GRUNT_TAKECOVER_FAILED,

	SCHED_GRUNT_TAKECOVER,
	SCHED_GRUNT_RANGE_ATTACK1,
	SCHED_GRUNT_RANGE_ATTACK2,

	SCHED_GRUNT_DROP_GRENADE_COVER,
	SCHED_GRUNT_TOSS_GRENADE_COVER,

	SCHED_GRUNT_ELOF_FAIL,
};

//=========================================================
// monster-specific tasks
//=========================================================
enum 
{
	TASK_GRUNT_FACE_TOSS_DIR = LAST_SHARED_TASK + 1,
	TASK_GRUNT_SELECT_IDEAL_ATTACK_SEQUENCE,
	TASK_GRUNT_SUPPRESS_HANDSIGNAL,
};


//=========================================================
// monster-specific conditions
//=========================================================
enum
{
	COND_GRUNT_NOFIRE = LAST_SHARED_CONDITION + 1,
};

// -----------------------------------------------
//	> Squad slots
// -----------------------------------------------
enum SquadSlot_T
{	
	SQUAD_SLOT_GRENADE1 = LAST_SHARED_SQUADSLOT,
	SQUAD_SLOT_GRENADE2,
	SQUAD_SLOT_CHARGE1,
	SQUAD_SLOT_CHARGE2,
};

//mainly for the spawner.
enum eHGruntWeapons
{
	WEAPON_HGRUNT_SMG = HGRUNT_9MMAR,
	WEAPON_HGRUNT_SMG_FRAG = (HGRUNT_9MMAR | HGRUNT_HANDGRENADE),
	WEAPON_HGRUNT_SMG_GL = (HGRUNT_9MMAR | HGRUNT_GRENADELAUNCHER),
	WEAPON_HGRUNT_SHOTGUN = HGRUNT_SHOTGUN,
	WEAPON_HGRUNT_SHOTGUN_FRAG = (HGRUNT_SHOTGUN | HGRUNT_HANDGRENADE),
	WEAPON_HGRUNT_SHOTGUN_GL = (HGRUNT_SHOTGUN | HGRUNT_GRENADELAUNCHER),
};

typedef struct
{
	int id;
	const char* name;
} GruntSentences_t;

enum eGruntSentenceTypes
{
	SENT_GREN,
	SENT_ALERT,
	SENT_MONST,
	SENT_COVER,
	SENT_THROW,
	SENT_CHARGE,
	SENT_TAUNT,
	SENT_TAUNT_EASTEREGG,
	SENT_CHECK,
	SENT_QUEST,
	SENT_IDLE,
	SENT_CLEAR,
	SENT_ANSWER,
	SENT_TAUNT_PLAYERHURT,
};

class CHGruntSentenceLoader
{
public:

	static GruntSentences_t HGruntSentenceMap[];
	static const char* GetNameForSentence(int iTeamNumber);
};

class CHGrunt : public CAI_BaseActor
{
	DECLARE_CLASS( CHGrunt, CAI_BaseActor);
public:
	void	Spawn( void );
	void	BecomeFriendly(void);
	void	FriendlyEscortCheck();
	bool	CreateBehaviors();
	void	GatherConditions(void);
	void	LoadInitAttributes();
    void	Precache( void );
    float	MaxYawSpeed( void );
	Class_T	Classify(void);
    int     GetSoundInterests( void ) { return (SOUND_WORLD | SOUND_COMBAT | SOUND_PLAYER | SOUND_BULLET_IMPACT | SOUND_DANGER); }
    void	HandleAnimEvent( animevent_t *pEvent );
    bool	FCanCheckAttacks( void );
   	int     RangeAttack1Conditions( float flDot, float flDist );
	int		MeleeAttack1Conditions( float flDot, float flDist );
	int     RangeAttack2Conditions( float flDot, float flDist );
    void	ClearAttackConditions( void );
	int     GetGrenadeConditions ( float flDot, float flDist );
    void	CheckAmmo( void );
    Activity NPC_TranslateActivity( Activity NewActivity );
    void	StartTask( const Task_t *pTask );
	void	RunTask( const Task_t *pTask );
    void	StartNPC( void );
	void	NPCThink();
    void	PainSound( const CTakeDamageInfo &info );
	void	DeathSound( const CTakeDamageInfo &info );
    void    IdleSound( void );
    Vector	Weapon_ShootPosition( void );
    int		SquadRecruit( int searchRadius, int maxMembers );
    void	Shoot (int bulletnum, Vector cone);
    void	PrescheduleThink ( void );
    void	Event_Killed( const CTakeDamageInfo &info );
	void	SetAndSpeakSentence(int sentenceGroup);
    void	SpeakSentence( void );
	void	TauntEnemy( bool bHurtTaunt = false );
    
    CBaseEntity *Kick( void );
	int		SelectSchedule( void );
	int		TranslateSchedule( int scheduleType );
    void    TraceAttack( const CTakeDamageInfo &info, const Vector &vecDir, trace_t *ptr, CDmgAccumulator* pAccumulator);
	int		OnTakeDamage_Alive( const CTakeDamageInfo &inputInfo );
    
	int		IRelationPriority( CBaseEntity *pTarget );

	bool	FOkToSpeak( void );
    void	JustSpoke( void );
	
    DECLARE_DATADESC();
	DEFINE_CUSTOM_AI;

	// checking the feasibility of a grenade toss is kind of costly, so we do it every couple of seconds,
	// not every server frame.
	float m_flNextGrenadeCheck;
	float m_flNextPainTime;
	float m_flLastEnemySightTime;
	float m_flTalkWaitTime;

	Vector	m_vecTossVelocity;

	int		m_iLastGrenadeCondition;
	bool	m_fStanding;
	bool	m_fIsFriendly;
	bool	m_fFirstEncounter;// only put on the handsign show in the squad's first encounter.
	int		m_iClipSize;

	int		m_voicePitch;

	int		m_iSentence;

	float	m_flCheckAttackTime;

	int		m_iAmmoType;
	
	int		m_iWeapons;

	int		m_fGruntQuestion;				// true if an idle grunt asked a question. Cleared when someone answers.
	int		m_iSquadIndex;

	CAI_FollowBehavior			m_FollowBehavior;
};

#endif // MONSTERMAKER_H