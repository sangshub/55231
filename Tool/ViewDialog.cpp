// ViewDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "ViewDialog.h"
#include "Include.h"
#include "CubeView.h"


// CViewDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CViewDialog, CDialog)

CViewDialog::CViewDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CViewDialog::IDD, pParent)
{

}

CViewDialog::~CViewDialog()
{
}

void CViewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CViewDialog, CDialog)
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CViewDialog 메시지 처리기입니다.

BOOL CViewDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

int CViewDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	RECT rc;
	GetClientRect(&rc);


	m_pContext.m_pNewViewClass = RUNTIME_CLASS(CCubeView);

	m_pCubeView = (CCubeView*)((CFrameWnd*)this)->CreateView(&m_pContext);

	ASSERT(m_pCubeView);

	m_pCubeView->ShowWindow(SW_SHOW);
	m_pCubeView->MoveWindow(-WINCX/2+200, -WINCY/2+200, WINCX, WINCY, 1);

	return 0;
}

void CViewDialog::SetWindowSize(RECT _rc)
{
	RECT rc, rc2;

	GetWindowRect(&rc);
	GetClientRect(&rc2);

	int iCX = (rc.right-rc.left) - (rc2.right-rc2.left);
	int iCY = (rc.bottom-rc.top) - (rc2.bottom-rc2.top);
	SetWindowPos(NULL, _rc.right+10, _rc.top, 400 + iCX, 400 + iCY, 0);
}

int CViewDialog::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CDialog::OnMouseActivate(pDesktopWnd, nHitTest, message);
}