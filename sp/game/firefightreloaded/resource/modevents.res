//=========== (C) Copyright 1999 Valve, L.L.C. All rights reserved. ===========
//
// The copyright to the contents herein is the property of Valve, L.L.C.
// The contents may be used and/or copied only with the written permission of
// Valve, L.L.C., or in accordance with the terms and conditions stipulated in
// the agreement/contract under which the contents have been supplied.
//=============================================================================

// No spaces in event names, max length 32
// All strings are case sensitive
//
// valid data key types are:
//   string : a zero terminated string
//   bool   : unsigned int, 1 bit
//   byte   : unsigned int, 8 bit
//   short  : signed int, 16 bit
//   long   : signed int, 32 bit
//   float  : float, 32 bit
//   local  : any data, but not networked to clients
//
// following key names are reserved:
//   local      : if set to 1, event is not networked to clients
//   unreliable : networked, but unreliable
//   suppress   : never fire this event
//   time	: firing server time
//   eventid	: holds the event ID

"modevents"
{
	"player_maxlevel"
	{
		"userid"	"short"			
	}
    
    "player_levelup"
	{
		"userid"	"short"	
        "level"	    "short"
	}
    
    "player_hurt"
	{
		"userid"	"short"   	// player index who was hurt				
		"attacker"	"short"	 	// player index who attacked
		"health"	"byte"		// remaining health points
        "damageamount"	"long"
	}
    
    "npc_hurt"
	{
		"entindex"	"short"   	// player index who was hurt				
		"attacker_player"	"short"	 	// player index who attacked
		"health"	"long"		// remaining health points
        "damageamount"	"long"
        "damageamount_consecutive"	"long"
        "timeshit"	"long"
        "alive"	    "bool"
	}
	
	"player_death"				// a game event, name may be 32 charaters long
	{
		"userid"	"short"   	// user ID who died				
		"attacker"	"short"	 	// user ID who killed
		"weapon"	"string" 	// weapon name killed used 
	}
	
	"npc_death"				// a game event, name may be 32 charaters long
	{
		"victimname" "string"   	// user ID who died			
		"attacker"	"short"	 	// user ID who killed
		"weapon"	"string" 	// weapon name killed used 
        "xpreward"	"short"
		"moneyreward"	"short"
        "victim_team"	"short"
	}
	
	"player_death_npc"				// a game event, name may be 32 charaters long
	{
		"userid"	"short"   	// user ID who died				
		"attacker"	"string"	 	// user ID who killed
		"weapon"	"string" 	// weapon name killed used 
        "attacker_team"	"short"
	}
    
    "npc_death_npc"				// a game event, name may be 32 charaters long
	{
		"victimname" "string"   // user ID who died				
		"attacker"	"string"	 // user ID who killed
		"weapon"	"string" 	// weapon name killed used 
        "attacker_team"	"short"
		"victim_team"	"short"
	}
    
    "achievement_event"
	{
		"achievement_name"	"string"	// non-localized name of achievement
		"cur_val"		"short"		// # of steps toward achievement
		"max_val"		"short"		// total # of steps in achievement
        "beta"	        "bool"
	}
	
	"npc_spawner_created"
	{
		"classname"	"string"
		"entindex"	"short"
	}
	
	"npc_spawner_killed"
	{
		"classname"	"string"
		"entindex"	"short"
	}
	
	"ds_stop"
	{}
}
