//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "player_command.h"
#include "player.h"
#include "igamemovement.h"
#include "hl_movedata.h"
#include "ipredictionsystem.h"
#include "iservervehicle.h"
#include "hl2_player.h"
#include "vehicle_base.h"
#include "gamestats.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static ConVar fr_charge_frametime_scaling("fr_charge_frametime_scaling", "1", FCVAR_REPLICATED | FCVAR_CHEAT, "When enabled, scale yaw limiting based on client performance (frametime).");
static ConVar fr_charge_turn_rate_multiplier("fr_charge_turn_rate_multiplier", "3", FCVAR_ARCHIVE);
static const float YAW_CAP_SCALE_MIN = 0.2f;
static const float YAW_CAP_SCALE_MAX = 2.f;

class CHLPlayerMove : public CPlayerMove
{
	DECLARE_CLASS( CHLPlayerMove, CPlayerMove );
public:
	CHLPlayerMove() :
		m_bWasInVehicle( false ),
		m_bVehicleFlipped( false ),
		m_bInGodMode( false ),
		m_bInNoClip( false )
	{
		m_vecSaveOrigin.Init();
	}

	void SetupMove( CBasePlayer *player, CUserCmd *ucmd, IMoveHelper *pHelper, CMoveData *move );
	void FinishMove( CBasePlayer *player, CUserCmd *ucmd, CMoveData *move );

private:
	Vector m_vecSaveOrigin;
	bool m_bWasInVehicle;
	bool m_bVehicleFlipped;
	bool m_bInGodMode;
	bool m_bInNoClip;
};

//
//
// PlayerMove Interface
static CHLPlayerMove g_PlayerMove;

//-----------------------------------------------------------------------------
// Singleton accessor
//-----------------------------------------------------------------------------
CPlayerMove *PlayerMove()
{
	return &g_PlayerMove;
}

//

static CHLMoveData g_HLMoveData;
CMoveData *g_pMoveData = &g_HLMoveData;

IPredictionSystem *IPredictionSystem::g_pPredictionSystems = NULL;

float CalculateChargeCap(void)
{
	float flCap = 0.45f;

	flCap *= fr_charge_turn_rate_multiplier.GetFloat();

	// Scale yaw cap based on frametime to prevent differences in turn effectiveness due to variable framerate (between clients mainly)
	if (fr_charge_frametime_scaling.GetBool())
	{
		// There's probably something better to use here as a baseline, instead of TICK_INTERVAL
		float flMod = RemapValClamped(gpGlobals->frametime, (TICK_INTERVAL * YAW_CAP_SCALE_MIN), (TICK_INTERVAL * YAW_CAP_SCALE_MAX), 0.25f, 2.f);
		flCap *= flMod;
	}

	return flCap;
}

