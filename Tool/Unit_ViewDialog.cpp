// Unit_ViewDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "Unit_ViewDialog.h"
#include "Include.h"
#include "UnitView.h"


// CUnit_ViewDialog ��ȭ �����Դϴ�.

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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return CDialog::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

BOOL CUnit_ViewDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

int CUnit_ViewDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

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
