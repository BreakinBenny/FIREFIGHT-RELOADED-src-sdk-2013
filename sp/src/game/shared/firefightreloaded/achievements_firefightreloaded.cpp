//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================


#include "cbase.h"
#include "achievementmgr.h"
#include "baseachievement.h"

#ifdef GAME_DLL
#include "basegrenade_shared.h"
#include "basehlcombatweapon_shared.h"
#include "ammodef.h"
#include "weapon_katana.h"
#include "ai_basenpc.h"

#ifndef MOD_VER
CAchievementMgr AchievementMgr;
#endif

class CFRBaseAchievement : public CBaseAchievement
{
public:
	virtual void OnAchieved() 
	{
		CBasePlayer* pEntity = UTIL_GetLocalPlayer();

		if (!pEntity)
			return;

		pEntity->AddXP(GetPointValue() * pEntity->m_iExpBoostMult);
		pEntity->AddMoney(GetPointValue() * pEntity->m_iKashBoostMult);
	}
};

class CFRMapAchievement : public CFRBaseAchievement
{
	virtual void Init()
	{
		SetFlags(ACH_LISTEN_MAP_EVENTS | ACH_SAVE_GLOBAL);
		SetGoal(1);
	}
};

#define DECLARE_FR_MAP_EVENT_ACHIEVEMENT( achievementID, achievementName, iPointValue, bHidden ) \
class CFRMapAchievement##achievementID : public CFRMapAchievement {};		\
DECLARE_ACHIEVEMENT_( CFRMapAchievement##achievementID, achievementID, achievementName, "firefightreloaded", iPointValue, bHidden )	\

class CAchievementKill20CombineSoldier : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter( "firefightreloaded" );
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal( 20 );
	}

	virtual void Event_EntityKilled( CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event ) 
	{
		if (FClassnameIs(pVictim, "npc_combine_s") || 
			FClassnameIs(pVictim, "npc_combine_e") || 
			FClassnameIs(pVictim, "npc_combine_p") || 
			FClassnameIs(pVictim, "npc_combine_shot") || 
			FClassnameIs(pVictim, "npc_combine_ace") || 
			FClassnameIs(pVictim, "npc_metropolice") || 
			FClassnameIs(pVictim, "npc_assassin") || 
			FClassnameIs(pVictim, "npc_manhack"))
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return false; }
};
DECLARE_ACHIEVEMENT(CAchievementKill20CombineSoldier, ACHIEVEMENT_FIREFIGHTRELOADED_KILL20COMBINE, "FIREFIGHTRELOADED_KILL20COMBINE", 5);

class CAchievementKill50CombineSoldier : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(50);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		if (FClassnameIs(pVictim, "npc_combine_s") || 
			FClassnameIs(pVictim, "npc_combine_e") || 
			FClassnameIs(pVictim, "npc_combine_p") || 
			FClassnameIs(pVictim, "npc_combine_shot") || 
			FClassnameIs(pVictim, "npc_combine_ace") || 
			FClassnameIs(pVictim, "npc_metropolice") || 
			FClassnameIs(pVictim, "npc_assassin") || 
			FClassnameIs(pVictim, "npc_manhack"))
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return false; }
};
DECLARE_ACHIEVEMENT(CAchievementKill50CombineSoldier, ACHIEVEMENT_FIREFIGHTRELOADED_KILL50COMBINE, "FIREFIGHTRELOADED_KILL50COMBINE", 10);

class CAchievementKill100CombineSoldier : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(100);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		if (FClassnameIs(pVictim, "npc_combine_s") || 
			FClassnameIs(pVictim, "npc_combine_e") || 
			FClassnameIs(pVictim, "npc_combine_p") || 
			FClassnameIs(pVictim, "npc_combine_shot") || 
			FClassnameIs(pVictim, "npc_combine_ace") || 
			FClassnameIs(pVictim, "npc_metropolice") || 
			FClassnameIs(pVictim, "npc_assassin") || 
			FClassnameIs(pVictim, "npc_manhack"))
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementKill100CombineSoldier, ACHIEVEMENT_FIREFIGHTRELOADED_KILL100COMBINE, "FIREFIGHTRELOADED_KILL100COMBINE", 15);

