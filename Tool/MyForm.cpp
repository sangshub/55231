// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"
#include "MainFrm.h"
#include "TileMap.h"
#include "FileInfo.h"
#include "Markup.h"
#include "BufferMgr.h"

// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(CMyForm::IDD)
	, m_iTileX(0)
	, m_iTileZ(0)
	, m_iDrawId(0)
	, m_pTileMap(CTileMap::GetInstance())
{
}

CMyForm::~CMyForm()
{
	Release();
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iTileX);
	DDX_Text(pDX, IDC_EDIT2, m_iTileZ);
	DDX_Text(pDX, IDC_EDIT3, m_iDrawId);
	DDX_Control(pDX, IDC_LIST2, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnBnClickedCubeTool)
	ON_BN_CLICKED(IDC_BUTTON7, &CMyForm::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyForm::OnBnClickedCreate)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON8, &CMyForm::OnBnClickedLoad)
	ON_LBN_SELCHANGE(IDC_LIST2, &CMyForm::OnLbnSelchangeList2)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON4, &CMyForm::OnBnClickedTileSave)
	ON_BN_CLICKED(IDC_BUTTON9, &CMyForm::OnBnClickedButtonObjTool)
	ON_BN_CLICKED(IDC_BUTTON10, &CMyForm::OnBnClickedItemTool)
	ON_BN_CLICKED(IDC_BUTTON11, &CMyForm::OnBnClickedAnimationTool)
	ON_BN_CLICKED(IDC_BUTTON26, &CMyForm::OnBnClickedPathTool)
	ON_BN_CLICKED(IDC_BUTTON27, &CMyForm::OnBnClickedEtcTool)
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

// CMyForm 메시지 처리기입니다.

void CMyForm::OnBnClickedCubeTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pCubeTex);
	if(m_CubeTool.GetSafeHwnd() == NULL)
	{
		m_CubeTool.Create(IDD_CUBETOOL);
	}
	m_CubeTool.ShowWindow(SW_SHOW);
}
void CMyForm::OnBnClickedButtonObjTool()
{

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_ObjTool.GetSafeHwnd() == NULL)
	{
		m_ObjTool.Create(IDD_OBJTOOL);
	}
	m_ObjTool.ShowWindow(SW_SHOW);
}


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_pMainView = ((CMainFrame*)AfxGetMainWnd())->m_pMainView;
	m_pBufferMgr = CBufferMgr::GetInstance();

	m_pCubeTex = new VTXCUBE[8];
	m_pBufferMgr->GetVtxInfo(TEXT("CubeTex"), m_pCubeTex);

	TileLoad();
}	

CCubeTool* CMyForm::GetCubeTool(void)
{
	return &m_CubeTool;
}

CObjTool* CMyForm::GetObjTool(void)
{
	return &m_ObjTool;
}

CItemTool* CMyForm::GetItemTool()
{
	return &m_ItemTool;
}

CEtcTool* CMyForm::GetEtcTool()
{
	return &m_EtcTool;
}

void CMyForm::OnBnClickedButton7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_UnitTool.GetSafeHwnd() == NULL)
	{
		m_UnitTool.Create(IDD_UNITTOOL);
	}
	m_UnitTool.ShowWindow(SW_SHOW);
}

void CMyForm::OnBnClickedItemTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_ItemTool.GetSafeHwnd() == NULL)
		m_ItemTool.Create(IDD_ITEMTOOL);

	m_ItemTool.ShowWindow(SW_SHOW);
}

void CMyForm::OnBnClickedCreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_pTileMap->CreateNewMap(m_iTileX, m_iTileZ, m_iDrawId);

	UpdateData(FALSE);
}

