#include "StdAfx.h"
#include "RectEffect.h"
#include "BufferMgr.h"
#include "Device.h"
#include "Pipeline.h"
#include "CamMgr.h"
#include "Rand.h"
#include "TextureMgr.h"
#include "TimeMgr.h"

CRectEffect::CRectEffect(void)
{
}

CRectEffect::~CRectEffect(void)
{
}

HRESULT CRectEffect::Initialize(void)
{
	m_dwVtxCnt = 4;
	m_dwIndexCnt = 2;

	m_pVtxCol = new VTXCOL[m_dwVtxCnt];
	m_pConvertVtxCol = new VTXCOL[m_dwVtxCnt];
	m_pIndex = new INDEX[m_dwIndexCnt];

	m_pBufferMgr->GetVtxInfo(TEXT("RcCol"), m_pVtxCol);
	m_pBufferMgr->GetIndex(TEXT("RcCol"), m_pIndex);

	ZeroMemory(m_fAngle, sizeof(float) * 3);

	m_eObjId = OBJ_ETC;

	return S_OK;
}

const int CRectEffect::Progress(void)
{
	SetActive();
	SetMatrix();

	m_fTime += GETTIME;
	m_fGravityTime += GETTIME;
	if(m_fTime > m_fMaxTime)
		return 1;

	return 0;
}

void CRectEffect::Render(void)
{
	m_pBufferMgr->SetVtxInfo(TEXT("RcCol"), m_pConvertVtxCol);
	m_pBufferMgr->RenderBuffer(TEXT("RcCol"));

}
void CRectEffect::SetMatrix(const float fScale)
{
	D3DXMATRIX matView;
	D3DXMATRIX matProj;

	CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(m_fScale, m_fScale, m_fScale), m_fAngle, m_tInfo.vPos);

	SetBillBoard();

	m_pCamMgr->GetMatrix(D3DTS_VIEW, matView);
	m_pCamMgr->GetMatrix(D3DTS_PROJECTION, matProj);

	memcpy(m_pConvertVtxCol, m_pVtxCol, sizeof(VTXCOL) * m_dwVtxCnt);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		CPipeline::MyTransformCoord(m_pConvertVtxCol[i].vPos, m_pConvertVtxCol[i].vPos, m_tInfo.matWorld);
		CPipeline::MyTransformCoord(m_pConvertVtxCol[i].vPos, m_pConvertVtxCol[i].vPos, matView);

		if(m_pConvertVtxCol[i].vPos.z < 1.f)
			m_pConvertVtxCol[i].vPos.z = 1.f;

		CPipeline::MyTransformCoord(m_pConvertVtxCol[i].vPos, m_pConvertVtxCol[i].vPos, matProj);
	}
}

void CRectEffect::RectSetting(const D3DXVECTOR3& vPos
							  , const float fScale
							  , const int	iSpeed //입자 속도
							  , const int	iMaxSpeed //입자 최대 속도
							  , const float fElasticity
							  , const float fTime
							  , const float fUpPower
							  , const DWORD dwDrawId
							  , const D3DXVECTOR3& vDamageDir // 데미지 받은 방향
							  , const bool bGravity
							  , const bool bSizeRnd // 크기랜덤
							  )
{
	m_tInfo.vPos = vPos;
	m_tInfo.vDir = D3DXVECTOR3((float)CRand::Random(-10, 10), (float)CRand::Random(-10, 10), (float)CRand::Random(-10, 10));
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	m_fSpeed = CRand::Random(iSpeed,iMaxSpeed) * 0.1f;
	m_fElasticity = fElasticity;
	m_fUpPower = fUpPower;
	m_fTime = 0.f;
	m_fMaxTime = fTime;
	m_fGravityTime = 0.f;
	m_vDamageDir = vDamageDir;
	m_bGravity = bGravity;
	m_bDown = false;
	m_bSizeRnd = bSizeRnd;

	for(int i = 0; i < 4; ++i)
		m_pVtxCol[i].dwColor = dwDrawId;

	if(bSizeRnd)
		m_fScale = CRand::Random(0, int(fScale * 100.f)) * 0.01f;
	else
		m_fScale = fScale;
}

void CRectEffect::SetActive()
{
	float fGravitySpeed = 0.f;
	m_tInfo.vPos += m_tInfo.vDir * (m_fSpeed * GETTIME);

	if(m_bGravity)
	{
		fGravitySpeed = m_fUpPower + (-5.f * m_fGravityTime);
		m_tInfo.vPos.y += fGravitySpeed;

		if(fGravitySpeed < 0)
			m_bDown = true;

		if(m_bDown && m_tInfo.vPos.y <= 0.1f)
		{
			m_fSpeed *= m_fElasticity;
			m_fUpPower *= m_fElasticity;
			if(m_fUpPower < 0.01f)
				m_tInfo.vPos.y = 0.1f;
			m_fGravityTime = 0.f;
			m_bDown = false;
		}
		else
			m_tInfo.vPos += m_vDamageDir * 0.15f;
	}
}