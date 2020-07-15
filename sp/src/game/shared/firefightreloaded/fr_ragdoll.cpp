#include "cbase.h"
#include "fr_ragdoll.h"

#if CLIENT_DLL
#include <c_basehlplayer.h>
#else

#endif // FR_CLIENT

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#ifdef CLIENT_DLL
//FRRagdoll_Player

IMPLEMENT_CLIENTCLASS_DT_NOBASE(C_FRRagdoll_Player, DT_FRRagdoll_Player, CFRRagdoll_Player)
RecvPropVector(RECVINFO(m_vecRagdollOrigin)),
RecvPropEHandle(RECVINFO(m_hEntity)),
RecvPropInt(RECVINFO(m_nModelIndex)),
RecvPropInt(RECVINFO(m_nForceBone)),
RecvPropVector(RECVINFO(m_vecForce)),
RecvPropVector(RECVINFO(m_vecRagdollVelocity))
END_RECV_TABLE()

C_FRRagdoll_Player::C_FRRagdoll_Player()
{

}

C_FRRagdoll_Player::~C_FRRagdoll_Player()
{
	PhysCleanupFrictionSounds(this);

	if (m_hEntity)
	{
		m_hEntity->CreateModelInstance();
	}
}

void C_FRRagdoll_Player::Interp_Copy(C_BaseAnimatingOverlay* pSourceEntity)
{
	if (!pSourceEntity)
		return;

	VarMapping_t* pSrc = pSourceEntity->GetVarMapping();
	VarMapping_t* pDest = GetVarMapping();

	// Find all the VarMapEntry_t's that represent the same variable.
	for (int i = 0; i < pDest->m_Entries.Count(); i++)
	{
		VarMapEntry_t* pDestEntry = &pDest->m_Entries[i];
		const char* pszName = pDestEntry->watcher->GetDebugName();
		for (int j = 0; j < pSrc->m_Entries.Count(); j++)
		{
			VarMapEntry_t* pSrcEntry = &pSrc->m_Entries[j];
			if (!Q_strcmp(pSrcEntry->watcher->GetDebugName(), pszName))
			{
				pDestEntry->watcher->Copy(pSrcEntry->watcher);
				break;
			}
		}
	}
}

void C_FRRagdoll_Player::ImpactTrace(trace_t* pTrace, int iDamageType, const char* pCustomImpactName)
{
	IPhysicsObject* pPhysicsObject = VPhysicsGetObject();

	if (!pPhysicsObject)
		return;

	Vector dir = pTrace->endpos - pTrace->startpos;

	if (iDamageType == DMG_BLAST)
	{
		dir *= 4000;  // adjust impact strenght

		// apply force at object mass center
		pPhysicsObject->ApplyForceCenter(dir);
	}
	else
	{
		Vector hitpos;

		VectorMA(pTrace->startpos, pTrace->fraction, dir, hitpos);
		VectorNormalize(dir);

		dir *= 4000;  // adjust impact strenght

		// apply force where we hit it
		pPhysicsObject->ApplyForceOffset(dir, hitpos);

		// Blood spray!
		//		FX_CS_BloodSpray( hitpos, dir, 10 );
	}

	m_pRagdoll->ResetRagdollSleepAfterTime();
}


