// UnitTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"

#include "FileInfo.h"
#include "UnitView.h"
#include "TextureMgr.h"
#include "TileMap.h"


// CUnitTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(CUnitTool::IDD, pParent)
{

}

CUnitTool::~CUnitTool()
{
	Release();
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnLbnSelchangeList1)
END_MESSAGE_MAP()




void CUnitTool::SetUnitView(CUnitView* _pUnitView)
{
	m_pUnitView = _pUnitView;

}

BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CFileInfo::DirtoryInfoExtraction_Ex( L"../Texture/Monster", m_ImgPathlist);

	wstring		wstrCombine = L"";
	TCHAR		szCount[MIN_STR] = L"";

	for(list<IMGPATH*>::iterator iter = m_ImgPathlist.begin();
		iter != m_ImgPathlist.end(); iter++)
	{
		_itow_s((*iter)->iCount, szCount, 10);

		if(FAILED(CTextureMgr::GetInstance()->InsertTexture((*iter)->wstrPath,(*iter)->wstrObjKey,
			TEXTYPE_MULTI,(*iter)->wstrStateKey )))			
		{
			return FALSE;
		}//그림 넣어줌
		if(	(*iter)->wstrStateKey == L"Normal")
		{
			wstring wstrFilePath = (*iter)->wstrPath;

			TCHAR szRelativePath[MIN_STR] = L"";
			wsprintf(szRelativePath, wstrFilePath.c_str(), 0);

			m_ListBox.AddString((*iter)->wstrObjKey.c_str());
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CUnitTool::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_ViewDialog.ShowWindow(SW_HIDE);
	CTileMap::GetInstance()->SetObjMode(false);

	CDialog::OnClose();
}

void CUnitTool::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if(m_ViewDialog.GetSafeHwnd() == NULL)
	{
		m_ViewDialog.Create(IDD_VIEWDIALOG);
	}
	RECT rc;
	GetWindowRect(&rc);
	m_ViewDialog.SetWindowSize(rc);
	m_ViewDialog.ShowWindow(SW_SHOW);

	CTileMap::GetInstance()->SetObjMode(false);

}

void CUnitTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnDropFiles(hDropInfo);
}

void CUnitTool::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strSelectName;

	int  iSelectIdx = m_ListBox.GetCurSel();
	if(iSelectIdx  == -1)
		return;

	
	m_ListBox.GetText(iSelectIdx, strSelectName);

	m_pUnitView->SetObjkey(strSelectName);

	m_wstrCurSelName = strSelectName;

	
}

void CUnitTool::Release(void)
{
	for_each(m_ImgPathlist.begin(), m_ImgPathlist.end(), CDeleteObj());
	m_ImgPathlist.clear();


}

wstring CUnitTool::GetCurSelectName()
{
	return m_wstrCurSelName;

}

