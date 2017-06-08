// AniDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "AniDialog.h"
#include "Include.h"
#include "AniView.h"


// CAniDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAniDialog, CDialog)

CAniDialog::CAniDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAniDialog::IDD, pParent)
{

}

CAniDialog::~CAniDialog()
{
}

void CAniDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAniDialog, CDialog)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CAniDialog �޽��� ó�����Դϴ�.


void CAniDialog::SetWindowSize(RECT _rc)
{
	RECT rc, rc2;

	GetWindowRect(&rc);
	GetClientRect(&rc2);

	int iCX = (rc.right-rc.left) - (rc2.right-rc2.left);
	int iCY = (rc.bottom-rc.top) - (rc2.bottom-rc2.top);
	SetWindowPos(NULL, _rc.right+10, _rc.top, WINCX + iCX, WINCY + iCY, 0);
}
BOOL CAniDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

int CAniDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	RECT rc;
	GetClientRect(&rc);


	m_pContext.m_pNewViewClass = RUNTIME_CLASS(CAniView);

	m_pAniView = (CAniView*)((CFrameWnd*)this)->CreateView(&m_pContext);

	ASSERT(m_pAniView);

	m_pAniView->ShowWindow(SW_SHOW);
	m_pAniView->MoveWindow(0, 0, WINCX, WINCY, 1);

	return 0;
}

void CAniDialog::SetAniTool(CAniTool* _pAniTool)
{
	m_pAniTool = _pAniTool;
	m_pAniView->SetAniTool(_pAniTool);
}
