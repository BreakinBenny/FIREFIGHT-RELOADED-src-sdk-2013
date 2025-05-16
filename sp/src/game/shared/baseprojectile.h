//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef BASEPROJECTILE_H
#define BASEPROJECTILE_H
#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"

#ifdef GAME_DLL
#include "basecombatcharacter.h"
#else
#include "c_basecombatcharacter.h"
#endif

#ifdef CLIENT_DLL
#define CBaseProjectile C_BaseProjectile
#endif // CLIENT_DLL

//=============================================================================
//
// Base Projectile.
//
//=============================================================================
class CBaseProjectile : public CBaseCombatCharacter
{
public:
	DECLARE_CLASS( CBaseProjectile, CBaseCombatCharacter);
	DECLARE_NETWORKCLASS();

	CBaseProjectile();

#ifdef GAME_DLL
	virtual int GetDestroyableHitCount( void ) const { return m_iDestroyableHitCount; }
	void IncrementDestroyableHitCount( void ) { ++m_iDestroyableHitCount; }
#endif // GAME_DLL

	virtual bool IsDestroyable( void ) { return false; }
	virtual void Destroy( bool bBlinkOut = true, bool bBreakRocket = false ) {}

protected:
#ifdef GAME_DLL
	int m_iDestroyableHitCount;
#endif // GAME_DLL
};

#endif // BASEPROJECTILE_H
