// ViewDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "ViewDialog.h"
#include "Include.h"
#include "CubeView.h"


// CViewDialog ��ȭ �����Դϴ�.

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


// CViewDialog �޽��� ó�����Դϴ�.

BOOL CViewDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

int CViewDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return CDialog::OnMouseActivate(pDesktopWnd, nHitTest, message);
}