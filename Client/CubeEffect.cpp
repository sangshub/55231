#include "StdAfx.h"
#include "CubeEffect.h"
#include "BufferMgr.h"
#include "Device.h"
#include "Pipeline.h"
#include "CamMgr.h"
#include "Rand.h"
#include "TextureMgr.h"
#include "TimeMgr.h"

CCubeEffect::CCubeEffect(void)
{
}

CCubeEffect::~CCubeEffect(void)
{
}

HRESULT CCubeEffect::Initialize(void)
{
	m_dwVtxCnt = 8;
	m_dwIndexCnt = 12;

	m_pCubeTex = new VTXCUBE[m_dwVtxCnt];
	m_pConvertCubeTex = new VTXCUBE[m_dwVtxCnt];
	m_pIndex = new INDEX[m_dwIndexCnt];

	m_pBufferMgr->GetVtxInfo(TEXT("CubeTex"), m_pCubeTex);
	m_pBufferMgr->GetIndex(TEXT("CubeTex"), m_pIndex);

	ZeroMemory(m_fAngle, sizeof(float)*3);
	ZeroMemory(&m_tCube, sizeof(m_tCube));

	m_eObjId = OBJ_ETC;

	return S_OK;
}

const int CCubeEffect::Progress(void)
{
	SetActive();
	SetMatrix();

	m_fTime += GETTIME;
	m_fGravityTime += GETTIME;
	if(m_fTime > m_fMaxTime)
		return 1;

	return 0;
}

void CCubeEffect::Render(void)
{
	m_pDevice->GetDevice()->SetTexture(0, m_pCubeTexture);
	m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pConvertCubeTex);
	m_pBufferMgr->RenderBuffer(TEXT("CubeTex"));
	m_pDevice->GetDevice()->SetTexture(0, NULL);
}

void CCubeEffect::SetMatrix(const float fScale)
{
	D3DXMATRIX matView;
	D3DXMATRIX matProj;

	CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(m_fScale, m_fScale, m_fScale), m_fAngle, m_tInfo.vPos);

	m_pCamMgr->GetMatrix(D3DTS_VIEW, matView);
	m_pCamMgr->GetMatrix(D3DTS_PROJECTION, matProj);

	memcpy(m_pConvertCubeTex, m_pCubeTex, sizeof(VTXCUBE) * m_dwVtxCnt);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		CPipeline::MyTransformCoord(m_pConvertCubeTex[i].vPos, m_pConvertCubeTex[i].vPos, m_tInfo.matWorld);
		CPipeline::MyTransformCoord(m_pConvertCubeTex[i].vPos, m_pConvertCubeTex[i].vPos, matView);

		if(m_pConvertCubeTex[i].vPos.z < 1.f)
			m_pConvertCubeTex[i].vPos.z = 1.f;

		CPipeline::MyTransformCoord(m_pConvertCubeTex[i].vPos, m_pConvertCubeTex[i].vPos, matProj);
	}
}

void CCubeEffect::CubeSetting(const D3DXVECTOR3& vPos
							  , const float fScale
							  , const int	iSpeed //입자 속도
							  , const int	iMaxSpeed //입자 최대 속도
							  , const float fElasticity
							  , const float fTime
							  , const float fUpPower
							  , const int iDrawId
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
	m_pCubeTexture = m_pTextureMgr->GetCubeTex(iDrawId);
	m_vDamageDir = vDamageDir;
	m_bGravity = bGravity;
	m_bDown = false;

	if(bSizeRnd)
		m_fScale = CRand::Random(0, int(fScale * 100.f)) * 0.01f;
	else
		m_fScale = fScale;
}

void CCubeEffect::SetActive()
{
	float fGravitySpeed = 0.f;
	m_tInfo.vPos += m_tInfo.vDir * (m_fSpeed * GETTIME);

	if(m_bGravity)
	{
		fGravitySpeed = m_fUpPower + (-5.f * m_fGravityTime);
		m_tInfo.vPos.y += fGravitySpeed;
		m_tInfo.vPos += m_vDamageDir * 0.3f;

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