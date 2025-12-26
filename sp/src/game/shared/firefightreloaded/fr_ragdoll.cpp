#include "cbase.h"
#include "fr_ragdoll.h"

#if CLIENT_DLL
#include "c_basehlplayer.h"
#include "bone_setup.h"
#include "c_gib.h"
#else

#endif // FR_CLIENT

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar fr_ragdoll_gore("fr_ragdoll_gore", "1", FCVAR_ARCHIVE | FCVAR_REPLICATED);
ConVar fr_ragdoll_gore_gib_amount("fr_ragdoll_gore_gib_amount", "4", FCVAR_ARCHIVE | FCVAR_REPLICATED, "This value sets the minimum amount of gibs created when a player gets gibbed. This value is multiplied by fr_ragdoll_gore_gib_amount_mult when calculating maximum gibs.", true, 1.0, false, 0.0);
ConVar fr_ragdoll_gore_gib_amount_head("fr_ragdoll_gore_gib_amount_head", "8", FCVAR_ARCHIVE | FCVAR_REPLICATED, "This value sets the minimum amount of gibs created when a player's head gets gibbed. This value is multiplied by fr_ragdoll_gore_gib_amount_mult when calculating maximum gibs.", true, 1.0, false, 0.0);
ConVar fr_ragdoll_gore_gib_amount_mult("fr_ragdoll_gore_gib_amount_mult", "2", FCVAR_ARCHIVE | FCVAR_REPLICATED, "The multiplier used for calculating how many gibs can be spawned at once.");

#ifdef CLIENT_DLL
//-----------------------------------------------------------------------------
// Purpose: Gore!
// Credit to Open Fortress for the gore code!
//-----------------------------------------------------------------------------

// Scale head to nothing
static void ScaleGoreHead(C_BaseAnimating* pAnimating)
{
	if (pAnimating)
	{
		int iBone = -1;

		const char* boneNames[] = { "ValveBiped.Bip01_Head1", "ValveBiped.Bip01_Neck1" };

		for (int i = 0; i < 2; i++)
		{
			iBone = pAnimating->LookupBone(boneNames[i]);
			if (iBone != -1)
				MatrixScaleByZero(pAnimating->GetBoneForWrite(iBone));
		}
	}

}

// Scale left arm to nothing
static void ScaleGoreLeftArm(C_BaseAnimating* pAnimating)
{
	if (pAnimating)
	{
		int iBone = -1;

		const char* boneNames[] = { "ValveBiped.Bip01_L_UpperArm", "ValveBiped.Bip01_L_Hand", "ValveBiped.Bip01_L_Forearm",
									"ValveBiped.Bip01_L_Finger0", "ValveBiped.Bip01_L_Finger01", "ValveBiped.Bip01_L_Finger02",
									"ValveBiped.Bip01_L_Finger1", "ValveBiped.Bip01_L_Finger11", "ValveBiped.Bip01_L_Finger12",
									"ValveBiped.Bip01_L_Finger2", "ValveBiped.Bip01_L_Finger21", "ValveBiped.Bip01_L_Finger22",
									"ValveBiped.Bip01_L_Finger3", "ValveBiped.Bip01_L_Finger31", "ValveBiped.Bip01_L_Finger32",
									"ValveBiped.Bip01_L_Finger4", "ValveBiped.Bip01_L_Finger41", "ValveBiped.Bip01_L_Finger42",
		};

		for (int i = 0; i < 18; i++)
		{
			iBone = pAnimating->LookupBone(boneNames[i]);
			if (iBone != -1)
				MatrixScaleByZero(pAnimating->GetBoneForWrite(iBone));
		}
	}
}

