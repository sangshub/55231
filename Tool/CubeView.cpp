// CubeView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "CubeView.h"
#include "Device.h"
#include "CamMgr.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "ObjFactory.h"
#include "Cube.h"
#include "MathMgr.h"
#include "TimeMgr.h"


// CCubeView

IMPLEMENT_DYNCREATE(CCubeView, CView)

CCubeView::CCubeView()
:m_bSwitch(false)
, m_pCube(NULL)
{
}

CCubeView::~CCubeView()
{
}

BEGIN_MESSAGE_MAP(CCubeView, CView)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEACTIVATE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CCubeView �׸����Դϴ�.

void CCubeView::OnDraw(CDC* /*pDC*/)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.

	m_pCube->Progress();
	CameraMove();
	SetCam();

	m_pDevice->Render_Begin();
	m_pCube->Render();
	m_pDevice->Render_End(m_hWnd);
}


// CCubeView �����Դϴ�.

#ifdef _DEBUG
void CCubeView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CCubeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCubeView �޽��� ó�����Դϴ�.

void CCubeView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_pDevice = CDevice::GetInstance();
	m_pCamMgr = CCamMgr::GetInstance();

	m_pCubeTool = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->GetCubeTool();
	m_pCubeTool->SetCubeView(this);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pCamMgr->SetProjection(D3DXToRadian(45.f), float(WINCX) / WINCY, 1.f, 1000.f);

	m_fCamDistance = 10.f;
	ZeroMemory(m_fCamAngle, sizeof(float)*3);
	m_fCamAngle[ANGLE_X] = D3DXToRadian(45.f);
	m_vCamPos = D3DXVECTOR3(0.f, 0.f, -1.f);
	
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}

BOOL CCubeView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return true;
}

int CCubeView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return MA_ACTIVATE;
}

void CCubeView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	::Safe_Delete(m_pCube);
}

 void CCubeView::SetCam(void)
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

	m_pCamMgr->SetCamPosition(m_vCamPos, D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0,1,0));
}

void CCubeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CameraMove();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCubeView::CameraMove(void)
{
	if(GetAsyncKeyState(VK_NUMPAD4))
		m_fCamAngle[ANGLE_Y] += D3DXToRadian(45.f) * GETTIME;
	if(GetAsyncKeyState(VK_NUMPAD6))
		m_fCamAngle[ANGLE_Y] += D3DXToRadian(-45.f) * GETTIME;
}

void CCubeView::SetCubeData(const CUBE& tCube)
{
	m_pCube->SetCubeInfo(tCube);
}

void CCubeView::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	m_pCube->SetToolViewSwitch(false);

	CView::OnClose();
}

void CCubeView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CView::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	if(!m_pCube)
		m_pCube = (CCube*)CObjFactory<CCube>::CreateObj(0);
	m_pCube->SetToolViewSwitch(true);
}
