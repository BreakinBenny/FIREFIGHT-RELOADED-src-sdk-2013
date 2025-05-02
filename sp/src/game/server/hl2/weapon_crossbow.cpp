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
#include "weapon_knife.h"
#include "cleanup_manager.h"

#ifdef PORTAL
	#include "portal_util_shared.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar sk_plr_dmg_crossbow;
extern ConVar sk_npc_dmg_crossbow;

ConVar	  crossbow_new_glass_passthrough("crossbow_new_glass_passthrough", "1", FCVAR_ARCHIVE);

ConVar	  chargebow_uncharged_damage_reduction("chargebow_uncharged_damage_reduction", "0.5", FCVAR_ARCHIVE);
ConVar	  chargebow_damage_falloff_amount("chargebow_damage_falloff_amount", "0.25", FCVAR_ARCHIVE);

ConVar	  bolt_debug_print_damage("bolt_debug_print_damage", "0", FCVAR_NONE);

LINK_ENTITY_TO_CLASS( crossbow_bolt, CCrossbowBolt );
LINK_ENTITY_TO_CLASS( chargebow_bolt, CCrossbowBolt);

BEGIN_DATADESC( CCrossbowBolt )
	// Function Pointers
	DEFINE_FUNCTION( BubbleThink ),
	DEFINE_FUNCTION( BoltTouch ),

	// These are recreated on reload, they don't need storage
	DEFINE_FIELD( m_pGlowSprite, FIELD_EHANDLE ),
	DEFINE_FIELD( m_iBoltType, FIELD_INTEGER ),
	DEFINE_FIELD(m_bStopPenetrating, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bCharged, FIELD_BOOLEAN ),
	DEFINE_FIELD(m_iDamage, FIELD_INTEGER),
	//DEFINE_FIELD( m_pGlowTrail, FIELD_EHANDLE ),

END_DATADESC()

IMPLEMENT_SERVERCLASS_ST( CCrossbowBolt, DT_CrossbowBolt )
END_SEND_TABLE()