// Scale left hand to nothing
static void ScaleGoreLeftHand(C_BaseAnimating* pAnimating)
{
	if (pAnimating)
	{
		int iBone = -1;

		const char* boneNames[] = { "ValveBiped.Bip01_L_Hand",
									"ValveBiped.Bip01_L_Finger0", "ValveBiped.Bip01_L_Finger01", "ValveBiped.Bip01_L_Finger02",
									"ValveBiped.Bip01_L_Finger1", "ValveBiped.Bip01_L_Finger11", "ValveBiped.Bip01_L_Finger12",
									"ValveBiped.Bip01_L_Finger2", "ValveBiped.Bip01_L_Finger21", "ValveBiped.Bip01_L_Finger22",
									"ValveBiped.Bip01_L_Finger3", "ValveBiped.Bip01_L_Finger31", "ValveBiped.Bip01_L_Finger32",
									"ValveBiped.Bip01_L_Finger4", "ValveBiped.Bip01_L_Finger41", "ValveBiped.Bip01_L_Finger42",
		};

		for (int i = 0; i < 16; i++)
		{
			iBone = pAnimating->LookupBone(boneNames[i]);
			if (iBone != -1)
				MatrixScaleByZero(pAnimating->GetBoneForWrite(iBone));
		}
	}
}

// Scale right arm to nothing
static void ScaleGoreRightArm(C_BaseAnimating* pAnimating)
{
	if (pAnimating)
	{
		int iBone = -1;

		const char* boneNames[] = { "ValveBiped.Bip01_R_UpperArm", "ValveBiped.Bip01_R_Hand", "ValveBiped.Bip01_R_Forearm",
									"ValveBiped.Bip01_R_Finger0", "ValveBiped.Bip01_R_Finger01", "ValveBiped.Bip01_R_Finger02",
									"ValveBiped.Bip01_R_Finger1", "ValveBiped.Bip01_R_Finger11", "ValveBiped.Bip01_R_Finger12",
									"ValveBiped.Bip01_R_Finger2", "ValveBiped.Bip01_R_Finger21", "ValveBiped.Bip01_R_Finger22",
									"ValveBiped.Bip01_R_Finger3", "ValveBiped.Bip01_R_Finger31", "ValveBiped.Bip01_R_Finger32",
									"ValveBiped.Bip01_R_Finger4", "ValveBiped.Bip01_R_Finger41", "ValveBiped.Bip01_R_Finger42",
		};

		for (int i = 0; i < 18; i++)
		{
			iBone = pAnimating->LookupBone(boneNames[i]);
			if (iBone != -1)
				MatrixScaleByZero(pAnimating->GetBoneForWrite(iBone));
		}
	}
}

// Scale right hand to nothing
static void ScaleGoreRightHand(C_BaseAnimating* pAnimating)
{
	if (pAnimating)
	{
		int iBone = -1;

		const char* boneNames[] = { "ValveBiped.Bip01_R_Hand",
									"ValveBiped.Bip01_R_Finger0", "ValveBiped.Bip01_R_Finger01", "ValveBiped.Bip01_R_Finger02",
									"ValveBiped.Bip01_R_Finger1", "ValveBiped.Bip01_R_Finger11", "ValveBiped.Bip01_R_Finger12",
									"ValveBiped.Bip01_R_Finger2", "ValveBiped.Bip01_R_Finger21", "ValveBiped.Bip01_R_Finger22",
									"ValveBiped.Bip01_R_Finger3", "ValveBiped.Bip01_R_Finger31", "ValveBiped.Bip01_R_Finger32",
									"ValveBiped.Bip01_R_Finger4", "ValveBiped.Bip01_R_Finger41", "ValveBiped.Bip01_R_Finger42",
		};

		for (int i = 0; i < 16; i++)
		{
			iBone = pAnimating->LookupBone(boneNames[i]);
			if (iBone != -1)
				MatrixScaleByZero(pAnimating->GetBoneForWrite(iBone));
		}
	}
}

