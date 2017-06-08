// CubeTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "CubeTool.h"
#include "CubeView.h"
#include "ObjFactory.h"
#include "Cube.h"
#include "Tile.h"
#include "ObjMgr.h"
#include "TileMap.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "BufferMgr.h"
#include "TextureMgr.h"


// CCubeTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCubeTool, CDialog)

CCubeTool::CCubeTool(CWnd* pParent /*=NULL*/)
	: CDialog(CCubeTool::IDD, pParent)
	, m_pCubeTex(NULL)
	, m_CubeName(_T(""))
{

}

CCubeTool::~CCubeTool()
{
	Release();
}

void CCubeTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ImageList);
	DDX_Control(pDX, IDC_LIST2, m_CubeList);
	DDX_Control(pDX, IDC_CHECK1, m_Check[0]);
	DDX_Control(pDX, IDC_CHECK2, m_Check[1]);
	DDX_Control(pDX, IDC_CHECK3, m_Check[2]);
	DDX_Control(pDX, IDC_CHECK4, m_Check[3]);
	DDX_Control(pDX, IDC_CHECK5, m_Check[4]);
	DDX_Control(pDX, IDC_CHECK6, m_Check[5]);
	DDX_Control(pDX, IDC_CHECK7, m_Check[6]);
	DDX_Text(pDX, IDC_EDIT1, m_CubeName);
}


BEGIN_MESSAGE_MAP(CCubeTool, CDialog)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_WM_KEYDOWN()
	ON_LBN_SELCHANGE(IDC_LIST1, &CCubeTool::OnLbnSelchangeImageList)
	ON_BN_CLICKED(IDC_BUTTON3, &CCubeTool::OnBnClickedAddCubeData)
	ON_LBN_SELCHANGE(IDC_LIST2, &CCubeTool::OnLbnSelchangeCubeDataList)
	ON_BN_CLICKED(IDC_BUTTON2, &CCubeTool::OnBnClickedDeleteList)
	ON_BN_CLICKED(IDC_BUTTON1, &CCubeTool::OnBnClickedChangeData)
	ON_BN_CLICKED(IDC_BUTTON4, &CCubeTool::OnBnClickedDropCubeDelete)
	ON_BN_CLICKED(IDC_BUTTON5, &CCubeTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON6, &CCubeTool::OnBnClickedLoad)
END_MESSAGE_MAP()


// CCubeTool 메시지 처리기입니다.

BOOL CCubeTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_tCube.iDrawId = 0;
	m_tCube.dwType = 0;
	m_tCube.vPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	m_pTileMap = CTileMap::GetInstance();
	m_pObjMgr = CObjMgr::GetInstance();
	m_pBufferMgr = CBufferMgr::GetInstance();
	m_pTextureMgr = CTextureMgr::GetInstance();
	m_pCubeTex = new VTXCUBE[8];

	m_pMainView = ((CMainFrame*)AfxGetMainWnd())->m_pMainView;
	m_pBufferMgr->GetVtxInfo(TEXT("CubeTex"), m_pCubeTex);

	m_mapCubeTex = m_pTextureMgr->GetCubeTexMap();

	CubeImageSetUp();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCubeTool::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_ViewDIalog.ShowWindow(SW_HIDE);
	m_pTileMap->SetCubeMode(false);
	m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pCubeTex);

	CDialog::OnClose();
}

void CCubeTool::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if(m_ViewDIalog.GetSafeHwnd() == NULL)
	{
		m_ViewDIalog.Create(IDD_VIEWDIALOG);
	}
	RECT rc;
	GetWindowRect(&rc);
	m_ViewDIalog.SetWindowSize(rc);
	m_ViewDIalog.ShowWindow(SW_SHOW);

	m_pCubeView->SetCubeData(m_tCube);

	m_pTileMap->SetCubeMode(true);
}

void CCubeTool::SetCubeView(CCubeView* _pCubeView)
{
	m_pCubeView = _pCubeView;

	m_pMainView->SetCubeView(_pCubeView);
}
void CCubeTool::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_pCubeView->CameraMove();

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCubeTool::CreateCube(D3DXVECTOR3 vTilePos, int iCount)
{
	int iSelect = m_CubeList.GetCurSel();
	if(iSelect > -1 && !iCount)
	{
		m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pCubeTex);

		vTilePos.y = 1.f;

		CCube* pCube = (CCube*)CObjFactory<CCube>::CreateObj(0);
		CUBE* pCubeData = new CUBE;

		GetCubeData(*pCubeData, vTilePos);
		pCube->SetCubeInfo(*pCubeData);

		m_pObjMgr->AddObj(OBJ_CUBE, pCube);
		m_CubeDrop.push_back(pCubeData); // 저장할때 쓸 데이터

		vector<CObj*> vecTile = m_pTileMap->GetFullTile();

		int iSize = vecTile.size();

		for(int i = 0; i < iSize; ++i)
		{
			if(vecTile[i]->GetInfo()->vPos == m_CubeDrop.back()->vPos + D3DXVECTOR3(0.f, -1.f, 0.f))
			{
				((CTile*)vecTile[i])->SetTileObj(pCube);
				((CTile*)vecTile[i])->SetCubeCnt(((CTile*)vecTile[i])->GetCubeCnt() + 1);

				return;
			}
		}
	}
}