void CHLPlayerMove::SetupMove( CBasePlayer *player, CUserCmd *ucmd, IMoveHelper *pHelper, CMoveData *move )
{
	// Call the default SetupMove code.
	BaseClass::SetupMove( player, ucmd, pHelper, move );

	// Convert to HL2 data.
	CHL2_Player *pHLPlayer = static_cast<CHL2_Player*>( player );
	Assert( pHLPlayer );

	CHLMoveData *pHLMove = static_cast<CHLMoveData*>( move );
	Assert( pHLMove );

	player->m_flForwardMove = ucmd->forwardmove;
	player->m_flSideMove = ucmd->sidemove;

	pHLMove->m_bIsSprinting = pHLPlayer->IsSprinting();
	pHLMove->m_bIsCharging = pHLPlayer->IsCharging();

	if ( gpGlobals->frametime != 0 )
	{
		IServerVehicle *pVehicle = player->GetVehicle();

		if ( pVehicle )
		{
			pVehicle->SetupMove( player, ucmd, pHelper, move ); 

			if ( !m_bWasInVehicle )
			{
				m_bWasInVehicle = true;
				m_vecSaveOrigin.Init();
			}
		}
		else
		{
			m_vecSaveOrigin = player->GetAbsOrigin();
			if ( m_bWasInVehicle )
			{
				m_bWasInVehicle = false;
			}

			// targe Exploit fix. Clients sending higher view angle changes then allowed
			// Clamp their YAW Movement
			if (pHLPlayer->IsCharging())
			{
				// Get the view deltas and clamp them if they are too high, give a high tolerance (lag)
				float flCap = CalculateChargeCap();
				flCap *= 2.5f;
				QAngle qAngle = pHLPlayer->m_qPreviousChargeEyeAngle;
				float flDiff = abs(qAngle[YAW]) - abs(ucmd->viewangles[YAW]);
				if (flDiff > flCap)
				{
					//float flReportedPitchDelta = qAngle[YAW] - ucmd->viewangles[YAW];
					if (ucmd->viewangles[YAW] > qAngle[YAW])
					{
						ucmd->viewangles[YAW] = qAngle[YAW] + flCap;
						pHLPlayer->SnapEyeAngles(ucmd->viewangles);
					}
					else // smaller values
					{
						ucmd->viewangles[YAW] = qAngle[YAW] - flCap;
						pHLPlayer->SnapEyeAngles(ucmd->viewangles);
					}
				}

				pHLPlayer->m_qPreviousChargeEyeAngle = ucmd->viewangles;
			}
			else
			{
				pHLPlayer->m_qPreviousChargeEyeAngle = pHLPlayer->EyeAngles();
			}
		}
	}
}


void CHLPlayerMove::FinishMove( CBasePlayer *player, CUserCmd *ucmd, CMoveData *move )
{
	// Call the default FinishMove code.
	BaseClass::FinishMove( player, ucmd, move );
	if ( gpGlobals->frametime != 0 )
	{		
		float distance = 0.0f;
		IServerVehicle *pVehicle = player->GetVehicle();
		if ( pVehicle )
		{
			pVehicle->FinishMove( player, ucmd, move );
			IPhysicsObject *obj = player->GetVehicleEntity()->VPhysicsGetObject();
			if ( obj )
			{
				Vector newPos;
				obj->GetPosition( &newPos, NULL );
				distance = VectorLength( newPos - m_vecSaveOrigin );
				if ( m_vecSaveOrigin == vec3_origin || distance > 100.0f )
					distance = 0.0f;
				m_vecSaveOrigin = newPos;
			}
			
			CPropVehicleDriveable *driveable = dynamic_cast< CPropVehicleDriveable * >( player->GetVehicleEntity() );
			if ( driveable )
			{
				// Overturned and at rest (if still moving it can fix itself)
				bool bFlipped = driveable->IsOverturned() && ( distance < 0.5f );
				if ( m_bVehicleFlipped != bFlipped )
				{
					if ( bFlipped )
					{
						gamestats->Event_FlippedVehicle( player, driveable );
					}
					m_bVehicleFlipped = bFlipped;
				}
			}
			else
			{
				m_bVehicleFlipped = false;
			}
		}
		else
		{
			m_bVehicleFlipped = false;
			distance = VectorLength( player->GetAbsOrigin() - m_vecSaveOrigin );
		}
		if ( distance > 0 )
		{
			gamestats->Event_PlayerTraveled( player, distance, pVehicle ? true : false, !pVehicle && static_cast< CHL2_Player * >( player )->IsSprinting() );
		}
	}

	bool bGodMode = ( player->GetFlags() & FL_GODMODE ) ? true : false;
	if ( m_bInGodMode != bGodMode )
	{
		m_bInGodMode = bGodMode;
		if ( bGodMode )
		{
			gamestats->Event_PlayerEnteredGodMode( player );
		}
	}
	bool bNoClip = ( player->GetMoveType() == MOVETYPE_NOCLIP );
	if ( m_bInNoClip != bNoClip )
	{
		m_bInNoClip = bNoClip;
		if ( bNoClip )
		{
			gamestats->Event_PlayerEnteredNoClip( player );
		}
	}
}