// Scale left knee to nothing
static void ScaleGoreLeftKnee(C_BaseAnimating* pAnimating)
{
	if (pAnimating)
	{
		int iBone = -1;

		const char* boneNames[] = { "ValveBiped.Bip01_L_Knee", "ValveBiped.Bip01_L_Shin", "ValveBiped.Bip01_L_Ankle",
									"ValveBiped.Bip01_L_Calf", "ValveBiped.Bip01_L_Foot", "ValveBiped.Bip01_L_Toe0"
		};

		for (int i = 0; i < 6; i++)
		{
			iBone = pAnimating->LookupBone(boneNames[i]);
			if (iBone != -1)
				MatrixScaleByZero(pAnimating->GetBoneForWrite(iBone));
		}
	}
}

// Scale left foot to nothing
static void ScaleGoreLeftFoot(C_BaseAnimating* pAnimating)
{
	if (pAnimating)
	{
		int iBone = -1;

		const char* boneNames[] = { "ValveBiped.Bip01_L_Foot", "ValveBiped.Bip01_L_Toe0" };

		for (int i = 0; i < 2; i++)
		{
			iBone = pAnimating->LookupBone(boneNames[i]);
			if (iBone != -1)
				MatrixScaleByZero(pAnimating->GetBoneForWrite(iBone));
		}
	}
}

// Scale right knee to nothing
static void ScaleGoreRightKnee(C_BaseAnimating* pAnimating)
{
	if (pAnimating)
	{
		int iBone = -1;

		const char* boneNames[] = { "ValveBiped.Bip01_R_Knee", "ValveBiped.Bip01_R_Shin", "ValveBiped.Bip01_R_Ankle",
									"ValveBiped.Bip01_R_Calf", "ValveBiped.Bip01_R_Foot", "ValveBiped.Bip01_R_Toe0"
		};

		for (int i = 0; i < 6; i++)
		{
			iBone = pAnimating->LookupBone(boneNames[i]);
			if (iBone != -1)
				MatrixScaleByZero(pAnimating->GetBoneForWrite(iBone));
		}
	}
}

// Scale right foot to nothing
static void ScaleGoreRightFoot(C_BaseAnimating* pAnimating)
{
	if (pAnimating)
	{
		int iBone = -1;

		const char* boneNames[] = { "ValveBiped.Bip01_R_Foot", "ValveBiped.Bip01_R_Toe0" };

		for (int i = 0; i < 2; i++)
		{
			iBone = pAnimating->LookupBone(boneNames[i]);
			if (iBone != -1)
				MatrixScaleByZero(pAnimating->GetBoneForWrite(iBone));
		}
	}
}
//FRRagdoll

IMPLEMENT_CLIENTCLASS_DT_NOBASE(C_FRRagdoll, DT_FRRagdoll, CFRRagdoll)
RecvPropVector(RECVINFO(m_vecRagdollOrigin)),
RecvPropEHandle(RECVINFO(m_hEntity)),
RecvPropInt(RECVINFO(m_nModelIndex)),
RecvPropInt(RECVINFO(m_nForceBone)),
RecvPropVector(RECVINFO(m_vecForce)),
RecvPropVector(RECVINFO(m_vecRagdollVelocity)),
RecvPropInt(RECVINFO(m_iGoreHead)),
RecvPropInt(RECVINFO(m_iGoreLeftArm)),
RecvPropInt(RECVINFO(m_iGoreRightArm)),
RecvPropInt(RECVINFO(m_iGoreLeftLeg)),
RecvPropInt(RECVINFO(m_iGoreRightLeg))
END_RECV_TABLE()

C_FRRagdoll::C_FRRagdoll()
{
	m_iGoreHead = 0;
	m_iGoreLeftArm = 0;
	m_iGoreRightArm = 0;
	m_iGoreLeftLeg = 0;
	m_iGoreRightLeg = 0;
}

C_FRRagdoll::~C_FRRagdoll()
{
	PhysCleanupFrictionSounds(this);

	if (m_hEntity)
	{
		m_hEntity->CreateModelInstance();
	}
}

