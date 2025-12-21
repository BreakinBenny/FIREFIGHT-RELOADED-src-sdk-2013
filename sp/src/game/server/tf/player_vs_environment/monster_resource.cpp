//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Entity that propagates general data needed by clients for non-player AI characters
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "player.h"
#include "monster_resource.h"
#include "coordsize.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
IMPLEMENT_SERVERCLASS_ST_NOBASE( CMonsterResource, DT_MonsterResource )
	SendPropInt( SENDINFO( m_iBossHealthPercentageByte ), 8, SPROP_UNSIGNED ),
	SendPropInt( SENDINFO( m_iBossState ) ),
	SendPropString(SENDINFO(m_szBossName)),
END_SEND_TABLE()

BEGIN_DATADESC( CMonsterResource )
	DEFINE_FIELD( m_iBossHealthPercentageByte, FIELD_INTEGER ),

	// Function Pointers
	DEFINE_FUNCTION( Update ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( monster_resource, CMonsterResource );

CMonsterResource *g_pMonsterResource = NULL;

//-----------------------------------------------------------------------------
void CMonsterResource::Spawn( void )
{
	SetThink( &CMonsterResource::Update );
	SetNextThink( gpGlobals->curtime );

	m_iBossHealthPercentageByte = 0;
	m_iBossState = 0;
}

//-----------------------------------------------------------------------------
//
// The Player resource is always transmitted to clients
//
int CMonsterResource::UpdateTransmitState( void )
{
	// ALWAYS transmit to all clients.
	return SetTransmitState( FL_EDICT_ALWAYS );
}

//-----------------------------------------------------------------------------
void CMonsterResource::Update( void )
{
	SetNextThink( gpGlobals->curtime + 0.1f );
}

//-----------------------------------------------------------------------------
void CMonsterResource::SetBossHealthPercentage( float percentFull )
{
	m_iBossHealthPercentageByte = 255.0f * percentFull;
}

//-----------------------------------------------------------------------------
void CMonsterResource::HideBossHealthMeter( void )
{
	m_iBossHealthPercentageByte = 0;
}

void CMonsterResource::SetBossName(CBaseEntity* pBoss)
{
	NpcName name;
	SPGameRules()->GetNPCName(name, pBoss);
	Q_strncpy(m_szBossName.GetForModify(), name, MAX_PATH);
}
