//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Entity that propagates general data needed by clients for non-player AI characters
//
// $NoKeywords: $
//=============================================================================//

#ifndef MONSTER_RESOURCE_H
#define MONSTER_RESOURCE_H
#ifdef _WIN32
#pragma once
#endif

#include "firefightreloaded/fr_shareddefs.h"
#include "shareddefs.h"
#include "singleplay_gamerules.h"

class CMonsterResource : public CBaseEntity
{
	DECLARE_CLASS( CMonsterResource, CBaseEntity );
public:
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	virtual void Spawn( void );
	virtual	int	 ObjectCaps( void ) { return BaseClass::ObjectCaps() | FCAP_DONT_SAVE; }

	virtual void Update( void );
	virtual int  UpdateTransmitState( void );

	virtual void SetBossHealthPercentage( float percentFull );	// if this is nonnegative, a HUD meter will be shown
	virtual void HideBossHealthMeter( void );

	void SetBossState( int iState ) { m_iBossState = iState; }
	void SetBossName(CBaseEntity* pBoss);

protected:
	CNetworkVar( int, m_iBossHealthPercentageByte );	// 0-255
	CNetworkVar( int, m_iBossState );					// boss state?
	CNetworkString(m_szBossName, MAX_PATH);
};

extern CMonsterResource *g_pMonsterResource;

#endif // MONSTER_RESOURCE_H
