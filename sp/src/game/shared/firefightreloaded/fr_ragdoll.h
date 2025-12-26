#ifdef CLIENT_DLL
#else
#include "basecombatcharacter.h"
#include "BaseAnimatingOverlay.h"
#include "physics_prop_ragdoll.h"
#endif

#ifdef CLIENT_DLL

class C_FRRagdoll : public C_BaseAnimatingOverlay
{
public:
	DECLARE_CLASS(C_FRRagdoll, C_BaseAnimatingOverlay);
	DECLARE_CLIENTCLASS();

	C_FRRagdoll();
	~C_FRRagdoll();

	virtual void OnDataChanged(DataUpdateType_t type);

	void ClientThink(void);

	IRagdoll* GetIRagdoll() const;

	void ImpactTrace(trace_t* pTrace, int iDamageType, const char* pCustomImpactName);
	void UpdateOnRemove(void);
	virtual void SetupWeights(const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights);

	// c_baseanimating functions
	virtual void BuildTransformations(CStudioHdr* pStudioHdr, Vector* pos, Quaternion q[], const matrix3x4_t& cameraTransform, int boneMask, CBoneBitList& boneComputed);
	
	// Gore
	void ScaleGoreBones(void);
	void InitDismember(void);
	void DismemberHead();
	void DismemberBase(char const* szBodyPart, bool bLevel, bool bBloodEffects, char const* szParticleBone);
	void DismemberLeftArm(bool bLevel);
	void DismemberRightArm(bool bLevel);
	void DismemberLeftLeg(bool bLevel);
	void DismemberRightLeg(bool bLevel);
	void DismemberRandomLimbs(void);
	void Bleed(void);

	// gore stuff
	int m_iGoreHead;
	int m_iGoreLeftArm;
	int m_iGoreRightArm;
	int m_iGoreLeftLeg;
	int m_iGoreRightLeg;

	// checks if this model can utilise gore
	bool m_bGoreEnabled;
	// how many blood decals to spray out when we dismember a limb overtime
	int m_iGoreDecalAmount;
	// the index of the bone we should spray blood decals out from
	int m_iGoreDecalBone;
	// time when blood decal was sprayed so that blood decals sprays are delayed in bursts for ClientThink
	float m_fGoreDecalTime;

private:

	C_FRRagdoll(const C_FRRagdoll&) {}

	void Interp_Copy(C_BaseAnimatingOverlay* pDestinationEntity);
	void CreateFRRagdoll(void);

private:

	EHANDLE	m_hEntity;
	CNetworkVector(m_vecRagdollVelocity);
	CNetworkVector(m_vecRagdollOrigin);
};
#else

class CFRRagdoll : public CBaseAnimatingOverlay
{
public:
	DECLARE_CLASS(CFRRagdoll, CBaseAnimatingOverlay);
	DECLARE_SERVERCLASS();

	// Transmit ragdolls to everyone.
	virtual int UpdateTransmitState()
	{
		return SetTransmitState(FL_EDICT_ALWAYS);
	}

public:
	// In case the client has the player entity, we transmit the player index.
	// In case the client doesn't have it, we transmit the player's model index, origin, and angles
	// so they can create a ragdoll in the right place.
	CNetworkHandle(CBaseEntity, m_hEntity);	// networked entity handle 
	CNetworkVector(m_vecRagdollVelocity);
	CNetworkVector(m_vecRagdollOrigin);
	
	// Gore
	CNetworkVar(int, m_iGoreHead);
	CNetworkVar(int, m_iGoreLeftArm);
	CNetworkVar(int, m_iGoreRightArm);
	CNetworkVar(int, m_iGoreLeftLeg);
	CNetworkVar(int, m_iGoreRightLeg);
};
#endif // FR_CLIENT