class CAchievementKill20Hunters : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetVictimFilter("npc_hunter");
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(20);
	}

	virtual bool ShouldShowProgressNotification() { return false; }

};
DECLARE_ACHIEVEMENT(CAchievementKill20Hunters, ACHIEVEMENT_FIREFIGHTRELOADED_KILL20HUNTERS, "FIREFIGHTRELOADED_KILL20HUNTERS", 10);

class CAchievementKill50Hunters : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetVictimFilter("npc_hunter");
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(50);
	}

	virtual bool ShouldShowProgressNotification() { return false; }
};
DECLARE_ACHIEVEMENT(CAchievementKill50Hunters, ACHIEVEMENT_FIREFIGHTRELOADED_KILL50HUNTERS, "FIREFIGHTRELOADED_KILL50HUNTERS", 15);

class CAchievementKill100Hunters : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetVictimFilter("npc_hunter");
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(100);
	}

	virtual bool ShouldShowProgressNotification() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementKill100Hunters, ACHIEVEMENT_FIREFIGHTRELOADED_KILL100HUNTERS, "FIREFIGHTRELOADED_KILL100HUNTERS", 20);

class CAchievementReachMaxLevel : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_SAVE_WITH_GAME);
		SetGameDirFilter("firefightreloaded");
		SetGoal(1);
	}

	virtual void ListenForEvents()
	{
		ListenForGameEvent("player_maxlevel");
	}

	void FireGameEvent_Internal(IGameEvent *event)
	{
		if (0 == Q_strcmp(event->GetName(), "player_maxlevel"))
		{
			IncrementCount();
		}
	}
};
DECLARE_ACHIEVEMENT(CAchievementReachMaxLevel, ACHIEVEMENT_FIREFIGHTRELOADED_REACHMAXLEVEL, "FIREFIGHTRELOADED_REACHMAXLEVEL", 40);

class CAchievementKill100Enemies : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_WITH_GAME);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(100);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		if (!g_pGameRules->IsSkillLevel(SKILL_NIGHTMARE))
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementKill100Enemies, ACHIEVEMENT_FIREFIGHTRELOADED_KILL100ENEMIES, "FIREFIGHTRELOADED_KILL100ENEMIES", 10);

class CAchievementKillThousandEnemies : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(1000);
	}

	virtual bool ShouldShowProgressNotification() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementKillThousandEnemies, ACHIEVEMENT_FIREFIGHTRELOADED_KILLTHOUSANDENEMIES, "FIREFIGHTRELOADED_KILLTHOUSANDENEMIES", 25);

class CAchievementKillCombineSoldierWithFlechette : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_KILL_EVENTS | ACH_SAVE_WITH_GAME);
		SetInflictorFilter("hunter_flechette");
		SetGameDirFilter("firefightreloaded");
		SetGoal(1);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		if (FClassnameIs(pVictim, "npc_combine_s") || 
			FClassnameIs(pVictim, "npc_combine_e") || 
			FClassnameIs(pVictim, "npc_combine_p") || 
			FClassnameIs(pVictim, "npc_combine_shot") || 
			FClassnameIs(pVictim, "npc_combine_ace") || 
			FClassnameIs(pVictim, "npc_metropolice"))
		{
			IncrementCount();
		}
	}
};
DECLARE_ACHIEVEMENT(CAchievementKillCombineSoldierWithFlechette, ACHIEVEMENT_FIREFIGHTRELOADED_KILLCOMBINEWITHFLECHETTE, "FIREFIGHTRELOADED_KILLCOMBINEWITHFLECHETTE", 10);

class CAchievementKill10HuntersWithFlechette : public CFRBaseAchievement
{
protected:
	void Init()
	{
		SetFlags(ACH_LISTEN_KILL_EVENTS | ACH_SAVE_GLOBAL);
		SetInflictorFilter("hunter_flechette");
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(10);
	}

