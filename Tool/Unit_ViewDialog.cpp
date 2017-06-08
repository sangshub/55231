// Unit_ViewDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Unit_ViewDialog.h"
#include "Include.h"
#include "UnitView.h"


// CUnit_ViewDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUnit_ViewDialog, CDialog)

CUnit_ViewDialog::CUnit_ViewDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CUnit_ViewDialog::IDD, pParent)
{

}

CUnit_ViewDialog::~CUnit_ViewDialog()
{
}

void CUnit_ViewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUnit_ViewDialog, CDialog)
//	ON_WM_CLOSE()
ON_WM_MOUSEACTIVATE()
ON_WM_CREATE()
END_MESSAGE_MAP()


int CUnit_ViewDialog::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CDialog::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

BOOL CUnit_ViewDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

int CUnit_ViewDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	RECT rc;
	GetClientRect(&rc);

	m_pContext.m_pNewViewClass =RUNTIME_CLASS(CUnitView);

	m_pUnitView = (CUnitView*)((CFrameWnd*)this)->CreateView(&m_pContext);

	ASSERT(m_pUnitView);

	m_pUnitView->ShowWindow(SW_SHOW);
	m_pUnitView->MoveWindow(0,0,WINCX,WINCY,1);

	return 0;
}

void CUnit_ViewDialog::SetWindowSize(RECT _rc)
{
	RECT rc, rc2;

	GetWindowRect(&rc);
	GetClientRect(&rc2);

	int iCX = (rc.right-rc.left) - (rc2.right-rc2.left);
	int iCY = (rc.bottom-rc.top) - (rc2.bottom-rc2.top);
	SetWindowPos(NULL, _rc.right+10, _rc.top, 200 + iCX, 200 + iCY, 0);
}