void CCubeTool::GetCubeData(CUBE& tOut, D3DXVECTOR3 _vTilePos)
{
	tOut.iDrawId = m_tCube.iDrawId;
	tOut.dwType = m_tCube.dwType;
	tOut.vPos = _vTilePos;
}

void CCubeTool::Release(void)
{
	for_each(m_CubeDrop.begin(), m_CubeDrop.end(), CDeleteObj());
	m_CubeDrop.clear();
	for_each(m_CubeData.begin(), m_CubeData.end(), CDeleteObj());
	m_CubeData.clear();
	if(m_pCubeTex)
	{
		delete [] m_pCubeTex;
		m_pCubeTex = NULL;
	}
}

void CCubeTool::CubeImageSetUp(void)
{
	UpdateData(TRUE);

	for(map<BYTE, LPDIRECT3DCUBETEXTURE9>::iterator iter = m_mapCubeTex->begin();
		iter != m_mapCubeTex->end(); ++iter)
	{
		int iNum = iter->first;
		TCHAR szImageName[MIN_STR] = L"";

		if(iNum/10 < 1)
			wsprintf(szImageName, L"CUBE0%d", iNum);
		else
			wsprintf(szImageName, L"CUBE%d", iNum);

		m_ImageList.AddString(szImageName);
	}

	UpdateData(false);
}

void CCubeTool::OnLbnSelchangeImageList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_CubeList.SetCurSel(-1);
	CString strSelect;
	int iNum = m_ImageList.GetCurSel();
	if(iNum > -1)
	{
		m_ImageList.GetText(iNum, strSelect);

		strSelect = strSelect.Right(strSelect.GetLength() - strSelect.ReverseFind(L'E') -1);

		int iSelectNum = _wtoi(strSelect.GetString());

		m_tCube.iDrawId = iSelectNum;

		m_pCubeView->SetCubeData(m_tCube);
	}
}

void CCubeTool::OnBnClickedAddCubeData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CUBE*	pCube = new CUBE;

	DWORD dwType = 0;

	if(m_Check[0].GetCheck())
		dwType |= CUBE_BROK;

	if(m_Check[1].GetCheck())
		dwType |= CUBE_BOOM;

	if(m_Check[2].GetCheck())
		dwType |= CUBE_NOTB;

	if(m_Check[3].GetCheck())
		dwType |= CUBE_ITEM;

	if(m_Check[4].GetCheck())
		dwType |= CUBE_THRO;

	if(m_Check[5].GetCheck())
		dwType |= CUBE_EVEN1;

	if(m_Check[6].GetCheck())
		dwType |= CUBE_EVEN2;

	pCube->dwType = dwType;
	pCube->iDrawId = m_tCube.iDrawId;
	pCube->vPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	TCHAR szCubeName[MIN_STR] = L"";

	if(m_CubeData.size() < 10)
		wsprintf(szCubeName, L"0%d%s",m_CubeData.size(), m_CubeName.GetString());
	else
		wsprintf(szCubeName, L"%d%s",m_CubeData.size(), m_CubeName.GetString());

	m_CubeList.AddString(szCubeName);
	m_CubeData.push_back(pCube);

	UpdateData(FALSE);
}

