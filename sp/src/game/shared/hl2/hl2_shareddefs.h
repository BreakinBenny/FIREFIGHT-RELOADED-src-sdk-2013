//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef HL2_SHAREDDEFS_H
#define HL2_SHAREDDEFS_H

#ifdef _WIN32
#pragma once
#endif

#include "const.h"


//--------------------------------------------------------------------------
// Collision groups
//--------------------------------------------------------------------------

enum
{
	HL2COLLISION_GROUP_PLASMANODE = LAST_SHARED_COLLISION_GROUP,
	HL2COLLISION_GROUP_SPIT,
	HL2COLLISION_GROUP_HOMING_MISSILE,
	HL2COLLISION_GROUP_COMBINE_BALL,

	HL2COLLISION_GROUP_FIRST_NPC,
	HL2COLLISION_GROUP_HOUNDEYE,
	HL2COLLISION_GROUP_CROW,
	HL2COLLISION_GROUP_HEADCRAB,
	HL2COLLISION_GROUP_STRIDER,
	HL2COLLISION_GROUP_GUNSHIP,
	HL2COLLISION_GROUP_ANTLION,
	HL2COLLISION_GROUP_LAST_NPC,
	HL2COLLISION_GROUP_COMBINE_BALL_NPC,
};


//--------------
// HL2 SPECIFIC
//--------------
#define DMG_SNIPER			(DMG_LASTGENERICFLAG<<1)	// This is sniper damage
#define DMG_MISSILEDEFENSE	(DMG_LASTGENERICFLAG<<2)	// The only kind of damage missiles take. (special missile defense)

//FR COLORS
#define COLOR_FR_OLD	Color(84, 84, 255, 255)
#define COLOR_FR		Color(0, 120, 255, 255)

//FR FUN COLORS
#define COLOR_PURPLE	Color(177, 110, 245, 255)
#define COLOR_ORANGE	Color(245, 139, 73, 255)
#define COLOR_CYAN		Color(73, 216, 245, 255)
#define COLOR_TURQUOISE	Color(56, 245, 210, 255)
#define COLOR_PINK		Color(237, 142, 220, 255)
#define COLOR_MAGENTA	Color(255, 0, 255, 255)

//FR NPC TEAMS. Player still gets TEAM_UNASSIGNED. Used for death notices.
#define TEAM_RED		2
#define TEAM_BLUE		3
#define TEAM_YELLOW		4
#define TEAM_GREEN		5
#define TEAM_GREY		6
#define TEAM_WHITE		7
#define TEAM_BLACK		8
#define TEAM_PURPLE		9
#define TEAM_ORANGE		10
#define TEAM_CYAN		11
#define TEAM_TURQUOISE	12
#define TEAM_PINK		13
#define TEAM_MAGENTA	14

#endif // HL2_SHAREDDEFS_H