	virtual void Event_EntityKilled(CBaseEntity* pVictim, CBaseEntity* pAttacker, CBaseEntity* pInflictor, IGameEvent* event)
	{
		if (FClassnameIs(pVictim, "npc_hunter"))
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementKill10HuntersWithFlechette, ACHIEVEMENT_FIREFIGHTRELOADED_KILL10HUNTERSWITHFLECHETTE, "FIREFIGHTRELOADED_KILL10HUNTERSWITHFLECHETTE", 20);

class CAchievementKill100EnemiesOnNightmare : public CFRBaseAchievement
{
protected:
	void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_WITH_GAME);
		SetGameDirFilter("firefightreloaded");
		SetGoal(100);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		if (g_pGameRules->IsSkillLevel(SKILL_NIGHTMARE))
		{
			IncrementCount();
		}
	}
};
DECLARE_ACHIEVEMENT(CAchievementKill100EnemiesOnNightmare, ACHIEVEMENT_FIREFIGHTRELOADED_KILL100ENEMIESONNIGHTMARE, "FIREFIGHTRELOADED_KILL100ENEMIESONNIGHTMARE", 45);

class CAchievementKill20Antlion : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(20);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		if (FClassnameIs(pVictim, "npc_antlion") || 
			FClassnameIs(pVictim, "npc_antlionworker") || 
			FClassnameIs(pVictim, "npc_antlionguard") ||
			FClassnameIs(pVictim, "npc_antlionguardian"))
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return false; }
};
DECLARE_ACHIEVEMENT(CAchievementKill20Antlion, ACHIEVEMENT_FIREFIGHTRELOADED_KILL20ANTLIONS, "FIREFIGHTRELOADED_KILL20ANTLIONS", 5);

class CAchievementKill50Antlion : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(50);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		if (FClassnameIs(pVictim, "npc_antlion") || 
			FClassnameIs(pVictim, "npc_antlionworker") || 
			FClassnameIs(pVictim, "npc_antlionguard") ||
			FClassnameIs(pVictim, "npc_antlionguardian"))
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return false; }
};
DECLARE_ACHIEVEMENT(CAchievementKill50Antlion, ACHIEVEMENT_FIREFIGHTRELOADED_KILL50ANTLIONS, "FIREFIGHTRELOADED_KILL50ANTLIONS", 10);

class CAchievementKill100Antlion : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(100);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		if (FClassnameIs(pVictim, "npc_antlion") || 
			FClassnameIs(pVictim, "npc_antlionworker") || 
			FClassnameIs(pVictim, "npc_antlionguard") ||
			FClassnameIs(pVictim, "npc_antlionguardian"))
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementKill100Antlion, ACHIEVEMENT_FIREFIGHTRELOADED_KILL100ANTLIONS, "FIREFIGHTRELOADED_KILL100ANTLIONS", 15);

class CAchievementKill20AntlionGuard : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(20);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		if (FClassnameIs(pVictim, "npc_antlionguard") || 
			FClassnameIs(pVictim, "npc_antlionguardian"))
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return false; }
};
DECLARE_ACHIEVEMENT(CAchievementKill20AntlionGuard, ACHIEVEMENT_FIREFIGHTRELOADED_KILL20ANTLIONGUARDS, "FIREFIGHTRELOADED_KILL20ANTLIONGUARDS", 10);

class CAchievementKill50AntlionGuard : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(50);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		if (FClassnameIs(pVictim, "npc_antlionguard") || 
			FClassnameIs(pVictim, "npc_antlionguardian"))
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return false; }
};
DECLARE_ACHIEVEMENT(CAchievementKill50AntlionGuard, ACHIEVEMENT_FIREFIGHTRELOADED_KILL50ANTLIONGUARDS, "FIREFIGHTRELOADED_KILL50ANTLIONGUARDS", 15);

class CAchievementKill100AntlionGuard : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(100);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		if (FClassnameIs(pVictim, "npc_antlionguard") || 
			FClassnameIs(pVictim, "npc_antlionguardian"))
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementKill100AntlionGuard, ACHIEVEMENT_FIREFIGHTRELOADED_KILL100ANTLIONGUARDS, "FIREFIGHTRELOADED_KILL100ANTLIONGUARDS", 20);