void C_FRRagdoll::Interp_Copy(C_BaseAnimatingOverlay* pSourceEntity)
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

void C_FRRagdoll::ImpactTrace(trace_t* pTrace, int iDamageType, const char* pCustomImpactName)
{
	IPhysicsObject* pPhysicsObject = VPhysicsGetObject();

	if (!pPhysicsObject)
		return;

	Vector dir = pTrace->endpos - pTrace->startpos;
	
	// m_iGore<limb> has a level, from 0 to 3
	// 1 is unused (reserved for normal TF bodygroups like pyro's head)
	// 2 means the lower limb is marked for removal, 3 means the upper limb is marked for removal, the head is an exception as it only has level 2
	// if our current level is at level 3, that means we can't dismember this limb anymore
	// if our current level is at level 2, that means we can dismember this limb once more up to level 3
	// if our current level is at level 0/1, that means we can dismember this limb up to level 2
	// Dismember<limb> function accepts true or false, true means this limb will be dismembered to level 3, false means dismembered to level 2

	if (m_bGoreEnabled)
	{
		switch (pTrace->hitgroup)
		{
		case HITGROUP_HEAD:
			if (m_iGoreHead == 3)
			{
				break;
			}
			else if (m_iGoreHead == 2)
			{
				break;
			}
			else
			{
				DismemberHead();
				break;
			}
		case HITGROUP_LEFTARM:
			if (m_iGoreLeftArm == 3)
			{
				break;
			}
			else if (m_iGoreLeftArm == 2)
			{
				DismemberLeftArm(true);
				break;
			}
			else
			{
				DismemberLeftArm(false);
				break;
			}
		case HITGROUP_RIGHTARM:
			if (m_iGoreRightArm == 3)
			{
				break;
			}
			else if (m_iGoreRightArm == 2)
			{
				DismemberRightArm(true);
				break;
			}
			else
			{
				DismemberRightArm(false);
				break;
			}
		case HITGROUP_LEFTLEG:
			if (m_iGoreLeftLeg == 3)
			{
				break;
			}
			else if (m_iGoreLeftLeg == 2)
			{
				DismemberLeftLeg(true);
				break;
			}
			else
			{
				DismemberLeftLeg(false);
				break;
			}
		case HITGROUP_RIGHTLEG:
			if (m_iGoreRightLeg == 3)
			{
				break;
			}
			else if (m_iGoreRightLeg == 2)
			{
				DismemberRightLeg(true);
				break;
			}
			else
			{
				DismemberRightLeg(false);
				break;
			}
		default:
			break;
		}
	}

	if (iDamageType == DMG_BLAST)
	{
		dir *= 4000;  // adjust impact strenght

		// apply force at object mass center
		pPhysicsObject->ApplyForceCenter(dir);
		
		if (m_bGoreEnabled)
			DismemberRandomLimbs();
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


void C_FRRagdoll::CreateFRRagdoll(void)
{
	// First, initialize all our data. If we have the player's entity on our client,
	// then we can make ourselves start out exactly where the player is.
	C_BaseAnimating *pEntity = dynamic_cast<C_BaseAnimating*>(m_hEntity.Get());

	if (pEntity && !pEntity->IsDormant())
	{
		// move my current model instance to the ragdoll's so decals are preserved.
		pEntity->SnatchModelInstance(this);

		VarMapping_t* varMap = GetVarMapping();

		// Copy all the interpolated vars from the player entity.
		// The entity uses the interpolated history to get bone velocity.

		//even though FR is a singleplayer game, create ragdoll entities for those who choose to play the buggy coop mode.
		C_BaseHLPlayer* pPlayer = dynamic_cast<C_BaseHLPlayer*>(m_hEntity.Get());
		bool bRemotePlayer = (pPlayer && pPlayer->IsPlayer() && pPlayer != C_BasePlayer::GetLocalPlayer());
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

			SetAbsAngles(pEntity->GetRenderAngles());

			SetAbsVelocity(m_vecRagdollVelocity);

			int iSeq = pEntity->GetSequence();
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

	if (pEntity && !pEntity->IsDormant())
	{
		pEntity->GetRagdollInitBoneArrays(boneDelta0, boneDelta1, currentBones, boneDt);
	}
	else
	{
		GetRagdollInitBoneArrays(boneDelta0, boneDelta1, currentBones, boneDt);
	}

	InitAsClientRagdoll(boneDelta0, boneDelta1, currentBones, boneDt);
	
	int iBone = LookupBone("ValveBiped.Bip01_Neck1");

	if (iBone != -1)
	{
		m_bGoreEnabled = fr_ragdoll_gore.GetBool();
	}
	else
	{
		m_bGoreEnabled = false;
	}

	if (m_bGoreEnabled)
		m_BoneAccessor.SetWritableBones(BONE_USED_BY_ANYTHING);

	SetNextClientThink(gpGlobals->curtime + 0.1f);
}

void C_FRRagdoll::Bleed(void)
{
	// emit some blood decals if necessary
	if (m_iGoreDecalAmount > 0 && m_fGoreDecalTime < gpGlobals->curtime)
	{
		// emit another decal again after 0.1 seconds
		m_fGoreDecalTime = gpGlobals->curtime + 0.1f;
		m_iGoreDecalAmount--;

		if (m_iGoreDecalBone != -1)
		{
			Vector direction;
			Vector start;
			QAngle dummy;
			trace_t	tr;

			GetBonePosition(m_iGoreDecalBone, start, dummy);

			// any random direction
			direction.x = random->RandomFloat(-64, 64);
			direction.y = random->RandomFloat(-64, 64);
			direction.z = random->RandomFloat(-64, 64);

			UTIL_TraceLine(start, start + direction, MASK_SOLID_BRUSHONLY, this, COLLISION_GROUP_NONE, &tr);
			UTIL_BloodDecalTrace(&tr, BLOOD_COLOR_RED);

			//debugoverlay->AddLineOverlay( start, start + direction, 0, 255, 0, true, 1 ); 
		}
	}
}

void C_FRRagdoll::ClientThink(void)
{
	SetNextClientThink(CLIENT_THINK_ALWAYS);

	Bleed();
}

void C_FRRagdoll::OnDataChanged(DataUpdateType_t type)
{
	BaseClass::OnDataChanged(type);

	if (type == DATA_UPDATE_CREATED)
	{
		CreateFRRagdoll();
		InitDismember();
	}
}

IRagdoll* C_FRRagdoll::GetIRagdoll() const
{
	return m_pRagdoll;
}

void C_FRRagdoll::UpdateOnRemove(void)
{
	VPhysicsSetObject(NULL);

	BaseClass::UpdateOnRemove();
}

//-----------------------------------------------------------------------------
// Purpose: clear out any face/eye values stored in the material system
//-----------------------------------------------------------------------------
void C_FRRagdoll::SetupWeights(const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights)
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

//---------------------------------------------------------------------------- -
// Purpose: Scale the bones that need to be scaled for gore
//-----------------------------------------------------------------------------
void C_FRRagdoll::BuildTransformations(CStudioHdr * pStudioHdr, Vector * pos, Quaternion q[], const matrix3x4_t & cameraTransform, int boneMask, CBoneBitList & boneComputed)
{
	BaseClass::BuildTransformations(pStudioHdr, pos, q, cameraTransform, boneMask, boneComputed);
	
	if (m_bGoreEnabled)
		ScaleGoreBones();
}

void C_FRRagdoll::ScaleGoreBones()
{
	if (m_iGoreHead > 1)
		ScaleGoreHead(this);

	if (m_iGoreLeftArm == 2)
		ScaleGoreLeftHand(this);
	else if (m_iGoreLeftArm == 3)
		ScaleGoreLeftArm(this);

	if (m_iGoreRightArm == 2)
		ScaleGoreRightHand(this);
	else if (m_iGoreRightArm == 3)
		ScaleGoreRightArm(this);

	if (m_iGoreLeftLeg == 2)
		ScaleGoreLeftFoot(this);
	else if (m_iGoreLeftLeg == 3)
		ScaleGoreLeftKnee(this);

	if (m_iGoreRightLeg == 2)
		ScaleGoreRightFoot(this);
	else if (m_iGoreRightLeg == 3)
		ScaleGoreRightKnee(this);
}

void C_FRRagdoll::DismemberHead()
{
	DismemberBase("head", true, true, "ValveBiped.Bip01_Neck1");

	m_iGoreHead = 3;
}

void C_FRRagdoll::DismemberBase(char const* szBodyPart, bool bLevel, bool bBloodEffects, char const* szParticleBone)
{
	int iAttach = LookupBone(szParticleBone);

	int iDefaultMax = RandomInt(fr_ragdoll_gore_gib_amount.GetInt(), (fr_ragdoll_gore_gib_amount.GetInt() * fr_ragdoll_gore_gib_amount_mult.GetFloat()));
	int iBloodAmount = iDefaultMax;
	int iGibAmount = iDefaultMax;

	if (iAttach != -1)
	{
		EmitSound("Gore.Headshot");

		if (FStrEq(szBodyPart, "head"))
		{
			ParticleProp()->Create("smod_headshot_r", PATTACH_BONE_FOLLOW, szParticleBone);
			ParticleProp()->Create("smod_blood_decap_r", PATTACH_BONE_FOLLOW, szParticleBone);
			int iDefaultHeadshotMax = RandomInt(fr_ragdoll_gore_gib_amount_head.GetInt(), (fr_ragdoll_gore_gib_amount_head.GetInt() * fr_ragdoll_gore_gib_amount_mult.GetFloat()));
			iBloodAmount += iDefaultHeadshotMax;
			iGibAmount += iDefaultHeadshotMax;
		}
		else
		{
			ParticleProp()->Create("smod_blood_gib_r", PATTACH_BONE_FOLLOW, szParticleBone);
		}

		for (int i = 0; i <= iGibAmount; i++)
		{
			int randModel = RandomInt(0, 1);
			const char* model = "models/gibs/pgib_p3.mdl";

			if (randModel == 1)
			{
				model = "models/gibs/pgib_p4.mdl";
			}

			Vector start;
			QAngle dummy;

			GetBonePosition(iAttach, start, dummy);
			Vector offset = start + RandomVector(-64, 64);
			C_Gib::CreateClientsideGib(model, offset, GetAbsVelocity() + RandomVector(-25.0f, 25.0f), RandomAngularImpulse(-32, 32), 30);
		}

		m_iGoreDecalAmount += iBloodAmount;
		m_iGoreDecalBone = iAttach;

		// bleed once this frame.
		Bleed();
	}
}

void C_FRRagdoll::DismemberLeftArm(bool bLevel)
{
	DismemberBase("leftarm", bLevel, true, bLevel ? "ValveBiped.Bip01_L_UpperArm" : "ValveBiped.Bip01_L_Forearm");

	if (bLevel)
		m_iGoreLeftArm = 3;
	else
		m_iGoreLeftArm = 2;
}

void C_FRRagdoll::DismemberRightArm(bool bLevel)
{
	DismemberBase("rightarm", bLevel, true, bLevel ? "ValveBiped.Bip01_R_UpperArm" : "ValveBiped.Bip01_R_Forearm");

	if (bLevel)
		m_iGoreRightArm = 3;
	else
		m_iGoreRightArm = 2;
}

void C_FRRagdoll::DismemberLeftLeg(bool bLevel)
{
	DismemberBase("leftleg", bLevel, true, bLevel ? "ValveBiped.Bip01_L_Knee" : "ValveBiped.Bip01_L_Foot");

	if (bLevel)
		m_iGoreLeftLeg = 3;
	else
		m_iGoreLeftLeg = 2;
}

void C_FRRagdoll::DismemberRightLeg(bool bLevel)
{
	DismemberBase("rightleg", bLevel, true, bLevel ? "ValveBiped.Bip01_R_Knee" : "ValveBiped.Bip01_R_Foot");

	if (bLevel)
		m_iGoreRightLeg = 3;
	else
		m_iGoreRightLeg = 2;
}

void C_FRRagdoll::InitDismember()
{
	// head does not have two levels of dismemberment, only one
	if (m_iGoreHead > 1)
		DismemberHead();

	if (m_iGoreLeftArm == 3)
		DismemberLeftArm(true);
	else if (m_iGoreLeftArm == 2)
		DismemberLeftArm(false);

	if (m_iGoreRightArm == 3)
		DismemberRightArm(true);
	else if (m_iGoreRightArm == 2)
		DismemberRightArm(false);

	if (m_iGoreLeftLeg == 3)
		DismemberLeftLeg(true);
	else if (m_iGoreLeftLeg == 2)
		DismemberLeftLeg(false);

	if (m_iGoreRightLeg == 3)
		DismemberRightLeg(true);
	else if (m_iGoreRightLeg == 2)
		DismemberRightLeg(false);
}

void C_FRRagdoll::DismemberRandomLimbs(void)
{
	int iGore = 0;

	// NOTE: head is not dismembered here intentionally

	if (m_iGoreLeftArm < 3)
	{
		iGore = random->RandomInt(0, 3);

		if (iGore == 1)
			iGore = 2;

		if (iGore == 2)
			DismemberLeftArm(false);
		else if (iGore == 3)
			DismemberLeftArm(true);
	}

	if (m_iGoreRightArm < 3)
	{
		iGore = random->RandomInt(0, 3);

		if (iGore == 1)
			iGore = 2;

		if (iGore == 2)
			DismemberRightArm(false);
		else if (iGore == 3)
			DismemberRightArm(true);
	}

	if (m_iGoreLeftLeg < 3)
	{
		iGore = random->RandomInt(0, 3);

		if (iGore == 1)
			iGore = 2;

		if (iGore == 2)
			DismemberLeftLeg(false);
		else if (iGore == 3)
			DismemberLeftLeg(true);
	}

	if (m_iGoreRightLeg < 3)
	{
		iGore = random->RandomInt(0, 3);

		if (iGore == 1)
			iGore = 2;

		if (iGore == 2)
			DismemberRightLeg(false);
		else if (iGore == 3)
			DismemberRightLeg(true);
	}
}

#else

// -------------------------------------------------------------------------------- //
// Ragdoll entities.
// -------------------------------------------------------------------------------- //

LINK_ENTITY_TO_CLASS(fr_ragdoll, CFRRagdoll);

IMPLEMENT_SERVERCLASS_ST_NOBASE(CFRRagdoll, DT_FRRagdoll)
SendPropVector(SENDINFO(m_vecRagdollOrigin), -1, SPROP_COORD),
SendPropEHandle(SENDINFO(m_hEntity)),
SendPropModelIndex(SENDINFO(m_nModelIndex)),
SendPropInt(SENDINFO(m_nForceBone), 8, 0),
SendPropVector(SENDINFO(m_vecForce), -1, SPROP_NOSCALE),
SendPropVector(SENDINFO(m_vecRagdollVelocity)),
SendPropInt(SENDINFO(m_iGoreHead), 2),
SendPropInt(SENDINFO(m_iGoreLeftArm), 2),
SendPropInt(SENDINFO(m_iGoreRightArm), 2),
SendPropInt(SENDINFO(m_iGoreLeftLeg), 2),
SendPropInt(SENDINFO(m_iGoreRightLeg), 2),
END_SEND_TABLE()

#endif // FR_CLIENT