void CMyForm::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE, TEXT("dat"), TEXT("*.dat"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("*.dat"), this);

	TCHAR szDirtoryPath[MAX_PATH] = TEXT("");
	lstrcpy(szDirtoryPath, CFileInfo::ChooseSaveFolder(TEXT("\\Data\\Map")).c_str());

	Dlg.m_ofn.lpstrInitialDir = szDirtoryPath;

	if(Dlg.DoModal() == IDCANCEL)
		return;

	// 타일 저장
	vector<TILE*> vecTileInfo;

	m_pTileMap->InfoReset();

	vecTileInfo = *m_pTileMap->GetTileInfo();

	char ch[MAX_PATH] = "";
	WideCharToMultiByte(CP_ACP, NULL, Dlg.GetPathName(), -1, ch, MAX_PATH, NULL, FALSE);

	FILE* pFile = NULL;
	fopen_s(&pFile, ch, "wb");

	if(pFile == NULL)
		AfxMessageBox(TEXT("File Open Fail"));

	size_t iSize = vecTileInfo.size();

	for(size_t i = 0; i < iSize; ++i)
	{
		fwrite(vecTileInfo[i], sizeof(TILE), 1, pFile);
	}

	int iState = fclose(pFile);

	if(iState != 0)
		AfxMessageBox(TEXT("File Delete Fail"));

	// 타일 정보 저장
	CMarkup xml;

	xml.SetDoc(TEXT("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n\n"));

	xml.AddElem(TEXT("TileInfo"));
	xml.AddChildElem(TEXT("TileX"), m_pTileMap->GetTileX());
	xml.AddChildElem(TEXT("TileZ"), m_pTileMap->GetTileZ());

	CString strFileName = Dlg.GetPathName();

	PathFindFileName(strFileName);
	strFileName.TrimRight(TEXT("dat"));

	strFileName = strFileName + TEXT("xml");

	xml.Save(strFileName);

	// 큐브 저장
	m_CubeTool.SaveToMyForm(Dlg.GetPathName());

	// 아이템 저장
	m_ItemTool.SaveToMyForm(Dlg.GetPathName());

	// 몬스터 저장
	m_ObjTool.SaveToMyForm(Dlg.GetPathName());

	// 객체 저장
	m_EtcTool.SaveToMyForm(Dlg.GetPathName());
}

void CMyForm::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(TRUE, TEXT("dat"), TEXT("*.dat"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("*.dat"), this);

	TCHAR szDirtoryPath[MAX_PATH] = TEXT("");
	lstrcpy(szDirtoryPath, CFileInfo::ChooseSaveFolder(TEXT("\\Data\\Map")).c_str());

	Dlg.m_ofn.lpstrInitialDir = szDirtoryPath;

	if(Dlg.DoModal() == IDCANCEL)
		return;

	// 타일 로드
	vector<TILE*> vecTileInfo;

	char ch[MAX_PATH] = "";
	WideCharToMultiByte(CP_ACP, NULL, Dlg.GetPathName(), -1, ch, MAX_PATH, NULL, FALSE);

	FILE* pFile = NULL;
	fopen_s(&pFile, ch, "rb");

	if(pFile == NULL)
		AfxMessageBox(TEXT("File Open Fail"));

	while(!feof((pFile)))
	{
		TILE* pTile = new TILE;

		int iCheck = fread(pTile, sizeof(TILE), 1, pFile);

		if(iCheck == 0)
		{
			::Safe_Delete(pTile);

			break;
		}

		vecTileInfo.push_back(pTile);
	}

	int iState = fclose(pFile);

	if(iState != 0)
		AfxMessageBox(TEXT("File Delete Fail"));

	// 타일 정보 로드
	CMarkup xml;

	CString strFileName = Dlg.GetPathName();

	PathFindFileName(strFileName);
	strFileName.TrimRight(TEXT("dat"));

	strFileName = strFileName + TEXT("xml");

	xml.Load(strFileName);

	xml.FindElem(TEXT("TileInfo"));

	while(xml.FindChildElem(TEXT("TileX")))
	{
		m_iTileX = _ttoi(xml.GetChildData());
	}
	while(xml.FindChildElem(TEXT("TileZ")))
	{
		m_iTileZ = _ttoi(xml.GetChildData());
	}

	m_pTileMap->CreateNewMap(m_iTileX, m_iTileZ, m_iDrawId);
	m_pTileMap->SetTileInfo(vecTileInfo);

	// 큐브 로드
	m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pCubeTex);
	if(m_CubeTool.GetSafeHwnd() == NULL)
	{
		m_CubeTool.Create(IDD_CUBETOOL);
	}
	m_CubeTool.ShowWindow(SW_SHOW);
	m_CubeTool.ShowWindow(SW_HIDE);
	m_CubeTool.OnClose();

	m_CubeTool.LoadToMyForm(Dlg.GetPathName());

	// 아이템 로드
	if(m_ItemTool.GetSafeHwnd() == NULL)
		m_ItemTool.Create(IDD_ITEMTOOL);

	m_ItemTool.LoadToMyForm(Dlg.GetPathName());

	// 몬스터 로드
	m_pTileMap->SetResetVtx();
	if(m_ObjTool.GetSafeHwnd() == NULL)
	{
		m_ObjTool.Create(IDD_OBJTOOL);
	}
	m_ObjTool.LoadToMyForm(Dlg.GetPathName());

	// 객체 로드
	m_pTileMap->SetResetVtx();
	if(m_EtcTool.GetSafeHwnd() == NULL)
	{
		m_EtcTool.Create(IDD_ETCTOOL);
	}
	m_EtcTool.LoadToMyForm(Dlg.GetPathName());
}