class CAchievementKill20Zombies : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(20);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		if (FClassnameIs(pVictim, "npc_zombie") || 
			FClassnameIs(pVictim, "npc_zombie_torso") || 
			FClassnameIs(pVictim, "npc_fastzombie") || 
			FClassnameIs(pVictim, "npc_fastzombie_torso") || 
			FClassnameIs(pVictim, "npc_poisonzombie") || 
			FClassnameIs(pVictim, "npc_zombine"))
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return false; }
};
DECLARE_ACHIEVEMENT(CAchievementKill20Zombies, ACHIEVEMENT_FIREFIGHTRELOADED_KILL20ZOMBIES, "FIREFIGHTRELOADED_KILL20ZOMBIES", 5);

class CAchievementKill50Zombies : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(50);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		if (FClassnameIs(pVictim, "npc_zombie") || 
			FClassnameIs(pVictim, "npc_zombie_torso") || 
			FClassnameIs(pVictim, "npc_fastzombie") || 
			FClassnameIs(pVictim, "npc_fastzombie_torso") || 
			FClassnameIs(pVictim, "npc_poisonzombie") || 
			FClassnameIs(pVictim, "npc_zombine"))
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return false; }
};
DECLARE_ACHIEVEMENT(CAchievementKill50Zombies, ACHIEVEMENT_FIREFIGHTRELOADED_KILL50ZOMBIES, "FIREFIGHTRELOADED_KILL50ZOMBIES", 10);

class CAchievementKill100Zombies : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(100);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		if (FClassnameIs(pVictim, "npc_zombie") || 
			FClassnameIs(pVictim, "npc_zombie_torso") || 
			FClassnameIs(pVictim, "npc_fastzombie") || 
			FClassnameIs(pVictim, "npc_fastzombie_torso") || 
			FClassnameIs(pVictim, "npc_poisonzombie") || 
			FClassnameIs(pVictim, "npc_zombine"))
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementKill100Zombies, ACHIEVEMENT_FIREFIGHTRELOADED_KILL100ZOMBIES, "FIREFIGHTRELOADED_KILL100ZOMBIES", 15);

class CAchievementKill20Headcrabs : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(20);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		if (FClassnameIs(pVictim, "npc_headcrab") || 
			FClassnameIs(pVictim, "npc_headcrab_fast") || 
			FClassnameIs(pVictim, "npc_headcrab_poison"))
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return false; }
};
DECLARE_ACHIEVEMENT(CAchievementKill20Headcrabs, ACHIEVEMENT_FIREFIGHTRELOADED_KILL20HEADCRABS, "FIREFIGHTRELOADED_KILL20HEADCRABS", 5);

class CAchievementKill50Headcrabs : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(50);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		if (FClassnameIs(pVictim, "npc_headcrab") || 
			FClassnameIs(pVictim, "npc_headcrab_fast") || 
			FClassnameIs(pVictim, "npc_headcrab_poison"))
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return false; }
};
DECLARE_ACHIEVEMENT(CAchievementKill50Headcrabs, ACHIEVEMENT_FIREFIGHTRELOADED_KILL50HEADCRABS, "FIREFIGHTRELOADED_KILL50HEADCRABS", 10);

class CAchievementKill100Headcrabs : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(100);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		if (FClassnameIs(pVictim, "npc_headcrab") || 
			FClassnameIs(pVictim, "npc_headcrab_fast") || 
			FClassnameIs(pVictim, "npc_headcrab_poison"))
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementKill100Headcrabs, ACHIEVEMENT_FIREFIGHTRELOADED_KILL100HEADCRABS, "FIREFIGHTRELOADED_KILL100HEADCRABS", 15);

