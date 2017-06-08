// AniDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "AniDialog.h"
#include "Include.h"
#include "AniView.h"


// CAniDialog 대화 상자입니다.

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


// CAniDialog 메시지 처리기입니다.


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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

int CAniDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

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
