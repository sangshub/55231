#include "stdafx.h"
#include "Obj.h"
#include "Device.h"
#include "TimeMgr.h"
#include "BufferMgr.h"
#include "CamMgr.h"
#include "MainCamMgr.h"
#include "TextureMgr.h"
#include "Pipeline.h"
#include "KeyMgr.h"

CObj::CObj()
: m_bIsDelete(false)
, m_dwVtxCnt(0)
, m_dwIndexCnt(0)
, m_fSpeed(0.f)
, m_iSceneCnt(0)
, m_fSceneTime(0.f)
, m_pDevice(CDevice::GetInstance())
, m_pTimeMgr(CTimeMgr::GetInstance())
, m_pBufferMgr(CBufferMgr::GetInstance())
, m_pTextureMgr(CTextureMgr::GetInstance())
, m_pCamMgr(CCamMgr::GetInstance())
, m_pMainCamMgr(CMainCamMgr::GetInstance())
, m_pKeyMgr(CKeyMgr::GetInstance())
, m_pVtxTex(NULL)
, m_pConvertVtxTex(NULL)
, m_pCubeTex(NULL)
, m_pConvertCubeTex(NULL)
, m_pIndex(NULL)
, m_matParents(NULL)
, m_bAniPlay(false)
{
	ZeroMemory(m_fAngle, sizeof(float) * ANGLE_END);
	ZeroMemory(&m_tInfo, sizeof(INFO));
	m_wstrObjKey = L"";
	m_wstrStateKey = L"";

	D3DXMatrixIdentity(&m_tInfo.matWorld);
}

CObj::~CObj()
{
	Release();
}

void CObj::Release()
{
	::Safe_Delete_Array(m_pVtxTex);
	::Safe_Delete_Array(m_pConvertVtxTex);
	::Safe_Delete_Array(m_pCubeTex);
	::Safe_Delete_Array(m_pConvertCubeTex);
	::Safe_Delete_Array(m_pIndex);

	for_each(m_vecAni.begin(), m_vecAni.end(), CDeleteObj());
}

void CObj::SetPos(const D3DXVECTOR3& vPos)
{
	m_tInfo.vPos = vPos;
}

const void* CObj::GetVtxPointer() const
{
	if(m_pVtxTex)
		return m_pVtxTex;
	else if(m_pConvertVtxTex)
		return m_pConvertVtxTex;
	else if(m_pCubeTex)
		return m_pCubeTex;
	else if(m_pConvertCubeTex)
		return m_pConvertCubeTex;
	else 
		return NULL;
}

const INFO* CObj::GetInfo() const
{
	return &m_tInfo;
}

const DWORD CObj::GetVtxCnt() const
{
	return m_dwVtxCnt;
}

const INDEX* CObj::GetIndex() const
{
	return m_pIndex;
}

const DWORD CObj::GetIndexCnt() const
{
	return m_dwIndexCnt;
}

void CObj::SetDirection()
{
	D3DXVec3TransformNormal(&m_tInfo.vDir, &g_vLook, &m_tInfo.matWorld);
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
}

const wstring CObj::GetObjkey()
{
	return m_wstrObjKey;
}

const wstring CObj::GetStatekey()
{
	return m_wstrStateKey;
}

void CObj::SetObjKey(wstring wstrObjKey)
{
	m_wstrObjKey = wstrObjKey;
}

void CObj::SetVtxTex(VTXTEX* pVtxTex)
{
	memcpy(pVtxTex,m_pVtxTex,4 * sizeof(VTXTEX));
}

void CObj::BeginRenderState()
{
	m_pDevice->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->GetDevice()->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

void CObj::EndRenderState()
{
	m_pDevice->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pDevice->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CObj::AniPlay(void)
{
	if(m_bAniPlay && m_vecAni.size() > 1)
	{
		if(AnimationPlayData(m_vecAni[m_iSceneCnt], m_vecAni[m_iSceneCnt+1]))
		{
			++m_iSceneCnt;
			if(m_iSceneCnt+1 >= m_vecAni.size())
				m_iSceneCnt = 0;
		}
	}
}

bool CObj::AnimationPlayData(ANI* pAni1, ANI* pAni2)
{
	D3DXMATRIX matTrans, matScale, matRot;
	D3DXVECTOR3 vResult; // 이동값 백터
	D3DXVECTOR3 vReScale; // 스케일값 백터
	D3DXQUATERNION Quat;

	if( m_fSceneTime > pAni1->fTime )
	{
		m_fSceneTime = 0.0f;
		return true;
	}

	/// 위치값의 선형보간
	vResult.x = Linear(pAni1->vPos.x, pAni2->vPos.x, pAni1->fTime, m_fSceneTime);
	vResult.y = Linear(pAni1->vPos.y, pAni2->vPos.y, pAni1->fTime, m_fSceneTime);
	vResult.z = Linear(pAni1->vPos.z, pAni2->vPos.z, pAni1->fTime, m_fSceneTime);
	D3DXMatrixTranslation( &matTrans, vResult.x, vResult.y, vResult.z );	/// 이동행렬을 구한다.

	/// 스케일값의 선형보간
	vReScale.x = Linear(pAni1->vScale.x, pAni2->vScale.x, pAni1->fTime, m_fSceneTime);
	vReScale.y = Linear(pAni1->vScale.y, pAni2->vScale.y, pAni1->fTime, m_fSceneTime);
	vReScale.z = Linear(pAni1->vScale.z, pAni2->vScale.z, pAni1->fTime, m_fSceneTime);
	D3DXMatrixScaling( &matScale, vReScale.x, vReScale.y, vReScale.z );	/// 크기행렬을 구한다.

	/// 회전값의 구면선형보간
	D3DXQuaternionSlerp( &Quat, &pAni1->qRot, &pAni2->qRot, m_fSceneTime/pAni1->fTime);
	D3DXMatrixRotationQuaternion( &matRot, &Quat );	/// 사원수를 회전행렬값으로 변환

	m_tInfo.matWorld = matScale*matRot*matTrans;
	m_matChild = matRot*matTrans;

	m_fSceneTime += GETTIME;

	return false;
}

float CObj:: Linear( float v0, float v1, float fTime ,float t )
{
	return v0 * (1.f - (t/fTime)) + v1 * (t/fTime);
}

void CObj::SetAniData(const ANI& tAni)
{
	ANI* pAni = new ANI;

	pAni->fTime = tAni.fTime;
	pAni->qRot = tAni.qRot;
	pAni->vPos = tAni.vPos;
	pAni->vScale = tAni.vScale;

	m_vecAni.push_back(pAni);
}

void CObj::SetAniPlay(bool _bAniPlay)
{
	m_bAniPlay = _bAniPlay;

	if(!m_bAniPlay)
	{
		m_fSceneTime = 0.f;
		m_iSceneCnt = 0;
	}
}

void CObj::ReleaseAnimation(void)
{
	for_each(m_vecAni.begin(), m_vecAni.end(), CDeleteObj());
	m_vecAni.clear();
}

D3DXMATRIX* CObj::GetParentMatrix(void)
{
	return &m_matChild;
}

void CObj::SetIsDelete()
{
	m_bIsDelete = true;
}