void C_FRRagdoll_Player::CreateFRRagdoll_Player(void)
{
	// First, initialize all our data. If we have the player's entity on our client,
	// then we can make ourselves start out exactly where the player is.
	C_BaseHLPlayer* pPlayer = dynamic_cast<C_BaseHLPlayer*>(m_hEntity.Get());

	if (pPlayer && !pPlayer->IsDormant())
	{
		// move my current model instance to the ragdoll's so decals are preserved.
		pPlayer->SnatchModelInstance(this);

		VarMapping_t* varMap = GetVarMapping();

		// Copy all the interpolated vars from the player entity.
		// The entity uses the interpolated history to get bone velocity.
		bool bRemotePlayer = (pPlayer != C_BasePlayer::GetLocalPlayer());
		if (bRemotePlayer)
		{
			Interp_Copy(pPlayer);

			SetAbsAngles(pPlayer->GetRenderAngles());
			GetRotationInterpolator().Reset();

			m_flAnimTime = pPlayer->m_flAnimTime;
			SetSequence(pPlayer->GetSequence());
			m_flPlaybackRate = pPlayer->GetPlaybackRate();
		}
		else
		{
			// This is the local player, so set them in a default
			// pose and slam their velocity, angles and origin
			SetAbsOrigin(m_vecRagdollOrigin);

			SetAbsAngles(pPlayer->GetRenderAngles());

			SetAbsVelocity(m_vecRagdollVelocity);

			int iSeq = pPlayer->GetSequence();
			if (iSeq == -1)
			{
				Assert(false);	// missing walk_lower?
				iSeq = 0;
			}

			SetSequence(iSeq);	// walk_lower, basic pose
			SetCycle(0.0);

			Interp_Reset(varMap);
		}
	}
	else
	{
		// overwrite network origin so later interpolation will
		// use this position
		SetNetworkOrigin(m_vecRagdollOrigin);

		SetAbsOrigin(m_vecRagdollOrigin);
		SetAbsVelocity(m_vecRagdollVelocity);

		Interp_Reset(GetVarMapping());

	}

	SetModelIndex(m_nModelIndex);

	// Make us a ragdoll..
	m_nRenderFX = kRenderFxRagdoll;

	matrix3x4_t boneDelta0[MAXSTUDIOBONES];
	matrix3x4_t boneDelta1[MAXSTUDIOBONES];
	matrix3x4_t currentBones[MAXSTUDIOBONES];
	const float boneDt = 0.05f;

	if (pPlayer && !pPlayer->IsDormant())
	{
		pPlayer->GetRagdollInitBoneArrays(boneDelta0, boneDelta1, currentBones, boneDt);
	}
	else
	{
		GetRagdollInitBoneArrays(boneDelta0, boneDelta1, currentBones, boneDt);
	}

	InitAsClientRagdoll(boneDelta0, boneDelta1, currentBones, boneDt);
}


void C_FRRagdoll_Player::OnDataChanged(DataUpdateType_t type)
{
	BaseClass::OnDataChanged(type);

	if (type == DATA_UPDATE_CREATED)
	{
		CreateFRRagdoll_Player();
	}
}

IRagdoll* C_FRRagdoll_Player::GetIRagdoll() const
{
	return m_pRagdoll;
}

void C_FRRagdoll_Player::UpdateOnRemove(void)
{
	VPhysicsSetObject(NULL);

	BaseClass::UpdateOnRemove();
}

//-----------------------------------------------------------------------------
// Purpose: clear out any face/eye values stored in the material system
//-----------------------------------------------------------------------------
void C_FRRagdoll_Player::SetupWeights(const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights)
{
	BaseClass::SetupWeights(pBoneToWorld, nFlexWeightCount, pFlexWeights, pFlexDelayedWeights);

	static float destweight[128];
	static bool bIsInited = false;

	CStudioHdr* hdr = GetModelPtr();
	if (!hdr)
		return;

	int nFlexDescCount = hdr->numflexdesc();
	if (nFlexDescCount)
	{
		Assert(!pFlexDelayedWeights);
		memset(pFlexWeights, 0, nFlexWeightCount * sizeof(float));
	}

	if (m_iEyeAttachment > 0)
	{
		matrix3x4_t attToWorld;
		if (GetAttachment(m_iEyeAttachment, attToWorld))
		{
			Vector local, tmp;
			local.Init(1000.0f, 0.0f, 0.0f);
			VectorTransform(local, attToWorld, tmp);
			modelrender->SetViewTarget(GetModelPtr(), GetBody(), tmp);
		}
	}
}
#else

// -------------------------------------------------------------------------------- //
// Ragdoll entities.
// -------------------------------------------------------------------------------- //

LINK_ENTITY_TO_CLASS(fr_ragdoll_player, CFRRagdoll_Player);

IMPLEMENT_SERVERCLASS_ST_NOBASE(CFRRagdoll_Player, DT_FRRagdoll_Player)
SendPropVector(SENDINFO(m_vecRagdollOrigin), -1, SPROP_COORD),
SendPropEHandle(SENDINFO(m_hEntity)),
SendPropModelIndex(SENDINFO(m_nModelIndex)),
SendPropInt(SENDINFO(m_nForceBone), 8, 0),
SendPropVector(SENDINFO(m_vecForce), -1, SPROP_NOSCALE),
SendPropVector(SENDINFO(m_vecRagdollVelocity))
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS(physics_prop_fr_ragdoll, CFRRagdoll_NPC);
LINK_ENTITY_TO_CLASS(prop_fr_ragdoll, CFRRagdoll_NPC);

CFRRagdoll_NPC::CFRRagdoll_NPC(void)
{
}

CFRRagdoll_NPC::~CFRRagdoll_NPC(void)
{
}


#endif // FR_CLIENT