class CAchievementKill20XenCreatures : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(20);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		if (FClassnameIs(pVictim, "npc_houndeye") || 
			FClassnameIs(pVictim, "npc_bullsquid") || 
			FClassnameIs(pVictim, "npc_vortigaunt") || 
			FClassnameIs(pVictim, "npc_agrunt") || 
			FClassnameIs(pVictim, "npc_acontroller"))
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return false; }
};
DECLARE_ACHIEVEMENT(CAchievementKill20XenCreatures, ACHIEVEMENT_FIREFIGHTRELOADED_KILL20XENCREATURES, "FIREFIGHTRELOADED_KILL20XENCREATURES", 5);

class CAchievementKill50XenCreatures : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(50);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		if (FClassnameIs(pVictim, "npc_houndeye") || 
			FClassnameIs(pVictim, "npc_bullsquid") || 
			FClassnameIs(pVictim, "npc_vortigaunt") || 
			FClassnameIs(pVictim, "npc_agrunt") || 
			FClassnameIs(pVictim, "npc_acontroller"))
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return false; }
};
DECLARE_ACHIEVEMENT(CAchievementKill50XenCreatures, ACHIEVEMENT_FIREFIGHTRELOADED_KILL50XENCREATURES, "FIREFIGHTRELOADED_KILL50XENCREATURES", 10);

class CAchievementKill100XenCreatures : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
#ifndef MOD_VER
		SetStoreProgressInSteam( true );
#endif
		SetGoal(100);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		if (FClassnameIs(pVictim, "npc_houndeye") || 
			FClassnameIs(pVictim, "npc_bullsquid") || 
			FClassnameIs(pVictim, "npc_vortigaunt") || 
			FClassnameIs(pVictim, "npc_agrunt") || 
			FClassnameIs(pVictim, "npc_acontroller"))
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementKill100XenCreatures, ACHIEVEMENT_FIREFIGHTRELOADED_KILL100XENCREATURES, "FIREFIGHTRELOADED_KILL100XENCREATURES", 15);

class CAchievementKill10EnemiesWithGrenades : public CFRBaseAchievement
{
protected:
	void Init()
	{
		SetInflictorFilter("grenade_ar2");
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_WITH_GAME);
		SetGameDirFilter("firefightreloaded");
		SetGoal(10);
	}

	virtual bool ShouldShowProgressNotification() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementKill10EnemiesWithGrenades, ACHIEVEMENT_FIREFIGHTRELOADED_KILL10ENEMIESWITHGRENADE, "FIREFIGHTRELOADED_KILL10ENEMIESWITHGRENADE", 35);

extern int CalcPlayerAttacks(bool bBulletOnly);

class CAchievementKill10EnemiesZeroBullets : public CFailableAchievementNoEvent
{
	DECLARE_CLASS(CAchievementKill10EnemiesZeroBullets, CFailableAchievementNoEvent);
protected:

	void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_WITH_GAME);
		SetGameDirFilter("firefightreloaded");
		SetGoal(10);
	}

	virtual void CheckIncrementCount()
	{
		// get # of attacks w/bullet weapons
		int iBulletAttackCount = CalcPlayerAttacks(true);
		// if more than 0 bullets fired, fail
		if (iBulletAttackCount > 0)
		{
			SetFailed();
		}
		BaseClass::CheckIncrementCount();
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		CheckIncrementCount();
	}

	virtual bool ShouldShowProgressNotification() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementKill10EnemiesZeroBullets, ACHIEVEMENT_FIREFIGHTRELOADED_KILL10ENEMIESZEROBULLETS, "FIREFIGHTRELOADED_KILL10ENEMIESZEROBULLETS", 20);

class CAchievementKill20EnemiesNoDamage : public CFailableAchievementNoEvent
{
	DECLARE_CLASS(CAchievementKill20EnemiesNoDamage, CFailableAchievementNoEvent);
protected:

