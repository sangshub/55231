#pragma once;

#include "Include.h"

class CObj;
class CTile;
class CDevice;
class CBufferMgr;
class CTimeMgr;
class CParticleMgr;
class CFrustum;
class CBeatMgr;
class CSceneMgr;

class CObjMgr
{
public:
	DECLARE_SINGLETON(CObjMgr)

private:
	CObjMgr();

public:
	~CObjMgr();

public:
	void GoldDrop(D3DXVECTOR3 vPos, int iCnt = 0);
	const void* GetVtxPointer(const OBJID& eObjId);
	CObj* GetObj(const OBJID& eObjId);
	list<CObj*>* GetObjList(const OBJID& eObjid);
	CObj* GetPlayer();
	CTile* GetTile(const int iIndex);
	const int GetTileX() const;
	const int GetTileZ() const;
	const int GetIndex(const D3DXVECTOR3& vPos) const;
	vector<CObj*> GetFullTile() const;
	const bool GetActionCamSwitch() const;
	const bool GetDarknessSwitch() const;
	void SetJump(const bool bJump);
	void SetStageEnd(const bool bStageEnd);
	void SetStageStart(const bool bStageStart);
	void SetActionCam(const bool bActionCam);
	void SetDarknessSwitch(const bool bDarkness);
	void SetTile(CObj* pObj);
	void SetTileSize(const int iTileX, const int iTileZ);
	void SetTwinkle(const bool bTwinkle);
	void SetTorchDistance(const int iTorchDis);
	void ResetRcCol();
	void ResetRcTex();
	void ResetUIVtx();
	void ResetCubeTex();
	const bool GetDeadCheck() const;
	const bool GetIsLight() const;
	void SetIsLight(const bool bLight);
	void SetObjectStageEnd(CObj* pObj);

public:
	void TileLightOfSight();
	void ChangeStage();

public:
	void AddObj(const OBJID& eObjId, CObj* pObj);

public:
	HRESULT Initialize();
	void Progress();
	void Render();

private:
	void Release();

private:
	void TileCulling();
	void CubeCulling();
	void Twinkle();
	bool CheckOBBCollision(OBB* Box1, OBB* Box2);

private:
	bool m_bDarkness;
	bool m_bActionCam;
	bool m_bTwinkle;
	bool m_bLight;
	bool m_bDeadCheck;
	bool m_bJump;
	bool m_bStageEnd;
	bool m_bStageStart;

	int m_iTileX;
	int m_iTileZ;
	int m_iTorchDis;
	float m_fTime;
	float m_fLightDis;

	VTXCOL* m_pOriRcCol;
	VTXTEX* m_pOriRcTex;
	VTXTEX* m_pOriUIVtx;
	VTXCUBE* m_pOriCubeTex;

	CDevice*  m_pDevice;
	CBufferMgr* m_pBufferMgr;
	CTimeMgr* m_pTimeMgr;
	CFrustum* m_pFrustum;
	CBeatMgr* m_pBeatMgr;
	CParticleMgr* m_pParticleMgr;
	CSceneMgr*	m_pSceneMgr;
	
	list<CObj*> m_ObjList[OBJ_END];
	// 컬링용 컨테이너들
	list<CObj*> m_TileList;
	list<CObj*> m_CubeList;
	vector<CObj*> m_vecFullTile;
};