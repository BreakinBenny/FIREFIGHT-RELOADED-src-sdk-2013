//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

#ifndef WEAPON_CROSSBOW_H
#define WEAPON_CROSSBOW_H

#include "basecombatcharacter.h"
#include "Sprite.h"

#if defined( _WIN32 )
#pragma once
#endif

//#define BOLT_MODEL			"models/crossbow_bolt.mdl"
//#define BOLT_MODEL	"models/weapons/w_missile_closed.mdl"

#define BOLT_AIR_VELOCITY	2500
#define BOLT_WATER_VELOCITY	1500

#define	BOLT_SKIN_NORMAL	0
#define BOLT_SKIN_GLOW		1

#define	CROSSBOW_GLOW_SPRITE	"sprites/light_glow02_noz.vmt"
#define	CROSSBOW_GLOW_SPRITE2	"sprites/blueflare1.vmt"

#define	BOLT_TIP_ATTACHMENT	2

// Various states for the crossbow's charger
enum ChargerState_t
{
	CHARGER_STATE_START_LOAD,
	CHARGER_STATE_START_CHARGE,
	CHARGER_STATE_READY,
	CHARGER_STATE_DISCHARGE,
	CHARGER_STATE_OFF,
};

//Knives have their own entities!!
enum BoltTypes
{
	BOLT_CROSSBOW,
	BOLT_CHARGEBOW
};

//-----------------------------------------------------------------------------
// Crossbow Bolt
//-----------------------------------------------------------------------------
class CCrossbowBolt : public CBaseCombatCharacter
{
	DECLARE_CLASS( CCrossbowBolt, CBaseCombatCharacter );

public:
	CCrossbowBolt() { };
	~CCrossbowBolt();

	Class_T Classify( void ) { return CLASS_NONE; }

public:
	void Spawn( void );
	void Precache( void );
	void BubbleThink( void );
	void BoltTouch( CBaseEntity *pOther );
	bool CreateVPhysics( void );
	unsigned int PhysicsSolidMaskForEntity() const;
	static CCrossbowBolt *BoltCreate(const Vector& vecOrigin, const QAngle& angAngles, CBasePlayer* pentOwner, int iBoltType = BOLT_CROSSBOW, bool bCharged = false);
	bool IsCharged() { return (m_iBoltType == BOLT_CHARGEBOW && m_bCharged); }
	bool IsUncharged() { return (m_iBoltType == BOLT_CHARGEBOW && !m_bCharged); }
	void GlassCollide(CBaseEntity* pOther);

public:

	int		m_iBoltType;
	bool	m_bStopPenetrating;
	bool	m_bCharged;
	int		m_iDamage;
	bool	m_bBoltPinnedEnemy;
	bool	m_bBoltHitFlyingEnemy;

protected:

	bool	CreateSprites( void );

	CHandle<CSprite>		m_pGlowSprite;
	//CHandle<CSpriteTrail>	m_pGlowTrail;

	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();
};

//-----------------------------------------------------------------------------
// Crossbow Bolt
//-----------------------------------------------------------------------------
class CKnifeBolt : public CBaseCombatCharacter
{
	DECLARE_CLASS(CKnifeBolt, CBaseCombatCharacter);

public:
	~CKnifeBolt();

	Class_T Classify(void) { return CLASS_NONE; }

public:
	void Spawn(void);
	void Precache(void);
	void BubbleThink(void);
	void BoltTouch(CBaseEntity* pOther);
	bool CreateVPhysics(void);
	unsigned int PhysicsSolidMaskForEntity() const;
	static CKnifeBolt* BoltCreate(const Vector& vecOrigin, const QAngle& angAngles, CBasePlayer* pentOwner = NULL);

public:
	bool	m_bBoltPinnedEnemy;
	bool	m_bBoltHitFlyingEnemy;

protected:

	DECLARE_DATADESC();
};

#endif // WEAPON_ALYXGUN_H
