// PathTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathTool.h"
#include "FileInfo.h"
#include "Markup.h"

// CPathTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPathTool, CDialog)

CPathTool::CPathTool(CWnd* pParent /*=NULL*/)
	: CDialog(CPathTool::IDD, pParent)
{
}

CPathTool::~CPathTool()
{
	Release();
}

void CPathTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CPathTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CPathTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON13, &CPathTool::OnBnClickedReset)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

// CPathTool �޽��� ó�����Դϴ�.
void CPathTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	UpdateData(true);

	TCHAR szFullPath[MAX_PATH] = TEXT("");
	int iFileNum = DragQueryFile(hDropInfo, -1, NULL, 0);

	for(int i = 0; i < iFileNum; ++i)
	{
		DragQueryFile(hDropInfo, i, (LPWSTR)szFullPath, MAX_PATH);

		CFileInfo::DirtoryInfoExtraction(szFullPath, m_ImgPathList);

		wstring wstrCombine = TEXT("");
		TCHAR szCount[MIN_STR] = TEXT("");

		for(list<IMGPATH*>::iterator iter = m_ImgPathList.begin();
			iter != m_ImgPathList.end(); ++iter)
		{
			_itow_s((*iter)->iCount, szCount, 10);

			wstrCombine = (*iter)->wstrObjKey + TEXT("|") + (*iter)->wstrStateKey + TEXT("|");
			wstrCombine += szCount;
			wstrCombine += TEXT("|") + (*iter)->wstrPath;
			m_ListBox.AddString(wstrCombine.c_str());
		}
	}

	UpdateData(false);

	CDialog::OnDropFiles(hDropInfo);
}

void CPathTool::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(FALSE, TEXT("xml"), TEXT("*.xml"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("*.xml"), this);

	TCHAR szDirtoryPath[MAX_PATH] = TEXT("");
	lstrcpy(szDirtoryPath, CFileInfo::ChooseSaveFolder(TEXT("\\Data\\Path")).c_str());

	Dlg.m_ofn.lpstrInitialDir = szDirtoryPath;

	if(Dlg.DoModal() == IDCANCEL)
		return;

	CMarkup xml;

	xml.SetDoc(TEXT("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n\n"));

	xml.AddElem(TEXT("Paths"));

	for(list<IMGPATH*>::iterator iter = m_ImgPathList.begin();
		iter != m_ImgPathList.end(); ++iter)
	{
		xml.AddChildElem(TEXT("Obj"));

		xml.IntoElem();

		xml.AddChildElem(TEXT("ObjKey"), (*iter)->wstrObjKey.c_str());
		xml.AddChildElem(TEXT("StateKey"), (*iter)->wstrStateKey.c_str());
		//xml.AddChildElem(TEXT("DirectionKey"), (*iter)->wstrDirectionKey.c_str());
		xml.AddChildElem(TEXT("Count"), (*iter)->iCount);
		xml.AddChildElem(TEXT("Path"), (*iter)->wstrPath.c_str());

		xml.OutOfElem();
	}

	xml.Save(Dlg.GetPathName());
}

void CPathTool::OnBnClickedReset()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	Release();

	m_ListBox.ResetContent();

	UpdateData(FALSE);
}

void CPathTool::Release()
{
	for_each(m_ImgPathList.begin(), m_ImgPathList.end(), ::CDeleteObj());
	m_ImgPathList.clear();
}