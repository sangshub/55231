#include "stdafx.h"
#include "MainCamObj.h"
#include "TimeMgr.h"
#include "MainCamMgr.h"
#include "Pipeline.h"
#include "Frustum.h"
#include "ToolView.h"
#include "MainFrm.h"

CMainCamObj::CMainCamObj()
: m_fCamDistance(30.f)
, m_fCamAngle(D3DXToRadian(60.f))
, m_pFrustum(CFrustum::GetInstance())
{
}

CMainCamObj::~CMainCamObj()
{
	::Safe_Destroy(m_pMainCamMgr);
	::Safe_Destroy(m_pFrustum);
}

HRESULT CMainCamObj::Initialize()
{
	m_pMainView = ((CMainFrame*)AfxGetMainWnd())->m_pMainView;
	m_pMainCamMgr->InitCamera();

	m_fSpeed = 20.f;

	return S_OK;
}

int CMainCamObj::Progress()
{
	SetDirection();
	SetCamera();
	KeyInput();

	SetMatrix();

	return 0;
}

void CMainCamObj::Render()
{
}

void CMainCamObj::SetMatrix()
{
	CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(1.f, 1.f, 1.f), m_fAngle, m_tInfo.vPos);
}

void CMainCamObj::SetCamera()
{
	D3DXVECTOR3 vCamPos = m_tInfo.vDir * -1.f;
	D3DXVec3Normalize(&vCamPos, &vCamPos);
	vCamPos *= m_fCamDistance;

	D3DXVECTOR3 vRight = D3DXVECTOR3(0.f, 0.f, 0.f);
	memcpy(&vRight, &m_tInfo.matWorld.m[0][0], sizeof(float) * 3);
	D3DXVec3Normalize(&vRight, &vRight);

	D3DXMATRIX matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, m_fCamAngle);

	D3DXVec3TransformNormal(&vCamPos, &vCamPos, &matRotAxis);

	vCamPos += m_tInfo.vPos;

	m_pMainCamMgr->SetCamPosition(vCamPos, m_tInfo.vPos, D3DXVECTOR3(0.f, 1.f, 0.f));

	m_pFrustum->SetFrustum();
}

void CMainCamObj::KeyInput()
{
	if(GetFocus() == m_pMainView->m_hWnd)
	{
		if(GetAsyncKeyState(VK_UP) & 0x8000)
			m_tInfo.vPos.z += m_fSpeed * m_pTimeMgr->GetTime();
		if(GetAsyncKeyState(VK_DOWN) & 0x8000)
			m_tInfo.vPos.z -= m_fSpeed * m_pTimeMgr->GetTime();
		if(GetAsyncKeyState(VK_LEFT) & 0x8000)
			m_tInfo.vPos.x -= m_fSpeed * m_pTimeMgr->GetTime();
		if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
			m_tInfo.vPos.x += m_fSpeed * m_pTimeMgr->GetTime();
		if(GetAsyncKeyState('W') & 0x8000)
			m_fCamDistance -= 5.f * m_pTimeMgr->GetTime();
		if(GetAsyncKeyState('S') & 0x8000)
			m_fCamDistance += 5.f * m_pTimeMgr->GetTime();
		if(GetAsyncKeyState('Q') & 0x8000)
			m_fCamAngle += D3DXToRadian(120.f) * m_pTimeMgr->GetTime();
		if(GetAsyncKeyState('E') & 0x8000)
			m_fCamAngle -= D3DXToRadian(120.f) * m_pTimeMgr->GetTime();
		if(GetAsyncKeyState('A') & 0x8000)
			m_fAngle[ANGLE_Z] += D3DXToRadian(120.f) * m_pTimeMgr->GetTime();
		if(GetAsyncKeyState('D') & 0x8000)
			m_fAngle[ANGLE_Z] -= D3DXToRadian(120.f) * m_pTimeMgr->GetTime();
	}
}