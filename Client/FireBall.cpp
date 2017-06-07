#include "stdafx.h"
#include "FireBall.h"
#include "FireBallStart.h"
#include "FireBallEnd.h"
#include "Player.h"
#include "Pipeline.h"
#include "CamMgr.h"
#include "ObjMgr.h"
#include "ObjFactory.h"

CFireBall::CFireBall()
: m_bInit(false)
, m_bLeft(true)
, m_iScreenMode(0)
{
}

CFireBall::~CFireBall()
{
	Release();
}

HRESULT CFireBall::Initialize()
{
	m_wstrObjKey = TEXT("FireBall");

	m_vecDamageTile.reserve(4);

	return S_OK;
}

const int CFireBall::Progress()
{
	if(!m_bInit)
	{
		CFireBallStart* pRightStart = (CFireBallStart*)CObjFactory<CFireBallStart>::CreateObj(m_tInfo.vPos);
		CFireBallStart* pLeftStart = (CFireBallStart*)CObjFactory<CFireBallStart>::CreateObj(m_tInfo.vPos);

		pLeftStart->m_bLeft = false;

		m_pObjMgr->AddObj(OBJ_EFFECT, pRightStart);
		m_pObjMgr->AddObj(OBJ_EFFECT, pLeftStart);

		CFireBallEnd* pRightEnd = (CFireBallEnd*)CObjFactory<CFireBallEnd>::CreateObj(m_tInfo.vPos);
		CFireBallEnd* pLeftEnd = (CFireBallEnd*)CObjFactory<CFireBallEnd>::CreateObj(m_tInfo.vPos);

		pLeftEnd->m_bLeft = false;

		m_pObjMgr->AddObj(OBJ_EFFECT, pRightEnd);
		m_pObjMgr->AddObj(OBJ_EFFECT, pLeftEnd);

		m_bInit = true;
	}

	return 1;
}

void CFireBall::Render()
{
}

void CFireBall::Release()
{
	vector<int>().swap(m_vecDamageTile);
}

void CFireBall::SetMatrix()
{
	switch(m_iScreenMode)
	{
	case 0:
		if(m_bLeft)
			CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(-5.f, 1.f, 1.f), m_fAngle, m_tInfo.vPos);
		else
			CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(5.f, 1.f, 1.f), m_fAngle, m_tInfo.vPos);
		break;
	case 1:
		if(m_bLeft)
			CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(1.f, 1.f, -5.f), m_fAngle, m_tInfo.vPos);
		else
			CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(1.f, 1.f, 5.f), m_fAngle, m_tInfo.vPos);
		break;
	case 2:
		if(m_bLeft)
			CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(5.f, 1.f, 1.f), m_fAngle, m_tInfo.vPos);
		else
			CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(-5.f, 1.f, 1.f), m_fAngle, m_tInfo.vPos);
		break;
	case 3:
		if(m_bLeft)
			CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(1.f, 1.f, 5.f), m_fAngle, m_tInfo.vPos);
		else
			CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(1.f, 1.f, -5.f), m_fAngle, m_tInfo.vPos);
		break;
	}

	SetBillBoard();

	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	m_pCamMgr->GetMatrix(D3DTS_VIEW, matView);
	m_pCamMgr->GetMatrix(D3DTS_PROJECTION, matProj);

	memcpy(m_pConvertVtxTex, m_pVtxTex, sizeof(VTXTEX) * m_dwVtxCnt);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		//m_pConvertVtxTex[i] = m_pVtxTex[i];

		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, m_tInfo.matWorld);
		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, matView);

		if(m_pConvertVtxTex[i].vPos.z < 1.f)
			m_pConvertVtxTex[i].vPos.z = 1.f;

		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, matProj);
	}
}

void CFireBall::SetScreenMode()
{
	float fAngle = D3DXToDegree(((CPlayer*)m_pObjMgr->GetPlayer())->GetCamAngleY());

	if(fAngle >= 360.f)
	{
		int i = int(fAngle / 360.f);

		fAngle = fAngle - 360.f * i;
	}

	if(fAngle < 0.f)
	{
		int i = int(-fAngle / 360.f);

		fAngle = 360.f * (i + 1) + fAngle;
	}

	if(fAngle == 360.f)
		fAngle = 0.f;

	if(fAngle >= 0.f && fAngle < 90.f)
		m_iScreenMode = 0;
	else if(fAngle >= 90.f && fAngle < 180.f)
		m_iScreenMode = 1;
	else if(fAngle >= 180.f && fAngle < 270.f)
		m_iScreenMode = 2;
	else if(fAngle >= 270.f && fAngle < 360.f)
		m_iScreenMode = 3;
	else
		m_iScreenMode = 0;
}