void CMyForm::OnLbnSelchangeList2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strSelectName = TEXT("");

	int iSelectIndex = m_ListBox.GetCurSel();

	if(iSelectIndex == -1)
		return;

	m_ListBox.GetText(iSelectIndex, strSelectName);

	map<CString, CImage*>::iterator iter = m_MapImage.find(strSelectName);

	if(iter == m_MapImage.end())
		return;

	m_Picture.SetBitmap(*iter->second);

	m_iDrawId = CFileInfo::GetFileNumber(strSelectName);

	m_pTileMap->SetDrawId(m_iDrawId);

	UpdateData(FALSE);
}

void CMyForm::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);

	TCHAR szFullPath[MAX_PATH] = TEXT("");
	int iFileNum = DragQueryFile(hDropInfo, -1, NULL, 0);

	for(int i = 0; i < iFileNum; ++i)
	{
		DragQueryFile(hDropInfo, i, (LPWSTR)szFullPath, MAX_PATH);

		wstring wstrRelativePath = CFileInfo::ConvertRelativePath(szFullPath);

		CString strFileName = CFileInfo::GetFileName(szFullPath);

		map<CString, CImage*>::iterator iter = m_MapImage.find(strFileName);

		if(iter == m_MapImage.end())
		{
			CImage* pImage = new CImage;
			pImage->Load(wstrRelativePath.c_str());

			m_MapImage.insert(map<CString, CImage*>::value_type(strFileName, pImage));
			m_ImgPathList.push_back(wstrRelativePath);

			m_ListBox.AddString(strFileName);
		}
	}
	UpdateData(FALSE);

	CFormView::OnDropFiles(hDropInfo);
}

void CMyForm::OnBnClickedTileSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMarkup xml;

	xml.SetDoc(TEXT("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n\n"));

	xml.AddElem(TEXT("Tiles"));
	xml.AddChildElem(TEXT("Tile"));
	xml.IntoElem();
	for(list<wstring>::iterator iter = m_ImgPathList.begin();
		iter != m_ImgPathList.end(); ++iter)
	{
		xml.AddChildElem(TEXT("Tile"), (*iter).c_str());
	}
	xml.OutOfElem();

	xml.Save(TEXT("../Data/Tile/TileInfo.xml"));

	UpdateData(FALSE);
}

void CMyForm::TileLoad()
{
	UpdateData(TRUE);

	CMarkup xml;

	if(!xml.Load(TEXT("../Data/Tile/TileInfo.xml")))
		return;

	xml.FindElem(TEXT("Tiles"));
	xml.IntoElem();

	while(xml.FindChildElem(TEXT("Tile")))
	{
		CString strFileName = CFileInfo::GetFileName(wstring(xml.GetChildData()), 1);

		map<CString, CImage*>::iterator iter = m_MapImage.find(strFileName);

		if(iter == m_MapImage.end())
		{
			CImage* pImage = new CImage;
			pImage->Load(xml.GetChildData());

			m_MapImage.insert(map<CString, CImage*>::value_type(strFileName, pImage));
			m_ImgPathList.push_back(wstring(xml.GetChildData()));

			m_ListBox.AddString(strFileName);
		}
	}
	xml.OutOfElem();

	UpdateData(FALSE);
}

void CMyForm::Release()
{
	for(map<CString, CImage*>::iterator iter = m_MapImage.begin();
		iter != m_MapImage.end(); ++iter)
	{
		iter->second->Destroy();
		::Safe_Delete(iter->second);
	}
	m_MapImage.clear();
	m_ImgPathList.clear();

	::Safe_Delete_Array(m_pCubeTex);
}
void CMyForm::OnBnClickedAnimationTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pCubeTex);

	if(m_AniTool.GetSafeHwnd() == NULL)
	{
		m_AniTool.Create(IDD_ANITOOL);
	}
	m_AniTool.ShowWindow(SW_SHOW);
}

void CMyForm::OnBnClickedPathTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_PathTool.GetSafeHwnd() == NULL)
		m_PathTool.Create(IDD_PATHTOOL);

	m_PathTool.ShowWindow(SW_SHOW);
}

void CMyForm::OnBnClickedEtcTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_EtcTool.GetSafeHwnd() == NULL)
		m_EtcTool.Create(IDD_ETCTOOL);

	m_EtcTool.ShowWindow(SW_SHOW);
}

CAniTool* CMyForm::GetAniTool(void)
{
	return &m_AniTool;
}