	void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_WITH_GAME);
		SetGameDirFilter("firefightreloaded");
		SetGoal(20);
	}

	virtual void CheckIncrementCount()
	{
		CBasePlayer *pPlayer = UTIL_GetLocalPlayer();
		// if The player's health is less than the maximum, fail
		//DevMsg("PLAYER HEALTH: %i PLAYER MAX HEALTH: %i", pPlayer->GetHealth(), pPlayer->GetMaxHealthValue());
		//DevMsg("IS PLAYER ALLOWED TO GET CAchievementKill20EnemiesNoDamage? %i\n", !(pPlayer->GetHealth() < pPlayer->GetMaxHealthValue()));
		if (pPlayer->GetHealth() < pPlayer->GetMaxHealthValue())
		{
			SetFailed();
		}
		BaseClass::CheckIncrementCount();
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		CheckIncrementCount();
	}

	virtual bool ShouldShowProgressNotification() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementKill20EnemiesNoDamage, ACHIEVEMENT_FIREFIGHTRELOADED_KILL20ENEMIESNODAMAGE, "FIREFIGHTRELOADED_KILL20ENEMIESNODAMAGE", 25);

class CAchievementKill10EnemiesAtLowHealth : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_WITH_GAME);
		SetGameDirFilter("firefightreloaded");
		SetGoal(10);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		CBasePlayer *pPlayer = UTIL_GetLocalPlayer();
		if (pPlayer->GetHealth() <= 30)
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementKill10EnemiesAtLowHealth, ACHIEVEMENT_FIREFIGHTRELOADED_KILL10ENEMIESATLOWHEALTH, "FIREFIGHTRELOADED_KILL10ENEMIESATLOWHEALTH", 15);

class CAchievementKill100EnemiesOnDeath : public CFRBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_SAVE_WITH_GAME);
		SetGameDirFilter("firefightreloaded");
		SetGoal(1);
	}

	virtual void ListenForEvents()
	{
		ListenForGameEvent("player_death");
		ListenForGameEvent("player_death_npc");
	}

	void FireGameEvent_Internal(IGameEvent* event)
	{
		CBasePlayer* victim = UTIL_PlayerByUserId(event->GetInt("userid"));
		if (victim)
		{
			int iFragGoal = 100;
			if (Q_strcmp(event->GetName(), "player_death") == 0)
			{
				if (victim->FragCount() >= iFragGoal)
				{
					IncrementCount();
				}
			}
			else if (Q_strcmp(event->GetName(), "player_death_npc") == 0)
			{
				if (victim->FragCount() >= iFragGoal)
				{
					IncrementCount();
				}
			}
		}
	}

	virtual bool ShouldShowProgressNotification() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementKill100EnemiesOnDeath, ACHIEVEMENT_FIREFIGHTRELOADED_KILL100ENEMIESONDEATH, "FIREFIGHTRELOADED_KILL100ENEMIESONDEATH", 20);

class CAchievementKill10EnemiesWithOwnGrenade : public CFRBaseAchievement
{
protected:
	virtual void Init()
	{
		SetFlags(ACH_LISTEN_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetInflictorFilter("npc_grenade_frag");
		SetGameDirFilter("firefightreloaded");
		SetGoal(10);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		CBaseGrenade *pGrenade = dynamic_cast<CBaseGrenade *>(pInflictor);
		if (pGrenade)
		{
			CBaseEntity *pThrower = pGrenade->GetThrower();
			CBaseEntity *pOriginalThrower = pGrenade->GetOriginalThrower();
			// check if player was most recent thrower, but the victim was the original thrower
			if (pThrower && pOriginalThrower && pThrower->IsPlayer() && (pOriginalThrower == pVictim))
			{
				IncrementCount();
			}
		}
	}

	virtual bool ShouldShowProgressNotification() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementKill10EnemiesWithOwnGrenade, ACHIEVEMENT_FIREFIGHTRELOADED_KILL10ENEMIESWITHOWNGRENADE, "FIREFIGHTRELOADED_KILL10ENEMIESWITHOWNGRENADE", 35);

//ENDGAME
DECLARE_FR_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_FIREFIGHTRELOADED_COMPLETEDBOSSFIGHT, "FIREFIGHTRELOADED_COMPLETEDBOSSFIGHT", 50, false);
DECLARE_FR_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_FIREFIGHTRELOADED_COMPLETEDTUTORIAL, "FIREFIGHTRELOADED_COMPLETEDTUTORIAL", 50, false);