CCrossbowBolt *CCrossbowBolt::BoltCreate( const Vector &vecOrigin, const QAngle &angAngles, CBasePlayer *pentOwner, int iBoltType, bool bCharged)
{
	// Create a new entity with CCrossbowBolt private data

	const char* szBoltClassname = "crossbow_bolt";

	if (iBoltType == BOLT_CHARGEBOW)
	{
		szBoltClassname = "chargebow_bolt";
	}

	CCrossbowBolt *pBolt = (CCrossbowBolt *)CreateEntityByName(szBoltClassname);
	if (pBolt)
	{
		UTIL_SetOrigin(pBolt, vecOrigin);
		pBolt->SetAbsAngles(angAngles);
		pBolt->m_iBoltType = iBoltType;
		pBolt->m_bCharged = bCharged;
		pBolt->Spawn();
		pBolt->SetOwnerEntity(pentOwner);
	}

	return pBolt;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CCrossbowBolt::~CCrossbowBolt( void )
{
	if ( m_pGlowSprite )
	{
		UTIL_Remove( m_pGlowSprite );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CCrossbowBolt::CreateVPhysics( void )
{
	// Create the object in the physics system
	VPhysicsInitNormal( SOLID_BBOX, FSOLID_NOT_STANDABLE, false );

	return true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
unsigned int CCrossbowBolt::PhysicsSolidMaskForEntity() const
{
	return ( BaseClass::PhysicsSolidMaskForEntity() | CONTENTS_HITBOX ) & ~CONTENTS_GRATE;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CCrossbowBolt::CreateSprites( void )
{
	// Start up the eye glow
	m_pGlowSprite = CSprite::SpriteCreate( "sprites/light_glow02_noz.vmt", GetLocalOrigin(), false );

	if ( m_pGlowSprite != NULL )
	{
		m_pGlowSprite->FollowEntity( this );
		m_pGlowSprite->SetTransparency( kRenderGlow, 255, 255, 255, 128, kRenderFxNoDissipation );
		m_pGlowSprite->SetScale( 0.2f );
		m_pGlowSprite->TurnOff();
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CCrossbowBolt::Spawn( void )
{
	Precache( );

	if (m_iBoltType == BOLT_CHARGEBOW)
	{
		SetModel("models/weapons/chagebow_bolt.mdl");
	}
	else
	{
		SetModel("models/crossbow_bolt.mdl");
	}

	SetMoveType( MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_CUSTOM );
	UTIL_SetSize( this, -Vector(0.3f,0.3f,0.3f), Vector(0.3f,0.3f,0.3f) );
	SetSolid( SOLID_BBOX );
	SetGravity( 0.05f );

	//so we don't call !IsCharged() and check for m_iBoltType == BOLT_CHARGEBOW again
	if (IsUncharged())
	{
		m_iDamage = sk_plr_dmg_crossbow.GetFloat() * chargebow_uncharged_damage_reduction.GetFloat();
	}
	else
	{
		m_iDamage = sk_plr_dmg_crossbow.GetFloat();
	}
	
	// Make sure we're updated if we're underwater
	UpdateWaterState();

	SetTouch( &CCrossbowBolt::BoltTouch );

	SetThink( &CCrossbowBolt::BubbleThink );
	SetNextThink( gpGlobals->curtime + 0.1f );
	
	CreateSprites();

	// Make us glow until we've hit the wall
	m_nSkin = BOLT_SKIN_GLOW;
	m_bStopPenetrating = false;
}


void CCrossbowBolt::Precache( void )
{
	PrecacheModel("models/weapons/chagebow_bolt.mdl");
	PrecacheModel("models/crossbow_bolt.mdl");
	PrecacheModel( "sprites/light_glow02_noz.vmt" );
}

void CCrossbowBolt::GlassCollide(CBaseEntity* pOther)
{
	trace_t tr;
	tr = CBaseEntity::GetTouchTrace();
	ClearMultiDamage();
	Vector forward;
	AngleVectors(GetLocalAngles(), &forward, NULL, NULL);
	CTakeDamageInfo info(this, GetOwnerEntity(), 1, DMG_CLUB);
	CalculateMeleeDamageForce(&info, GetAbsVelocity(), GetAbsOrigin());
	pOther->DispatchTraceAttack(info, forward, &tr);
	ApplyMultiDamage();
	SetAbsVelocity(GetAbsVelocity());
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CCrossbowBolt::BoltTouch( CBaseEntity *pOther )
{
	if ( pOther->IsSolidFlagSet(FSOLID_VOLUME_CONTENTS | FSOLID_TRIGGER) )
	{
		// Some NPCs are triggers that can take damage (like antlion grubs). We should hit them.
		if ( (pOther->m_takedamage == DAMAGE_NO) || (pOther->m_takedamage == DAMAGE_EVENTS_ONLY) )
		{
			return;
		}
	}

	if (FClassnameIs(pOther, "func_breakable"))
	{
		if (crossbow_new_glass_passthrough.GetBool())
		{
			CBreakable* pOtherEntity = static_cast<CBreakable*>(pOther);
			if (pOtherEntity && (pOtherEntity->GetMaterialType() == matGlass || pOtherEntity->GetMaterialType() == matWeb))
			{
				GlassCollide(pOther);
				return;
			}
		}
	}
	else if (FClassnameIs(pOther, "func_breakable_surf"))
	{
		if (crossbow_new_glass_passthrough.GetBool())
		{
			CBreakableSurface* pOtherEntity = static_cast<CBreakableSurface*>(pOther);
			if (pOtherEntity && (pOtherEntity->GetMaterialType() == matGlass || pOtherEntity->GetMaterialType() == matWeb))
			{
				GlassCollide(pOther);
				return;
			}
		}
	}
	else if (FClassnameIs(pOther, "prop_door_rotating") ||
		FClassnameIs(pOther, "func_door") ||
		FClassnameIs(pOther, "func_door_rotating") ||
		FClassnameIs(pOther, "func_movelinear") ||
		FClassnameIs(pOther, "func_train") ||
		FClassnameIs(pOther, "func_tanktrain") ||
		FClassnameIs(pOther, "func_conveyor") ||
		FClassnameIs(pOther, "func_brush") ||
		FClassnameIs(pOther, "func_tracktrain") &&
		m_iBoltType != BOLT_CHARGEBOW)
	{
		trace_t tr;
		tr = BaseClass::GetTouchTrace();

		Vector vecDir = GetAbsVelocity();
		float speed = VectorNormalize(vecDir);

		float hitDot = DotProduct(tr.plane.normal, -vecDir);
		Vector vReflection = 2.0f * tr.plane.normal * hitDot + vecDir;

		QAngle reflectAngles;
		VectorAngles(vReflection, reflectAngles);

		SetLocalAngles(reflectAngles);
		SetAbsVelocity(vReflection * speed * 0.75f);

		// Shoot some sparks
		if (UTIL_PointContents(GetAbsOrigin()) != CONTENTS_WATER)
		{
			g_pEffects->Sparks(GetAbsOrigin());
		}

		EmitSound("Weapon_Crossbow.BoltBounce");

		return;
	}

	if ( pOther->m_takedamage != DAMAGE_NO )
	{
		if (bolt_debug_print_damage.GetBool())
		{
			Msg("%i, (%f,%f,%f)\n", m_iDamage, GetAbsVelocity().x, GetAbsVelocity().y, GetAbsVelocity().z);
		}

		trace_t	tr, tr2;
		tr = BaseClass::GetTouchTrace();
		Vector	vecNormalizedVel = GetAbsVelocity();

		ClearMultiDamage();
		VectorNormalize( vecNormalizedVel );

		if( GetOwnerEntity() && GetOwnerEntity()->IsPlayer() && pOther->IsNPC() )
		{
			CTakeDamageInfo	dmgInfo( this, GetOwnerEntity(), m_iDamage, DMG_NEVERGIB );
			dmgInfo.AdjustPlayerDamageInflictedForSkillLevel();
			CalculateMeleeDamageForce( &dmgInfo, vecNormalizedVel, tr.endpos, 0.7f );
			dmgInfo.SetDamagePosition( tr.endpos );
			pOther->DispatchTraceAttack( dmgInfo, vecNormalizedVel, &tr );

			CBasePlayer *pPlayer = ToBasePlayer( GetOwnerEntity() );
			if ( pPlayer )
				gamestats->Event_WeaponHit(pPlayer, true, "weapon_crossbow", dmgInfo);
		}
		else
		{
			CTakeDamageInfo	dmgInfo( this, GetOwnerEntity(), m_iDamage, DMG_BULLET | DMG_NEVERGIB );
			CalculateMeleeDamageForce( &dmgInfo, vecNormalizedVel, tr.endpos, 0.7f );
			dmgInfo.SetDamagePosition( tr.endpos );
			pOther->DispatchTraceAttack( dmgInfo, vecNormalizedVel, &tr );
		}

		ApplyMultiDamage();

		//Adrian: keep going through the glass.
		if ( pOther->GetCollisionGroup() == COLLISION_GROUP_BREAKABLE_GLASS )
			 return;

		if ( !pOther->IsAlive() )
		{
			// We killed it! 
			const surfacedata_t *pdata = physprops->GetSurfaceData( tr.surface.surfaceProps );
			if ( pdata->game.material == CHAR_TEX_GLASS )
			{
				return;
			}
		}

		bool bShotPenetrate = false;

		if (IsCharged() && !m_bStopPenetrating)
		{
			bShotPenetrate = true;
		}

		if (!bShotPenetrate)
		{
			SetAbsVelocity(Vector(0, 0, 0));

			// play body "thwack" sound
			EmitSound("Weapon_Crossbow.BoltHitBody");

			Vector vForward;

			AngleVectors(GetAbsAngles(), &vForward);
			VectorNormalize(vForward);

			UTIL_TraceLine(GetAbsOrigin(), GetAbsOrigin() + vForward * 128, MASK_BLOCKLOS, pOther, COLLISION_GROUP_NONE, &tr2);

			if (tr2.fraction != 1.0f)
			{
				//			NDebugOverlay::Box( tr2.endpos, Vector( -16, -16, -16 ), Vector( 16, 16, 16 ), 0, 255, 0, 0, 10 );
				//			NDebugOverlay::Box( GetAbsOrigin(), Vector( -16, -16, -16 ), Vector( 16, 16, 16 ), 0, 0, 255, 0, 10 );

				if (tr2.m_pEnt == NULL || (tr2.m_pEnt && tr2.m_pEnt->GetMoveType() == MOVETYPE_NONE))
				{
					CEffectData	data;

					data.m_vOrigin = tr2.endpos;
					data.m_vNormal = vForward;
					if (m_iBoltType == BOLT_CHARGEBOW)
					{
						data.m_fFlags = SBFL_CHARGEBOWARROW | SBFL_STICKRAGDOLL;
						DispatchEffect("ArrowImpact", data);
					}
					else
					{
						data.m_fFlags = SBFL_CROSSBOWBOLT | SBFL_STICKRAGDOLL;
						DispatchEffect("BoltImpact", data);
					}

					UTIL_ImpactTrace(&tr2, DMG_BULLET);
				}
			}
		
			SetTouch(NULL);
			SetThink(NULL);

			UTIL_Remove(this);
		}
		else
		{
			SetAbsVelocity(GetAbsVelocity() - (GetAbsVelocity() * chargebow_damage_falloff_amount.GetFloat()));

			// play body "thwack" sound
			EmitSound("Weapon_Crossbow.BoltHitBody");

			m_iDamage = m_iDamage - (m_iDamage * chargebow_damage_falloff_amount.GetFloat());

			if (m_iDamage <= 0 || GetAbsVelocity() == Vector(0, 0, 0))
			{
				//end penetration NOW.
				m_bStopPenetrating = true;
			}
		}
	}
	else
	{
		trace_t	tr;
		tr = BaseClass::GetTouchTrace();

		// See if we struck the world
		if ( pOther->GetMoveType() == MOVETYPE_NONE && !( tr.surface.flags & SURF_SKY ) )
		{
			EmitSound( "Weapon_Crossbow.BoltHitWorld" );

			//fix bolts colliding with each other
			if (FClassnameIs(pOther, "crossbow_bolt"))
			{
				SetTouch(NULL);
				SetThink(&CCrossbowBolt::SUB_Remove);
				//NOW.
				SetNextThink(gpGlobals->curtime);
				UTIL_ImpactTrace(&tr, DMG_BULLET);
				// Shoot some sparks
				if (UTIL_PointContents(GetAbsOrigin()) != CONTENTS_WATER)
				{
					g_pEffects->Sparks(GetAbsOrigin());
				}
				return;
			}

			// if what we hit is static architecture, can stay around for a while.
			Vector vecDir = GetAbsVelocity();
			float speed = VectorNormalize( vecDir );

			// See if we should reflect off this surface
			float hitDot = DotProduct( tr.plane.normal, -vecDir );
			
			//don't deflect if we're using the chargebow
			if ( ( hitDot < 0.5f ) && ( speed > 100 ) && m_iBoltType != BOLT_CHARGEBOW)
			{
				Vector vReflection = 2.0f * tr.plane.normal * hitDot + vecDir;
				
				QAngle reflectAngles;

				VectorAngles( vReflection, reflectAngles );

				SetLocalAngles( reflectAngles );

				SetAbsVelocity( vReflection * speed * 0.75f );

				// Start to sink faster
				SetGravity( 1.0f );
			}
			else
			{
				//FIXME: We actually want to stick (with hierarchy) to what we've hit
				SetMoveType( MOVETYPE_NONE );

				m_bStopPenetrating = true;
			
				Vector vForward;

				AngleVectors( GetAbsAngles(), &vForward );
				VectorNormalize ( vForward );

				CEffectData	data;

				data.m_vOrigin = tr.endpos;
				data.m_vNormal = vForward;

				if (m_iBoltType == BOLT_CHARGEBOW)
				{
					data.m_fFlags = SBFL_CHARGEBOWARROW;
					DispatchEffect("ArrowImpact", data);
				}
				else
				{
					data.m_fFlags = SBFL_CROSSBOWBOLT;
					DispatchEffect("BoltImpact", data);
				}
				
				UTIL_ImpactTrace( &tr, DMG_BULLET );

				AddEffects( EF_NODRAW );
				SetTouch( NULL );
				SetThink( &CCrossbowBolt::SUB_Remove );
				SetNextThink(gpGlobals->curtime);

				if ( m_pGlowSprite != NULL )
				{
					m_pGlowSprite->TurnOn();
					m_pGlowSprite->FadeAndDie( 3.0f );
				}
			}
			
			// Shoot some sparks
			if ( UTIL_PointContents( GetAbsOrigin() ) != CONTENTS_WATER)
			{
				g_pEffects->Sparks( GetAbsOrigin() );
			}
		}
		else
		{
			m_bStopPenetrating = true;

			// Put a mark unless we've hit the sky
			if ( ( tr.surface.flags & SURF_SKY ) == false )
			{
				UTIL_ImpactTrace( &tr, DMG_BULLET );
			}

			UTIL_Remove( this );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CCrossbowBolt::BubbleThink( void )
{
	QAngle angNewAngles;

	VectorAngles( GetAbsVelocity(), angNewAngles );
	SetAbsAngles( angNewAngles );

	SetNextThink( gpGlobals->curtime + 0.1f );

	// Make danger sounds out in front of me, to scare snipers back into their hole
	CSoundEnt::InsertSound( SOUND_DANGER_SNIPERONLY, GetAbsOrigin() + GetAbsVelocity() * 0.2, 120.0f, 0.5f, this, SOUNDENT_CHANNEL_REPEATED_DANGER );

	if ( GetWaterLevel()  == 0 )
		return;

	UTIL_BubbleTrail( GetAbsOrigin() - GetAbsVelocity() * 0.1f, GetAbsOrigin(), 5 );
}

//knife bolt

extern ConVar sk_plr_dmg_knife_thrown;
extern ConVar sk_npc_dmg_knife_thrown;

LINK_ENTITY_TO_CLASS(knife_bolt, CKnifeBolt);

BEGIN_DATADESC(CKnifeBolt)
// Function Pointers
DEFINE_THINKFUNC(BubbleThink),
DEFINE_ENTITYFUNC(BoltTouch),
END_DATADESC()

CKnifeBolt* CKnifeBolt::BoltCreate(const Vector& vecOrigin, const QAngle& angAngles, CBasePlayer* pentOwner)
{
	// Create a new entity with CKnifeBolt private data
	CKnifeBolt* pBolt = (CKnifeBolt*)CreateEntityByName("knife_bolt");
	UTIL_SetOrigin(pBolt, vecOrigin);
	pBolt->SetAbsAngles(angAngles);
	pBolt->Spawn();
	pBolt->SetOwnerEntity(pentOwner);

	return pBolt;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CKnifeBolt::~CKnifeBolt(void)
{
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CKnifeBolt::CreateVPhysics(void)
{
	// Create the object in the physics system
	VPhysicsInitNormal(SOLID_BBOX, FSOLID_NOT_STANDABLE, false);

	return true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
unsigned int CKnifeBolt::PhysicsSolidMaskForEntity() const
{
	return MASK_PLAYERSOLID;

}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CKnifeBolt::Spawn(void)
{
	Precache();

	SetModel("models/knife_proj.mdl");

	SetMoveType(MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_CUSTOM);
	UTIL_SetSize(this, -Vector(0.3f, 0.3f, 0.3f), Vector(0.3f, 0.3f, 0.3f));
	SetSolid(SOLID_BBOX);
	SetGravity(0.05f);

	// Make sure we're updated if we're underwater
	UpdateWaterState();

	SetTouch(&CKnifeBolt::BoltTouch);

	SetThink(&CKnifeBolt::BubbleThink);
	SetNextThink(gpGlobals->curtime + 0.1f);
}

void CKnifeBolt::Precache(void)
{
	PrecacheModel("models/knife_proj.mdl");
	PrecacheModel("sprites/light_glow02_noz.vmt");
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CKnifeBolt::BoltTouch(CBaseEntity* pOther)
{
	CEffectData	data;
	bool dispatchEffect = false;
	bool doneMoving = false;
	CBaseAnimating* ragdoll = nullptr;
	bool stuck = false;

	if (pOther->IsSolidFlagSet(FSOLID_VOLUME_CONTENTS | FSOLID_TRIGGER) && !pOther->IsSolidFlagSet(FSOLID_USE_TRIGGER_BOUNDS))
	{
		// Some NPCs are triggers that can take damage (like antlion grubs). We should hit them.
		if ((pOther->m_takedamage == DAMAGE_NO || pOther->m_takedamage == DAMAGE_EVENTS_ONLY))
			return;
	}

	if (pOther->m_takedamage != DAMAGE_NO)
	{
		trace_t	tr, tr2;
		tr = BaseClass::GetTouchTrace();
		Vector vecNormalizedVel = GetAbsVelocity();

		ClearMultiDamage();
		VectorNormalize(vecNormalizedVel);

		float curDamage = sk_plr_dmg_knife_thrown.GetFloat();
		const surfacedata_t* pdata = physprops->GetSurfaceData(tr.surface.surfaceProps);

		if (GetOwnerEntity() && GetOwnerEntity()->IsPlayer() && pOther->IsNPC())
		{
			CTakeDamageInfo	dmgInfo(this, GetOwnerEntity(), curDamage, DMG_NEVERGIB);
			dmgInfo.AdjustPlayerDamageInflictedForSkillLevel();
			CalculateMeleeDamageForce(&dmgInfo, vecNormalizedVel, tr.endpos, 0.7f);
			dmgInfo.SetDamagePosition(tr.endpos);
			doneMoving = true;
			pOther->DispatchTraceAttack(dmgInfo, vecNormalizedVel, &tr);

			CBasePlayer* pPlayer = ToBasePlayer(GetOwnerEntity());
			if (pPlayer)
				gamestats->Event_WeaponHit(pPlayer, true, "weapon_knife", dmgInfo);
		}
		else if (FClassnameIs(pOther, "func_breakable") || FClassnameIs(pOther, "func_breakable_surf"))
		{
			CTakeDamageInfo	dmgInfo(this, GetOwnerEntity(), curDamage, DMG_BULLET | DMG_NEVERGIB);
			CalculateMeleeDamageForce(&dmgInfo, vecNormalizedVel, tr.endpos, 0.7f);
			dmgInfo.SetDamagePosition(tr.endpos);
			pOther->DispatchTraceAttack(dmgInfo, vecNormalizedVel, &tr);

			CBreakable* pOtherEntity = static_cast<CBreakable*>(pOther);
			if (pOtherEntity && (pOtherEntity->GetMaterialType() == matGlass || pOtherEntity->GetMaterialType() == matWeb))
				return;
		}
		else
		{
			CTakeDamageInfo	dmgInfo(this, GetOwnerEntity(), curDamage, DMG_BULLET | DMG_NEVERGIB);
			CalculateMeleeDamageForce(&dmgInfo, vecNormalizedVel, tr.endpos, 0.7f);
			dmgInfo.SetDamagePosition(tr.endpos);

			if (pOther->GetCollisionGroup() == COLLISION_GROUP_BREAKABLE_GLASS)
			{
				pOther->DispatchTraceAttack(dmgInfo, vecNormalizedVel, &tr);
				return;
			}
			else if (FClassnameIs(pOther, "func_breakable"))
			{
				pOther->DispatchTraceAttack(dmgInfo, vecNormalizedVel, &tr);

				CBreakable* pOtherEntity = static_cast<CBreakable*>(pOther);
				if (pOtherEntity && (pOtherEntity->GetMaterialType() == matGlass || pOtherEntity->GetMaterialType() == matWeb))
					return;
			}
			else if (FClassnameIs(pOther, "func_breakable_surf"))
			{
				pOther->DispatchTraceAttack(dmgInfo, vecNormalizedVel, &tr);

				CBreakableSurface* pOtherEntity = static_cast<CBreakableSurface*>(pOther);
				if (pOtherEntity && (pOtherEntity->GetMaterialType() == matGlass || pOtherEntity->GetMaterialType() == matWeb))
					return;
			}
			else if (pdata->game.material != CHAR_TEX_GLASS)
				doneMoving = true;
			pOther->DispatchTraceAttack(dmgInfo, vecNormalizedVel, &tr);
		}

		ApplyMultiDamage();

		if (!pOther->IsAlive() && pdata->game.material == CHAR_TEX_GLASS)
			return;

		SetAbsVelocity(Vector(0, 0, 0));

		// play body "thwack" sound 
		EmitSound("Weapon_Crossbow.BoltHitBody");

		Vector vForward;

		AngleVectors(GetAbsAngles(), &vForward);
		VectorNormalize(vForward);

		UTIL_TraceLine(GetAbsOrigin(), GetAbsOrigin() + vForward * 128, MASK_BLOCKLOS, pOther, COLLISION_GROUP_NONE, &tr2);

		if (tr2.fraction != 1.0f)
		{
			//			NDebugOverlay::Box( tr2.endpos, Vector( -16, -16, -16 ), Vector( 16, 16, 16 ), 0, 255, 0, 0, 10 );
			//			NDebugOverlay::Box( GetAbsOrigin(), Vector( -16, -16, -16 ), Vector( 16, 16, 16 ), 0, 0, 255, 0, 10 );

			if (tr2.m_pEnt == NULL || (tr2.m_pEnt && tr2.m_pEnt->GetMoveType() == MOVETYPE_NONE))
			{
				//CEffectData	data;

				data.m_vOrigin = tr2.endpos;
				data.m_vNormal = vForward;
				data.m_fFlags = SBFL_STICKRAGDOLL;

				dispatchEffect = true;
				doneMoving = true;

				auto anim = dynamic_cast<CBaseAnimating*>(pOther);
				if (anim != nullptr && anim->CanBecomeRagdoll(true))
				{
					ragdoll = anim;
					stuck = true;
					UTIL_ImpactTrace(&tr2, DMG_BULLET);
					SetAbsOrigin(tr2.endpos);
				}
			}
		}
	}
	else
	{
		trace_t	tr;
		tr = BaseClass::GetTouchTrace();

		// See if we struck the world
		if (pOther->GetMoveType() == MOVETYPE_NONE && !(tr.surface.flags & SURF_SKY) && !(tr.contents & CONTENTS_PLAYERCLIP))
		{
			EmitSound("Weapon_Crossbow.BoltHitWorld");

			// if what we hit is static architecture, can stay around for a while.
			Vector vecDir = GetAbsVelocity();
			float speed = VectorNormalize(vecDir);

			// See if we should reflect off this surface
			float hitDot = DotProduct(tr.plane.normal, -vecDir);

			if ((hitDot < 0.5f) && (speed > 100))
			{
				Vector vReflection = 2.0f * tr.plane.normal * hitDot + vecDir;

				QAngle reflectAngles;

				VectorAngles(vReflection, reflectAngles);

				SetLocalAngles(reflectAngles);

				SetAbsVelocity(vReflection * speed * 0.75f);

				// Start to sink faster
				SetGravity(1.0f);
			}
			else
			{
				//FIXME: We actually want to stick (with hierarchy) to what we've hit
				SetMoveType(MOVETYPE_NONE);

				Vector vForward;

				AngleVectors(GetAbsAngles(), &vForward);
				VectorNormalize(vForward);

				UTIL_ImpactTrace(&tr, DMG_BULLET);

				doneMoving = true;
				stuck = true;
			}

			// Shoot some sparks
			if (UTIL_PointContents(GetAbsOrigin()) != CONTENTS_WATER)
			{
				g_pEffects->Sparks(GetAbsOrigin());
			}
		}
		else if (tr.surface.flags & SURF_SKY || tr.contents & CONTENTS_PLAYERCLIP)
			doneMoving = true;
		else
		{
			UTIL_ImpactTrace(&tr, DMG_BULLET);
			doneMoving = true;
		}
	}

	if (doneMoving)
	{
		QAngle angle = GetAbsAngles();
		// The weapon model is reversed for some reason.
		angle[0] += 180;
		CWeaponKnife* pWeap = (CWeaponKnife*)CBaseEntity::CreateNoSpawn("weapon_knife", GetAbsOrigin(), angle);
		pWeap->AddSpawnFlags(SF_NORESPAWN);
		pWeap->m_hStuckRagdoll = ragdoll;
		DispatchSpawn(pWeap);

		if (dispatchEffect)
		{
			data.m_nEntIndex = pWeap->entindex();
			DispatchEffect("BoltImpact", data);
		}

		IPhysicsObject* phys = pWeap->VPhysicsGetObject();
		if (stuck && phys != nullptr)
		{
			phys->EnableMotion(false);
			//phys->EnableGravity(false);
			phys->Sleep();
			//pWeap->SetCollisionGroup( COLLISION_GROUP_DEBRIS );
		}
		pWeap->SetAbsVelocity(Vector(0, 0, 0));
		pWeap->AddEffects(EF_ITEM_BLINK);

		CCleanupManager::AddThrownKnife(pWeap);

		Remove();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CKnifeBolt::BubbleThink(void)
{
	QAngle angNewAngles;

	VectorAngles(GetAbsVelocity(), angNewAngles);
	SetAbsAngles(angNewAngles);

	SetNextThink(gpGlobals->curtime + 0.1f);

	// Make danger sounds out in front of me, to scare snipers back into their hole
	CSoundEnt::InsertSound(SOUND_DANGER_SNIPERONLY, GetAbsOrigin() + GetAbsVelocity() * 0.2, 120.0f, 0.5f, this, SOUNDENT_CHANNEL_REPEATED_DANGER);

	if (GetWaterLevel() == 0)
		return;

	UTIL_BubbleTrail(GetAbsOrigin() - GetAbsVelocity() * 0.1f, GetAbsOrigin(), 5);
}

//-----------------------------------------------------------------------------
// CWeaponCrossbow
//-----------------------------------------------------------------------------

class CWeaponCrossbow : public CBaseHLCombatWeapon
{
	DECLARE_CLASS( CWeaponCrossbow, CBaseHLCombatWeapon );
public:

	CWeaponCrossbow( void );
	
	virtual void	Precache( void );
	virtual void	PrimaryAttack( void );
	virtual void	SecondaryAttack( void );
	virtual bool	Deploy( void );
	virtual void	Drop( const Vector &vecVelocity );
	virtual bool	Holster( CBaseCombatWeapon *pSwitchingTo = NULL );
	virtual bool	Reload( void );
	virtual void	ItemPostFrame( void );
	virtual void	ItemBusyFrame( void );
	virtual void	Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );
	virtual bool	SendWeaponAnim( int iActivity );
	virtual bool	IsWeaponZoomed() { return m_bInZoom; }
	
	bool	ShouldDisplayHUDHint() { return true; }


	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();
	DECLARE_ACTTABLE();

private:
	
	void	StopEffects( void );
	void	SetSkin( int skinNum );
	void	CheckZoomToggle( void );
	void	FireBolt( void );
	void	ToggleZoom( void );

	void	CreateChargerEffects( void );
	void	SetChargerState( ChargerState_t state );
	void	DoLoadEffect( void );

private:
	
	// Charger effects
	ChargerState_t		m_nChargeState;
	CHandle<CSprite>	m_hChargerSprite;

	bool				m_bInZoom;
	bool				m_bMustReload;
};

acttable_t CWeaponCrossbow::m_acttable[] =
{
	{ ACT_HL2MP_IDLE, ACT_HL2MP_IDLE_CROSSBOW, false },
	{ ACT_HL2MP_RUN, ACT_HL2MP_RUN_CROSSBOW, false },
	{ ACT_HL2MP_IDLE_CROUCH, ACT_HL2MP_IDLE_CROUCH_CROSSBOW, false },
	{ ACT_HL2MP_WALK_CROUCH, ACT_HL2MP_WALK_CROUCH_CROSSBOW, false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK, ACT_HL2MP_GESTURE_RANGE_ATTACK_CROSSBOW, false },
	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_CROSSBOW,				false },
	{ ACT_HL2MP_JUMP, ACT_HL2MP_JUMP_CROSSBOW, false },
	{ ACT_RANGE_ATTACK1, ACT_RANGE_ATTACK_SHOTGUN, false },
};

IMPLEMENT_ACTTABLE(CWeaponCrossbow);

LINK_ENTITY_TO_CLASS( weapon_crossbow, CWeaponCrossbow );

PRECACHE_WEAPON_REGISTER( weapon_crossbow );

IMPLEMENT_SERVERCLASS_ST( CWeaponCrossbow, DT_WeaponCrossbow )
END_SEND_TABLE()

BEGIN_DATADESC( CWeaponCrossbow )

	DEFINE_FIELD( m_bInZoom,		FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bMustReload,	FIELD_BOOLEAN ),
	DEFINE_FIELD( m_nChargeState,	FIELD_INTEGER ),
	DEFINE_FIELD( m_hChargerSprite,	FIELD_EHANDLE ),

END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponCrossbow::CWeaponCrossbow( void )
{
	m_bReloadsSingly	= true;
	m_bFiresUnderwater	= true;
	m_bAltFiresUnderwater = true;
	m_bInZoom			= false;
	m_bMustReload		= false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponCrossbow::Precache( void )
{
	UTIL_PrecacheOther( "crossbow_bolt" );

	PrecacheScriptSound( "Weapon_Crossbow.BoltHitBody" );
	PrecacheScriptSound( "Weapon_Crossbow.BoltHitWorld" );
	PrecacheScriptSound( "Weapon_Crossbow.BoltSkewer" );

	PrecacheModel( CROSSBOW_GLOW_SPRITE );
	PrecacheModel( CROSSBOW_GLOW_SPRITE2 );

	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponCrossbow::PrimaryAttack( void )
{
	if ( m_bInZoom && g_pGameRules->IsMultiplayer() )
	{
//		FireSniperBolt();
		FireBolt();
	}
	else
	{
		FireBolt();
	}

	// Signal a reload
	m_bMustReload = true;

	SetWeaponIdleTime( gpGlobals->curtime + SequenceDuration( ACT_VM_PRIMARYATTACK ) );

	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if ( pPlayer )
	{
		m_iPrimaryAttacks++;
		gamestats->Event_WeaponFired( pPlayer, true, GetClassname() );
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponCrossbow::SecondaryAttack( void )
{
	//NOTENOTE: The zooming is handled by the post/busy frames
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CWeaponCrossbow::Reload( void )
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
void CWeaponCrossbow::CheckZoomToggle( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	
	if (!IsIronsighted())
	{
		if (pPlayer->m_afButtonPressed & IN_ATTACK2)
		{
			ToggleZoom();
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponCrossbow::ItemBusyFrame( void )
{
	// Allow zoom toggling even when we're reloading
	CheckZoomToggle();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponCrossbow::ItemPostFrame( void )
{
	// Allow zoom toggling
	CheckZoomToggle();

	if ( m_bMustReload && HasWeaponIdleTimeElapsed() )
	{
		Reload();
	}

	BaseClass::ItemPostFrame();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponCrossbow::FireBolt( void )
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

	pOwner->RumbleEffect( RUMBLE_357, 0, RUMBLE_FLAG_RESTART );

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

	CCrossbowBolt *pBolt = CCrossbowBolt::BoltCreate( vecSrc, angAiming, pOwner);

	if (pBolt)
	{

		if (pOwner->GetWaterLevel() == 3)
		{
			pBolt->SetAbsVelocity(vecAiming * BOLT_WATER_VELOCITY);
		}
		else
		{
			pBolt->SetAbsVelocity(vecAiming * BOLT_AIR_VELOCITY);
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
	SetChargerState( CHARGER_STATE_DISCHARGE );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CWeaponCrossbow::Deploy( void )
{
	if ( m_iClip1 <= 0 )
	{
		return DefaultDeploy( (char*)GetViewModel(), (char*)GetWorldModel(), ACT_CROSSBOW_DRAW_UNLOADED, (char*)GetAnimPrefix() );
	}

	SetSkin( BOLT_SKIN_GLOW );

	return BaseClass::Deploy();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pSwitchingTo - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CWeaponCrossbow::Holster( CBaseCombatWeapon *pSwitchingTo )
{
	StopEffects();
	return BaseClass::Holster( pSwitchingTo );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponCrossbow::ToggleZoom( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	
	if ( pPlayer == NULL )
		return;

	if (IsIronsighted())
		return;

	if ( m_bInZoom )
	{
		if ( pPlayer->SetFOV( this, 0, 0.2f ) )
		{
			m_bInZoom = false;
		}
	}
	else
	{
		if ( pPlayer->SetFOV( this, 20, 0.1f ) )
		{
			m_bInZoom = true;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponCrossbow::CreateChargerEffects( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );

	if ( m_hChargerSprite != NULL )
		return;

	m_hChargerSprite = CSprite::SpriteCreate( CROSSBOW_GLOW_SPRITE, GetAbsOrigin(), false );

	if ( m_hChargerSprite )
	{
		if ( pOwner != NULL )
		    m_hChargerSprite->SetAttachment( pOwner->GetViewModel(), BOLT_TIP_ATTACHMENT );
		m_hChargerSprite->SetTransparency( kRenderTransAdd, 255, 128, 0, 255, kRenderFxNoDissipation );
		m_hChargerSprite->SetBrightness( 0 );
		m_hChargerSprite->SetScale( 0.1f );
		m_hChargerSprite->TurnOff();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : skinNum - 
//-----------------------------------------------------------------------------
void CWeaponCrossbow::SetSkin( int skinNum )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	
	if ( pOwner == NULL )
		return;

	CBaseViewModel *pViewModel = pOwner->GetViewModel();

	if ( pViewModel == NULL )
		return;

	pViewModel->m_nSkin = skinNum;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponCrossbow::DoLoadEffect( void )
{
	SetSkin( BOLT_SKIN_GLOW );

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

	CSprite *pBlast = CSprite::SpriteCreate( CROSSBOW_GLOW_SPRITE2, GetAbsOrigin(), false );

	if ( pBlast )
	{
		pBlast->SetAttachment( pOwner->GetViewModel(), 1 );
		pBlast->SetTransparency( kRenderTransAdd, 255, 255, 255, 255, kRenderFxNone );
		pBlast->SetBrightness( 128 );
		pBlast->SetScale( 0.2f );
		pBlast->FadeOutFromSpawn();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : state - 
//-----------------------------------------------------------------------------
void CWeaponCrossbow::SetChargerState( ChargerState_t state )
{
	// Make sure we're setup
	CreateChargerEffects();

	// Don't do this twice
	if ( state == m_nChargeState )
		return;

	m_nChargeState = state;

	switch( m_nChargeState )
	{
	case CHARGER_STATE_START_LOAD:
	
		WeaponSound( SPECIAL1 );
		
		// Shoot some sparks and draw a beam between the two outer points
		DoLoadEffect();
		
		break;

	case CHARGER_STATE_START_CHARGE:
		{
			if ( m_hChargerSprite == NULL )
				break;
			
			m_hChargerSprite->SetBrightness( 32, 0.5f );
			m_hChargerSprite->SetScale( 0.025f, 0.5f );
			m_hChargerSprite->TurnOn();
		}

		break;

	case CHARGER_STATE_READY:
		{
			// Get fully charged
			if ( m_hChargerSprite == NULL )
				break;
			
			m_hChargerSprite->SetBrightness( 80, 1.0f );
			m_hChargerSprite->SetScale( 0.1f, 0.5f );
			m_hChargerSprite->TurnOn();
		}

		break;

	case CHARGER_STATE_DISCHARGE:
		{
			SetSkin( BOLT_SKIN_NORMAL );
			
			if ( m_hChargerSprite == NULL )
				break;
			
			m_hChargerSprite->SetBrightness( 0 );
			m_hChargerSprite->TurnOff();
		}

		break;

	case CHARGER_STATE_OFF:
		{
			SetSkin( BOLT_SKIN_NORMAL );

			if ( m_hChargerSprite == NULL )
				break;
			
			m_hChargerSprite->SetBrightness( 0 );
			m_hChargerSprite->TurnOff();
		}
		break;

	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pEvent - 
//			*pOperator - 
//-----------------------------------------------------------------------------
void CWeaponCrossbow::Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator )
{
	switch( pEvent->event )
	{
	case EVENT_WEAPON_THROW:
		SetChargerState( CHARGER_STATE_START_LOAD );
		break;

	case EVENT_WEAPON_THROW2:
		SetChargerState( CHARGER_STATE_START_CHARGE );
		break;
	
	case EVENT_WEAPON_THROW3:
		SetChargerState( CHARGER_STATE_READY );
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
bool CWeaponCrossbow::SendWeaponAnim( int iActivity )
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
// Purpose: Stop all zooming and special effects on the viewmodel
//-----------------------------------------------------------------------------
void CWeaponCrossbow::StopEffects( void )
{
	// Stop zooming
	if ( m_bInZoom )
	{
		ToggleZoom();
	}

	// Turn off our sprites
	SetChargerState( CHARGER_STATE_OFF );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWeaponCrossbow::Drop( const Vector &vecVelocity )
{
	StopEffects();
	BaseClass::Drop( vecVelocity );
}
