//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#include "cbase.h"
#include "predicted_viewmodel.h"
#ifdef CLIENT_DLL
#include "c_baseviewmodel.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

LINK_ENTITY_TO_CLASS( predicted_viewmodel, CPredictedViewModel );

IMPLEMENT_NETWORKCLASS_ALIASED( PredictedViewModel, DT_PredictedViewModel )

BEGIN_NETWORK_TABLE( CPredictedViewModel, DT_PredictedViewModel )
END_NETWORK_TABLE()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
#ifdef CLIENT_DLL
CPredictedViewModel::CPredictedViewModel() : m_LagAnglesHistory("CPredictedViewModel::m_LagAnglesHistory")
{
	m_vLagAngles.Init();
	m_LagAnglesHistory.Setup( &m_vLagAngles, 0 );
}
#else
CPredictedViewModel::CPredictedViewModel()
{
}
#endif


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CPredictedViewModel::~CPredictedViewModel()
{
}

#ifdef CLIENT_DLL
ConVar cl_wpn_sway_interp( "cl_wpn_sway_interp", "0.1", FCVAR_CLIENTDLL );
ConVar cl_wpn_sway_scale( "cl_wpn_sway_scale", "1.0", FCVAR_CLIENTDLL|FCVAR_CHEAT );
ConVar cl_wpn_sway_scale_ironsight("cl_wpn_sway_scale_ironsight", "0.05", FCVAR_CLIENTDLL | FCVAR_CHEAT);

//used to fix compile on Linux.
bool IsFlipped(CBaseCombatWeapon* pWeapon)
{
	ConVar* cl_righthand = cvar->FindVar("cl_righthand");

	if (pWeapon)
	{
		if (pWeapon->IsDualWielding() || (viewmodel_adjust_user_position_mode.GetInt() == VM_CENTERED && !pWeapon->GetWpnData().m_bCenterAllowFlipped))
			return false;

		const FileWeaponInfo_t* pInfo = &pWeapon->GetWpnData();
		return pInfo->m_bAllowFlipping && pInfo->m_bBuiltRightHanded != cl_righthand->GetBool();
	}

	return !cl_righthand->GetBool(); // hack for scout ball projeciles to have properly flipped viewmodels
}
#endif

void CPredictedViewModel::CalcViewModelLag( Vector& origin, QAngle& angles, QAngle& original_angles )
{
#ifdef CLIENT_DLL
	// Calculate our drift
	Vector	forward, right, up;
	AngleVectors( angles, &forward, &right, &up );
		
	// Add an entry to the history.
	m_vLagAngles = angles;
	m_LagAnglesHistory.NoteChanged( gpGlobals->curtime, cl_wpn_sway_interp.GetFloat(), false );
		
	// Interpolate back 100ms.
	m_LagAnglesHistory.Interpolate( gpGlobals->curtime, cl_wpn_sway_interp.GetFloat() );
		
	// Now take the 100ms angle difference and figure out how far the forward vector moved in local space.
	Vector vLaggedForward;
	QAngle angleDiff = m_vLagAngles - angles;
	AngleVectors( -angleDiff, &vLaggedForward, 0, 0 );
	Vector vForwardDiff = Vector(1,0,0) - vLaggedForward;

	// Now offset the origin using that.
	CBaseCombatWeapon* pWeapon = GetOwningWeapon();

	if (pWeapon && pWeapon->IsIronsighted())
	{
		vForwardDiff *= cl_wpn_sway_scale_ironsight.GetFloat();
	}
	else
	{
		vForwardDiff *= cl_wpn_sway_scale.GetFloat();
	}

	if (IsFlipped(pWeapon))
	{
		origin += forward * vForwardDiff.x + right * vForwardDiff.y + up * vForwardDiff.z;
	}
	else
	{
		origin += forward * vForwardDiff.x + right * -vForwardDiff.y + up * vForwardDiff.z;
	}
#endif
}