//INVASION
class CAchievementKillMaxKatanaMultiplier : public CFRBaseAchievement
{
protected:
	virtual void Init()
	{
		SetFlags(ACH_LISTEN_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
		SetGoal(1);
	}

	virtual void Event_EntityKilled(CBaseEntity* pVictim, CBaseEntity* pAttacker, CBaseEntity* pInflictor, IGameEvent* event)
	{
		CBasePlayer* pPlayer = ToBasePlayer(pAttacker);
		if (!pPlayer)
			return;

		CBaseCombatWeapon* pWeapon = pPlayer->GetActiveWeapon();
		if (pWeapon)
		{
			CWeaponKatana* pKatana = dynamic_cast<CWeaponKatana*>(pWeapon);
			if (pKatana)
			{
				if (!pKatana->IsKillMultiplierEnabled())
					return;

				if (pKatana->GetKillMultiplier() == sv_katana_healthbonus_maxmultiplier.GetInt() - 1)
				{
					IncrementCount();
				}
			}
		}
	}

	virtual bool ShouldShowProgressNotification() { return false; }
	virtual bool IsExperimental() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementKillMaxKatanaMultiplier, ACHIEVEMENT_FIREFIGHTRELOADED_MAXKATANAMULTIPLIER, "FIREFIGHTRELOADED_MAXKATANAMULTIPLIER", 50);

class CAchievementSkybornePin : public CFRBaseAchievement
{
protected:
	virtual void Init()
	{
		SetFlags(ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
		SetGoal(1);
	}

	//earned through entity/s itself

	virtual bool ShouldShowProgressNotification() { return false; }
	virtual bool IsExperimental() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementSkybornePin, ACHIEVEMENT_FIREFIGHTRELOADED_SKYBORNEPIN, "FIREFIGHTRELOADED_SKYBORNEPIN", 50);

class CAchievementMultiKillstreak : public CFRBaseAchievement
{
protected:
	virtual void Init()
	{
		SetFlags(ACH_LISTEN_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
		SetGoal(1);
	}

	virtual void Event_EntityKilled(CBaseEntity* pVictim, CBaseEntity* pAttacker, CBaseEntity* pInflictor, IGameEvent* event)
	{
		CBasePlayer* pPlayer = ToBasePlayer(pAttacker);
		if (!pPlayer)
			return;

		if (pPlayer->m_iCompleteKillstreakCount > 1)
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return false; }
	virtual bool IsExperimental() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementMultiKillstreak, ACHIEVEMENT_FIREFIGHTRELOADED_MULTIKILLSTREAK, "FIREFIGHTRELOADED_MULTIKILLSTREAK", 50);

class CAchievementSkyborneDecap : public CFRBaseAchievement
{
protected:
	virtual void Init()
	{
		SetFlags(ACH_LISTEN_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGameDirFilter("firefightreloaded");
		SetGoal(1);
	}

	virtual void Event_EntityKilled(CBaseEntity* pVictim, CBaseEntity* pAttacker, CBaseEntity* pInflictor, IGameEvent* event)
	{
		CAI_BaseNPC* pNPC = pVictim->MyNPCPointer();
		if (!pNPC)
			return;

		bool isFallingWithStyle = ((pNPC->GetGroundEntity() == NULL && !(pNPC->GetMoveType() == MOVETYPE_FLY || pNPC->GetMoveType() == MOVETYPE_FLYGRAVITY || pNPC->GetMoveType() == MOVETYPE_VPHYSICS)));

		if (pNPC->m_bDecapitated && isFallingWithStyle)
		{
			IncrementCount();
		}
	}

	virtual bool ShouldShowProgressNotification() { return false; }
	virtual bool IsExperimental() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementSkyborneDecap, ACHIEVEMENT_FIREFIGHTRELOADED_SKYBORNEDECAP, "FIREFIGHTRELOADED_SKYBORNEDECAP", 50);

#endif // GAME_DLL