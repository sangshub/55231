#pragma once

#include "Include.h"

class CDevice;
class CTimeMgr;
class CBufferMgr;
class CTextureMgr;
class CKeyMgr;
class CCamMgr;
class CMainCamMgr;

class CObj
{
protected:
	bool m_bIsDelete;
	DWORD m_dwVtxCnt;
	DWORD m_dwIndexCnt;
	float m_fSpeed;
	float m_fAngle[ANGLE_END];

	CDevice*		m_pDevice;
	CTimeMgr*		m_pTimeMgr;
	CBufferMgr*		m_pBufferMgr;
	CKeyMgr*		m_pKeyMgr;
	CTextureMgr*	m_pTextureMgr;
	CCamMgr*		m_pCamMgr;
	CMainCamMgr*	m_pMainCamMgr;

	VTXTEX* m_pVtxTex;
	VTXTEX* m_pConvertVtxTex;

	VTXCUBE* m_pCubeTex;
	VTXCUBE* m_pConvertCubeTex;
	
	INDEX* m_pIndex;

	wstring		m_wstrStateKey;
	wstring		m_wstrObjKey;

	INFO m_tInfo;

public:
	const void* GetVtxPointer() const;
	const INFO* GetInfo() const;
	const DWORD GetVtxCnt() const;
	const INDEX* GetIndex() const;
	const DWORD GetIndexCnt() const;
	const wstring GetStatekey();
	const wstring GetObjkey();

	void SetPos(const D3DXVECTOR3& vPos);
	void SetStateKey(wstring wstrStateKey);
	void SetObjKey(wstring wstrObjKey);
	void SetVtxTex(VTXTEX* pVtxTex);
	void SetIsDelete();

public: // 애니메이션 관련
	bool				m_bAniPlay; // 
	size_t				m_iSceneCnt; // 애니씬 카운트용
	float				m_fSceneTime;
	D3DXMATRIX			m_matChild;
	D3DXMATRIX*			m_matParents; // 부모행렬
	vector<ANI*>		m_vecAni; // 실험용
	bool AnimationPlayData(ANI* pAni1, ANI* pAni2);
	float Linear( float v0, float v1, float fTime ,float t );
	void AniPlay(void);
	void ReleaseAnimation(void);
	D3DXMATRIX* GetParentMatrix(void);
	void SetAniPlay(bool _bAniPlay);
	void SetAniData(const ANI& tAni);

public:
	virtual HRESULT Initialize()PURE;
	virtual int Progress()PURE;
	virtual void Render()PURE;

protected:
	void Release();

protected:
	void BeginRenderState();
	void EndRenderState();

private:
	virtual void SetMatrix()PURE;

protected:
	void SetDirection();

public:
	CObj();
	virtual ~CObj();
};