// UnitView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitView.h"
#include "Device.h"
#include "CamMgr.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "ObjFactory.h"
#include "Monster.h"
#include "MathMgr.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
// CUnitView

IMPLEMENT_DYNCREATE(CUnitView, CView)

CUnitView::CUnitView()
{

}

CUnitView::~CUnitView()
{
}

BEGIN_MESSAGE_MAP(CUnitView, CView)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEACTIVATE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CUnitView 그리기입니다.

void CUnitView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.

	//m_pDevice->GetDevice()->Clear(0, NULL
	//	, D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET
	//	, D3DCOLOR_ARGB(255, 0, 0, 0), 1.f, 0);
	//m_pDevice->GetDevice()->BeginScene();
	//m_pDevice->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	//m_wstrObjkey = L"Blue_Slime";

	//if(m_wstrObjkey == L"")
	//	return;
	//else
	//{
	//	
	//	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(m_wstrObjkey, TEXT("Normal"));

	//	//D3DXMATRIX		matTrans;

	//	//D3DXMatrixTranslation(&matTrans
	//	//	, 100.f
	//	//	, 100.f
	//	//	, 0.f);

	//	D3DXVECTOR3		vPos = D3DXVECTOR3(100.f, 100.f, 0.f);

	//	D3DXMATRIX		matWorld, matTrans, matScale;

	//	D3DXMatrixIdentity(&matWorld);
	//	D3DXMatrixIdentity(&matTrans);
	//	D3DXMatrixIdentity(&matScale);

	//	D3DXMatrixScaling(&matScale, 5.f, 5.f, 0.f);

	//	D3DXMatrixTranslation(&matTrans
	//		, vPos.x
	//		, vPos.y
	//		, vPos.z);

	//	matWorld = matScale * matTrans;
	//
	//	m_pDevice->GetSprite()->SetTransform(&matWorld);
	//	m_pDevice->GetSprite()->Draw(pTexInfo->pTexture
	//		, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width * 0.5f, pTexInfo->ImgInfo.Height* 0.5f, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));


	//}

	//m_pDevice->GetSprite()->End();
	//m_pDevice->GetDevice()->EndScene();
	//m_pDevice->GetDevice()->Present(NULL, NULL, m_hWnd, NULL);

	//Invalidate(TRUE);
}


// CUnitView 진단입니다.

#ifdef _DEBUG
void CUnitView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CUnitView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CUnitView 메시지 처리기입니다.

void CUnitView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_pDevice = CDevice::GetInstance();

	//m_pUnitTool = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->GetUnitTool();
	//m_pUnitTool->SetUnitView(this);

	m_wstrObjkey = L"";


	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

BOOL CUnitView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;
}

int CUnitView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return MA_ACTIVATE;
}

void CUnitView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.


}

void CUnitView::SetObjkey(CString strObjkey)
{
	m_wstrObjkey = strObjkey;

}