void CCubeTool::OnLbnSelchangeCubeDataList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	int iNum = m_CubeList.GetCurSel();

	if(iNum > -1)
	{
		m_tCube.dwType = m_CubeData[iNum]->dwType;
		m_tCube.iDrawId = m_CubeData[iNum]->iDrawId;
		m_tCube.vPos = m_CubeData[iNum]->vPos;

		m_CubeList.GetText(iNum, m_CubeName);

		for(int i = 0; i < 7; ++i)
			m_Check[i].SetCheck(0);

		if(m_tCube.dwType & CUBE_BROK)
			m_Check[0].SetCheck(1);
		if(m_tCube.dwType & CUBE_BOOM)
			m_Check[1].SetCheck(1);
		if(m_tCube.dwType & CUBE_NOTB)
			m_Check[2].SetCheck(1);
		if(m_tCube.dwType & CUBE_ITEM)
			m_Check[3].SetCheck(1);
		if(m_tCube.dwType & CUBE_THRO)
			m_Check[4].SetCheck(1);
		if(m_tCube.dwType & CUBE_EVEN1)
			m_Check[5].SetCheck(1);
		if(m_tCube.dwType & CUBE_EVEN2)
			m_Check[6].SetCheck(1);

		m_pCubeView->SetCubeData(m_tCube);
	}
	
	UpdateData(FALSE);
}

void CCubeTool::OnBnClickedDeleteList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int iNum = m_CubeList.GetCurSel();

	if(iNum > -1)
	{
		for(vector<CUBE*>::iterator iter = m_CubeData.begin();
		iter != m_CubeData.end(); ++iter)
		{
			if((*iter) == m_CubeData[iNum])
			{
				::Safe_Delete(*iter);
				m_CubeData.erase(iter);
				break;
			}
		}
		m_CubeList.DeleteString(iNum);
		m_CubeName = L"";

		m_CubeList.SetCurSel(-1);

		for(int i = 0; i < 7; ++i)
			m_Check[i].SetCheck(0);

		RemoveCubeData(m_tCube);
		m_pCubeView->SetCubeData(m_tCube);
	}

	

	UpdateData(FALSE);
}

void CCubeTool::OnBnClickedChangeData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	int iNum = m_CubeList.GetCurSel();

	if(iNum > -1)
	{
		m_CubeList.DeleteString(iNum);

		TCHAR szCubeName[MIN_STR] = L"";

		if(iNum < 10)
			wsprintf(szCubeName, L"0%d%s",iNum, m_CubeName.GetString());
		else
			wsprintf(szCubeName, L"%d%s",iNum, m_CubeName.GetString());

		m_CubeList.AddString(szCubeName);

		DWORD dwType = 0;
		if(m_Check[0].GetCheck())
			dwType |= CUBE_BROK;
		if(m_Check[1].GetCheck())
			dwType |= CUBE_BOOM;
		if(m_Check[2].GetCheck())
			dwType |= CUBE_NOTB;
		if(m_Check[3].GetCheck())
			dwType |= CUBE_ITEM;
		if(m_Check[4].GetCheck())
			dwType |= CUBE_THRO;
		if(m_Check[5].GetCheck())
			dwType |= CUBE_EVEN1;
		if(m_Check[6].GetCheck())
			dwType |= CUBE_EVEN2;

		m_tCube.dwType = dwType;

		for(vector<CUBE*>::iterator iter = m_CubeData.begin();
			iter != m_CubeData.end(); ++iter)
		{
			if((*iter) == m_CubeData[iNum])
			{
				(*iter)->dwType = m_tCube.dwType;
				(*iter)->iDrawId = m_tCube.iDrawId;
				(*iter)->vPos = m_tCube.vPos;
				break;
			}
		}

		m_pCubeView->SetCubeData(m_tCube);
	}
	UpdateData(FALSE);
}

void CCubeTool::RemoveCubeData(CUBE& tOut)
{
	m_tCube.dwType = 0;
	m_tCube.iDrawId = 0;
	m_tCube.vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
}

void CCubeTool::OnBnClickedDropCubeDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_CubeDrop.size())
	{
		vector<CObj*> vecTile = m_pTileMap->GetFullTile();

		int iSize = vecTile.size();

		for(int i = 0; i < iSize; ++i)
		{
			if(vecTile[i]->GetInfo()->vPos == m_CubeDrop.back()->vPos + D3DXVECTOR3(0.f, -1.f, 0.f))
			{
				((CTile*)vecTile[i])->SetCubeCnt(0);

				::Safe_Delete(m_CubeDrop.back());

				m_CubeDrop.pop_back();
				m_pObjMgr->CubeLastDelete();

				return;
			}
		}
	}
}

void CCubeTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog		Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.dat", this);

	TCHAR		szDirtoryPath[MAX_PATH];
	GetCurrentDirectory(sizeof(szDirtoryPath), szDirtoryPath);
	SetCurrentDirectory(szDirtoryPath);

	PathRemoveFileSpec(szDirtoryPath);
	lstrcat(szDirtoryPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szDirtoryPath;

	if(Dlg.DoModal() == IDCANCEL)
		return;

	HANDLE hFile = NULL;
	DWORD dwByte = 0;

	hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS
		, FILE_ATTRIBUTE_NORMAL, NULL);

	for(list<CUBE*>::iterator iter = m_CubeDrop.begin();
		iter != m_CubeDrop.end(); ++iter)
	{
		WriteFile(hFile, *iter, sizeof(CUBE), &dwByte, NULL);
	}
	CloseHandle(hFile);
}

