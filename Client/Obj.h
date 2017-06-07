#pragma once

#include "Include.h"

class CDevice;
class CTimeMgr;
class CBufferMgr;
class CTextureMgr;
class CCamMgr;
class CObjMgr;
class CBeatMgr;
class CKeyMgr;
class CCube;
class CTile;
class CParticleMgr;
class CComboMgr;
class CFrozen;
class CSceneMgr;

class CObj
{
public:
	CObj(void);
	virtual ~CObj(void);

public:
	virtual HRESULT Initialize(void)PURE;
	virtual const int Progress(void)PURE;
	virtual void Render(void)PURE;

protected:
	void Release(void);

public:
	const float GetAngleY() const;
	const INFO* GetInfo() const;
	const UNIT*	GetUnitInfo() const;
	void* GetVtxPointer() const;
	const DWORD GetVtxCnt() const;
	INDEX* GetIndex() const;
	const DWORD GetIndexCnt() const;
	const wstring GetStatekey() const;
	const wstring GetObjkey() const;
	const OBJID GetObjId() const;
	const CUBE&	GetCubeInfo() const;
	const BYTE GetDarkness() const;

	void SetUnitInfo(const UNIT& tUnit);
	void SetMegaJump(const bool bJump);
	void SetPos(const D3DXVECTOR3& vPos);
	void SetObjKey(const wstring& wstrObjKey);
	void SetStateKey(const wstring& wstrStateKey);
	void SetAngle(const float fAngle);
	void SetDamage(const int iAtt);
	void SetBeatMax(const float Max);
	void SetDelete();
	void SetFrozen(const bool bFreeze);
	void SetFrozenTimer(void);
	void SetStructFrame(const FRAME& tFrame);
	void SetDark(const BYTE iDarkness);
	void SetDarkness(const BYTE byDarkness);
	void SetStageEnd(const bool bStageEnd);
	void SetStageStart(const bool bStageStart);

private:
	virtual void SetMatrix(const float fScale = 1.f)PURE;

protected:
	void StageEndScene(void);
	void BoxMove(const float fTime); // �����̴� �ڽ� �����̱�
	void MegaJump(); // ����ü ����
	void SetDirection();
	void SetFrame(const float fSpeed = 1.f);
	void FrameMove(const bool bLoop = true);
	void BeginRenderState(const BYTE byAlpha = 255);
	void EndRenderState();
	void SetBillBoard();
	void GetPlayerToDistance();

public: // �ִϸ��̼� ����////////////////////////////////
	STATEKEY			m_eStatKey;
	STATEKEY			m_eAniStateKey;
	int					m_iChildCnt;
	float				m_fSceneTime;
	float				m_fAniFullTime;
	size_t				m_iSceneCnt; // �ִϾ� ī��Ʈ��
	D3DXMATRIX			m_matLocal;
	D3DXMATRIX			m_matParents;
	D3DXMATRIX*			m_pmatParents; // �θ����
	vector<CCube*>		m_vecChild; // �ڽ� ������Ʈ
	vector<ANI*>		m_vecAni[STAT_END];

public:
	CUBE					m_tCube; // ����뵵
	LPDIRECT3DCUBETEXTURE9	m_pCubeTexture;
	
	const float Linear(const float v0, const float v1, const float fTime ,const float t);
	void AniPlay(const STATEKEY& eStatKey);
	void BossAniPlay(const STATEKEY& eStatKey);
	void ConvertAniData(ANI& tAni, const CUBEDATA& tCubeData);
	void ReleaseAnimation(void);
	const bool AnimationPlayData(ANI* pAni1, ANI* pAni2);
	void LoadObject(const wstring& FilePath);
	void LoadAnimation(const wstring& FilePath, STATEKEY eStatKey);
	D3DXMATRIX* GetParentMatrix(void);
	void SetCubeTexture(const int iDrawId);
	void SetParentsMatrix(D3DXMATRIX* _pmatParents);
	void SetLocalMatrix(const CUBEDATA* pCubeData);
	void SetAniData(const CUBEDATA& tCubeData, const STATEKEY& eStatKey);
	void SetAniStateKey(const STATEKEY& eStatKey);
	
	////////////////////////////////////////////////////////
protected:
	bool	m_bIsDelete;
	bool	m_bFreeze;
	bool	m_bMegaJump;
	bool	m_bBoxMove;
	bool	m_bStageEnd; // �������� �����
	bool	m_bStageStart;
	DWORD	m_dwVtxCnt;
	DWORD	m_dwIndexCnt;
	DWORD	m_dwState; // ���� Ű��
	int		m_iCommenCnt;
	BYTE	m_iDarkness;
	float	m_fFullTime;
	float	m_fMegaY; /////
	float	m_fMegaTime;///�� ��ü ���� ����
	float	m_fMegaPow;////
	float	m_fFreezeTime;
	float	m_fLightCnt; // �������� ���� ���� ����
	float	m_fSpeed;
	float	m_fAngle[ANGLE_END];
	float   m_fGravity;
	D3DXVECTOR3 m_vOriPos;

	CDevice* m_pDevice;
	CTimeMgr* m_pTimeMgr;
	CBufferMgr* m_pBufferMgr;
	CTextureMgr* m_pTextureMgr;
	CCamMgr* m_pCamMgr;
	CObjMgr* m_pObjMgr;
	CBeatMgr* m_pBeatMgr;
	CKeyMgr* m_pKeyMgr;
	CParticleMgr* m_pPaticleMgr;
	CComboMgr*	m_pComboMgr;
	CSceneMgr*	m_pSceneMgr;

	VTXCOL* m_pVtxCol;
	VTXCOL* m_pConvertVtxCol;
	VTXTEX* m_pVtxTex;
	VTXTEX* m_pConvertVtxTex;
	VTXCUBE* m_pCubeTex;
	VTXCUBE* m_pConvertCubeTex;
	INDEX* m_pIndex;
	CFrozen* m_pFrozen;

	//���� ���� ����
	D3DXVECTOR3*	m_pvCamDir; //�÷��̾ ������ �ִ� ķ�𷺼�
	CTile*			m_pTile;// Ÿ�� Ȯ�ο�

	OBJID m_eObjId;
	INFO m_tInfo;
	UNIT m_tUnitInfo; // ���ֵ� ���°�

	wstring m_wstrObjKey;
	wstring m_wstrStateKey;

	FRAME			m_tFrame;

public:		//OBB�̿�
	void UsingOBB(void);
	
	OBB  m_tOBB;

	OBB GetObjOBB(void)
	{
		return m_tOBB;
	}
};