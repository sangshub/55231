// AniView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "AniView.h"
#include "Cube.h"
#include "Device.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "ObjFactory.h"
#include "CamMgr.h"
#include "MathMgr.h"
#include "TimeMgr.h"
#include "TileMap.h"
#include "Tile.h"

IMPLEMENT_DYNCREATE(CAniView, CView)

CAniView::CAniView()
{
}

CAniView::~CAniView()
{
}

BEGIN_MESSAGE_MAP(CAniView, CView)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEACTIVATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CAniView 그리기입니다.

void CAniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.

	if(!m_pAniTool->KeyInput())
	{
		CameraMove();
		SetCam();
	}

	for(vector<CCube*>::iterator iter = m_pAnimation->begin();
		iter != m_pAnimation->end(); ++iter)
	{
		(*iter)->Progress();
	}
	m_pTile->SetAniMatrix();

	m_pDevice->Render_End();
	m_pDevice->Render_Begin();

	m_pTile->Render();
	for(vector<CCube*>::iterator iter = m_pAnimation->begin();
		iter != m_pAnimation->end(); ++iter)
	{
		(*iter)->Render();
	}

	m_pDevice->Render_End(m_hWnd);
	m_pDevice->Render_Begin();
}


// CAniView 진단입니다.

#ifdef _DEBUG
void CAniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CAniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAniView 메시지 처리기입니다.

void CAniView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_pDevice = CDevice::GetInstance();
	m_pCamMgr = CCamMgr::GetInstance();
	m_pTileMap = CTileMap::GetInstance();

	m_pAniTool = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->GetAniTool();
	m_pAniTool->SetAniView(this);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pCamMgr->SetAniProjection(D3DXToRadian(45.f), float(WINCX) / WINCY, 1.f, 1000.f);

	m_pTileMap->SetResetVtx();
	m_pTile = new CTile;
	m_pTile->Initialize();
	m_pTile->SetDrawId(1);
	m_pTile->SetStateKey(L"Tile");

	m_fCamDistance = 10.f;
	ZeroMemory(m_fCamAngle, sizeof(float)*3);
	m_fCamAngle[ANGLE_X] = D3DXToRadian(45.f);
	m_vCamPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	m_pAnimation = m_pAniTool->GetAnimationCube();
}

BOOL CAniView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;
}

int CAniView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return MA_ACTIVATE;
}

void CAniView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	for_each(m_pAnimation->begin(), m_pAnimation->end(), ::CDeleteObj());
	::Safe_Delete(m_pTile);
}

void CAniView::SetCam(void)
{
	m_vCamPos = D3DXVECTOR3(0.f, 0.f, -1.f);

	m_vCamPos *= m_fCamDistance;

	D3DXVECTOR3		vRight, vLook;
	D3DXMATRIX		matRotAxis;

	CMathMgr::MyRotationY(m_vCamPos, m_vCamPos, m_fCamAngle[ANGLE_Y]);

	vLook = m_vCamPos * -1.f;

	D3DXVec3Cross(&vRight, &D3DXVECTOR3(0.f, 1.f, 0.f), &vLook);

	D3DXMatrixRotationAxis(&matRotAxis, &vRight, m_fCamAngle[ANGLE_X]);

	D3DXVec3TransformNormal(&m_vCamPos, &m_vCamPos, &matRotAxis);

	m_pCamMgr->SetAniCamPosition(m_vCamPos, D3DXVECTOR3(0.f, 1.f, 0.f), D3DXVECTOR3(0,1,0));
}

void CAniView::CameraMove(void)
{
	if(GetAsyncKeyState(VK_NUMPAD4))
		m_fCamAngle[ANGLE_Y] += D3DXToRadian(60.f) * GETTIME;
	if(GetAsyncKeyState(VK_NUMPAD6))
		m_fCamAngle[ANGLE_Y] += D3DXToRadian(-60.f) * GETTIME;
	if(GetAsyncKeyState(VK_NUMPAD8))
		m_fCamAngle[ANGLE_X] += D3DXToRadian(60.f) * GETTIME;
	if(GetAsyncKeyState(VK_NUMPAD5))
		m_fCamAngle[ANGLE_X] += D3DXToRadian(-60.f) * GETTIME;
	if(GetAsyncKeyState(VK_ADD))
		m_fCamDistance -= 10.f * GETTIME;
	if(GetAsyncKeyState(VK_SUBTRACT))
		m_fCamDistance += 10.f * GETTIME;
}
void CAniView::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnClose();
}

void CAniView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CView::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CAniView::SetAniTool(CAniTool* _pAniTool)
{
	m_pAniTool = _pAniTool;
}