void CCubeTool::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pObjMgr->CubeDataReset();
	for_each(m_CubeDrop.begin(), m_CubeDrop.end(), CDeleteObj());
	m_CubeDrop.clear();
	
	CFileDialog		Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.dat", this);

	if(Dlg.DoModal() == IDCANCEL)
		return;

	HANDLE hFile = NULL;
	DWORD dwByte = 0;

	hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, NULL, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL, NULL);

	m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pCubeTex);

	while(true)
	{
		CUBE* pCubeData = new CUBE;
		ReadFile(hFile, pCubeData, sizeof(CUBE), &dwByte, NULL);

		if(dwByte == 0)
		{
			Safe_Delete(pCubeData);
			break;
		}

		CCube* pCube = (CCube*)CObjFactory<CCube>::CreateObj();
		pCube->SetCubeInfo(*pCubeData);

		m_pObjMgr->AddObj(OBJ_CUBE, pCube);
		m_CubeDrop.push_back(pCubeData); // 저장할때 쓸 데이터
	}
	CloseHandle(hFile);
}

void CCubeTool::SaveToMyForm(const CString& strPath)
{
	CString strCubePath = strPath;
	CString strCubeName = PathFindFileName(strCubePath);
	strCubeName.Replace(TEXT(".dat"), TEXT(""));
	strCubeName = strCubeName + TEXT("_Cube.txt");
	CString strOriginName = PathFindFileName(strCubePath);
	strCubePath.Replace(strOriginName, strCubeName);

	HANDLE hFile = NULL;
	DWORD dwByte = 0;

	hFile = CreateFile(strCubePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS
		, FILE_ATTRIBUTE_NORMAL, NULL);

	for(list<CUBE*>::iterator iter = m_CubeDrop.begin();
		iter != m_CubeDrop.end(); ++iter)
	{
		WriteFile(hFile, *iter, sizeof(CUBE), &dwByte, NULL);
	}
	CloseHandle(hFile);
}

void CCubeTool::LoadToMyForm(const CString& strPath)
{
	m_pObjMgr->CubeDataReset();
	for_each(m_CubeDrop.begin(), m_CubeDrop.end(), CDeleteObj());
	m_CubeDrop.clear();

	CString strCubePath = strPath;
	CString strCubeName = PathFindFileName(strCubePath);
	strCubeName.Replace(TEXT(".dat"), TEXT(""));
	strCubeName = strCubeName + TEXT("_Cube.txt");
	CString strOriginName = PathFindFileName(strCubePath);
	strCubePath.Replace(strOriginName, strCubeName);

	HANDLE hFile = NULL;
	DWORD dwByte = 0;

	hFile = CreateFile(strCubePath, GENERIC_READ, 0, NULL, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL, NULL);

	m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pCubeTex);

	vector<CObj*> vecTile = m_pTileMap->GetFullTile();

	int iSize = vecTile.size();

	while(true)
	{
		CUBE* pCubeData = new CUBE;
		ReadFile(hFile, pCubeData, sizeof(CUBE), &dwByte, NULL);

		if(dwByte == 0)
		{
			Safe_Delete(pCubeData);
			break;
		}

		CCube* pCube = (CCube*)CObjFactory<CCube>::CreateObj();
		pCube->SetCubeInfo(*pCubeData);

		m_pObjMgr->AddObj(OBJ_CUBE, pCube);
		m_CubeDrop.push_back(pCubeData); // 저장할때 쓸 데이터

		for(int i = 0; i < iSize; ++i)
		{
			if(vecTile[i]->GetInfo()->vPos == m_CubeDrop.back()->vPos + D3DXVECTOR3(0.f, -1.f, 0.f))
				((CTile*)vecTile[i])->SetTileObj(pCube);
		}
	}
	CloseHandle(hFile);
}

void CCubeTool::DeleteCubeData(D3DXVECTOR3 vPos)
{
	vPos.y = 1.f;

	for(list<CUBE*>::iterator iter = m_CubeDrop.begin();
		iter != m_CubeDrop.end();)
	{
		if((*iter)->vPos == vPos)
		{
			::Safe_Delete(*iter);
			iter = m_CubeDrop.erase(iter);

			return;
		}
		else
			++iter;
	}
}