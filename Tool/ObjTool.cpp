// ObjTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "ObjTool.h"
#include "FileInfo.h"
#include "TileMap.h"
#include "TextureMgr.h"
#include "BufferMgr.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "Tile.h"
#include "Monster.h"
#include "ObjFactory.h"


// CObjTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjTool, CDialog)

CObjTool::CObjTool(CWnd* pParent /*=NULL*/)
	: CDialog(CObjTool::IDD, pParent)
	, m_pTileMap(CTileMap::GetInstance())
	, m_pVtxTex(NULL)	
{
}

CObjTool::~CObjTool()
{
	Release();
}

void CObjTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_Listbox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}


BEGIN_MESSAGE_MAP(CObjTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CObjTool::OnLbnSelchangeList1)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON1, &CObjTool::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON11, &CObjTool::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON12, &CObjTool::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTONSave, &CObjTool::OnBnClickedButtonsave1)
	ON_BN_CLICKED(IDC_BUTTONLoad, &CObjTool::OnBnClickedButtonload1)
END_MESSAGE_MAP()


// CObjTool 메시지 처리기입니다.

void CObjTool::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strSelectName;

	int  iSelectIdx = m_Listbox.GetCurSel();
	if(iSelectIdx  == -1)
		return;

	m_Listbox.GetText(iSelectIdx, strSelectName);

	map<CString, CImage*>::iterator iter = m_MapImage.find(strSelectName);

	m_wstrCurSelName = strSelectName;

	if(iter == m_MapImage.end())
		return;

	m_Picture.SetBitmap(*iter->second);	
}

wstring CObjTool::GetCurSelectName()
{
	return m_wstrCurSelName;

}

void CObjTool::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CTileMap::GetInstance()->SetObjMode(false);

	CDialog::OnClose();
}

void CObjTool::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.


	CTileMap::GetInstance()->SetObjMode(true);
}

BOOL CObjTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CFileInfo::DirtoryInfoExtraction_Ex( L"../Texture/Monster", m_ImgPathlist);
	wstring		wstrCombine = L"";
	TCHAR		szCount[MIN_STR] = L"";

	for(list<IMGPATH*>::iterator iter = m_ImgPathlist.begin();
		iter != m_ImgPathlist.end(); ++iter)
	{
		_itow_s((*iter)->iCount, szCount, 10);

		if((*iter)->wstrObjKey == TEXT("Light_Minotaur_Charge") 
			|| (*iter)->wstrObjKey == TEXT("Light_Minotaur_Stun")
			|| (*iter)->wstrObjKey == TEXT("Octoboss")
			|| (*iter)->wstrObjKey == TEXT("Yellow_Skeleton_NoHead")
			|| (*iter)->wstrObjKey == TEXT("Blue_Banshee_Cry")
			|| (*iter)->wstrObjKey == TEXT("Black_Skeleton_NoHead"))
			continue;

		if(FAILED(CTextureMgr::GetInstance()->InsertTexture((*iter)->wstrPath,(*iter)->wstrObjKey,
			TEXTYPE_MULTI,(*iter)->wstrStateKey,TEXT(""),(*iter)->iCount )))			
		{
			return FALSE;
		}//그림 넣어줌
		if(	(*iter)->wstrStateKey == L"Normal")
		{
			wstring wstrFilePath = (*iter)->wstrPath;

			TCHAR szRelativePath[MIN_STR] = L"";
			wsprintf(szRelativePath, wstrFilePath.c_str(), 0);

			map<CString, CImage*>::iterator iter2 = m_MapImage.find((*iter)->wstrObjKey.c_str());

				if( iter2 == m_MapImage.end())
				{
					CImage*		pImage = new CImage;
					pImage->Load(szRelativePath);

					m_MapImage.insert(map<CString, CImage*>::value_type( (*iter)->wstrObjKey.c_str(), pImage ));
				}

			m_Listbox.AddString((*iter)->wstrObjKey.c_str());
		}
	}

	m_pVtxTex = new VTXTEX[4];

	CBufferMgr::GetInstance()->GetVtxInfo(TEXT("UnitTex"), m_pVtxTex);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CObjTool::CreateObj(D3DXVECTOR3 vPos)
{
	int iSelect = m_Listbox.GetCurSel();
	if(iSelect > -1)
	{
		//CBufferMgr::GetInstance()->SetVtxInfo(TEXT("UnitTex"), m_pVtxTex);

		vPos.y = 1.f;

		CObj* pObj = (CMonster*)CObjFactory<CMonster>::CreateObj();
		pObj->SetPos(vPos);
		pObj->SetObjKey(m_wstrCurSelName);

		CObjMgr::GetInstance()->AddObj(OBJ_MONSTER, pObj);

		SLIME* tData = new SLIME;

		tData->vPos = vPos;

		wcscpy_s(tData->szObjkey,m_wstrCurSelName.c_str());

		m_MonsterData.push_back(tData);

		vector<CObj*> vecTile = m_pTileMap->GetFullTile();

		int iSize = vecTile.size();

		for(int i = 0; i < iSize; ++i)
		{
			if(vecTile[i]->GetInfo()->vPos.x == m_MonsterData.back()->vPos.x
				&& vecTile[i]->GetInfo()->vPos.z == m_MonsterData.back()->vPos.z)
			{
				((CTile*)vecTile[i])->SetTileObj(pObj);

				return;
			}
		}
	}
}

void CObjTool::Release()
{
	for_each(m_ImgPathlist.begin(), m_ImgPathlist.end(), CDeleteObj());
	m_ImgPathlist.clear();

	for_each(m_MonsterData.begin(), m_MonsterData.end(), CDeleteObj());
	m_MonsterData.clear();

	for(map<CString, CImage*>::iterator iter = m_MapImage.begin();
		iter != m_MapImage.end(); ++iter)
	{
		iter->second->Destroy();
		Safe_Delete(iter->second);
	}

	m_MapImage.clear();

	::Safe_Delete_Array(m_pVtxTex);

}

void CObjTool::OnBnClickedButtonCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(!m_MonsterData.empty())
	{
		vector<CObj*> vecTile = m_pTileMap->GetFullTile();

		int iSize = vecTile.size();

		for(int i = 0; i < iSize; ++i)
		{
			if(m_MonsterData.back()->vPos.x == vecTile[i]->GetInfo()->vPos.x
				&& m_MonsterData.back()->vPos.z == vecTile[i]->GetInfo()->vPos.z)
				((CTile*)vecTile[i])->SetObjCnt(0);
		}

		::Safe_Delete(m_MonsterData.back());
		m_MonsterData.pop_back();
		CObjMgr::GetInstance()->MonsterLastDelete();

	}

}

void CObjTool::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	/*CFileDialog		Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
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

	for(list<SLIME*>::iterator iter = m_MonsterData.begin();
		iter != m_MonsterData.end(); ++iter)
	{
		WriteFile(hFile, *iter, sizeof(SLIME), &dwByte, NULL);
	}
	CloseHandle(hFile);*/

	


}

void CObjTool::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


}

void CObjTool::OnBnClickedButtonsave1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HANDLE		hFile = CreateFile(L"../Data/Monster/Monster.dat", GENERIC_WRITE
		, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD	dwByte;

	for(list<SLIME*>::iterator iter = m_MonsterData.begin();
		iter != m_MonsterData.end(); iter++)
	{
		WriteFile(hFile, (*iter), sizeof(SLIME), &dwByte, NULL);
	}
	CloseHandle(hFile);
}

void CObjTool::OnBnClickedButtonload1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CObjMgr::GetInstance()->MonsterDataReset();
	for_each(m_MonsterData.begin(), m_MonsterData.end(), CDeleteObj());
	m_MonsterData.clear();

	HANDLE		hFile = CreateFile(L"../Data/Monster/Monster.dat", GENERIC_READ
		, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD		dwByte;

	while( 1 )
	{
		SLIME*		pData = new SLIME;

		ReadFile(hFile, pData, sizeof(SLIME), &dwByte, NULL);

		if(dwByte == 0)
		{
			Safe_Delete(pData);
			break;
		}

		CBufferMgr::GetInstance()->SetVtxInfo(TEXT("UnitTex"), m_pVtxTex);

		pData->vPos.y = 1.f;

		CObj* pObj = (CMonster*)CObjFactory<CMonster>::CreateObj();
		pObj->SetPos(pData->vPos);
		pObj->SetObjKey(pData->szObjkey);

		CObjMgr::GetInstance()->AddObj(OBJ_MONSTER, pObj);

		m_MonsterData.push_back(pData);
	}
	CloseHandle(hFile);
}

void CObjTool::SaveToMyForm(const CString& strPath)
{
	CString strObjPath = strPath;
	CString strObjName = PathFindFileName(strObjPath);
	strObjName.Replace(TEXT(".dat"), TEXT(""));
	strObjName = strObjName + TEXT("_Obj.txt");
	CString strOriginName = PathFindFileName(strObjPath);
	strObjPath.Replace(strOriginName, strObjName);

	HANDLE		hFile = CreateFile(strObjPath, GENERIC_WRITE
		, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD	dwByte;

	for(list<SLIME*>::iterator iter = m_MonsterData.begin();
		iter != m_MonsterData.end(); iter++)
	{
		WriteFile(hFile, (*iter), sizeof(SLIME), &dwByte, NULL);
	}
	CloseHandle(hFile);
}

void CObjTool::LoadToMyForm(const CString& strPath)
{
	CObjMgr::GetInstance()->MonsterDataReset();
	for_each(m_MonsterData.begin(), m_MonsterData.end(), CDeleteObj());
	m_MonsterData.clear();

	CString strObjPath = strPath;
	CString strObjName = PathFindFileName(strObjPath);
	strObjName.Replace(TEXT(".dat"), TEXT(""));
	strObjName = strObjName + TEXT("_Obj.txt");
	CString strOriginName = PathFindFileName(strObjPath);
	strObjPath.Replace(strOriginName, strObjName);

	HANDLE		hFile = CreateFile(strObjPath, GENERIC_READ
		, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD		dwByte;

	vector<CObj*> vecTile = m_pTileMap->GetFullTile();

	int iSize = vecTile.size();

	while( 1 )
	{
		SLIME*		pData = new SLIME;

		ReadFile(hFile, pData, sizeof(SLIME), &dwByte, NULL);

		if(dwByte == 0)
		{
			Safe_Delete(pData);
			break;
		}

		CBufferMgr::GetInstance()->SetVtxInfo(TEXT("UnitTex"), m_pVtxTex);

		pData->vPos.y = 1.f;

		CObj* pObj = (CMonster*)CObjFactory<CMonster>::CreateObj();
		pObj->SetPos(pData->vPos);
		pObj->SetObjKey(pData->szObjkey);

		CObjMgr::GetInstance()->AddObj(OBJ_MONSTER, pObj);

		m_MonsterData.push_back(pData);

		for(int i = 0; i < iSize; ++i)
		{
			if(vecTile[i]->GetInfo()->vPos.x == m_MonsterData.back()->vPos.x
				&& vecTile[i]->GetInfo()->vPos.z == m_MonsterData.back()->vPos.z)
			{
				((CTile*)vecTile[i])->SetTileObj(pObj);
			}
		}
	}
	CloseHandle(hFile);
}

void CObjTool::DeleteObjData(D3DXVECTOR3 vPos)
{
	for(list<SLIME*>::iterator iter = m_MonsterData.begin();
		iter != m_MonsterData.end();)
	{
		if((*iter)->vPos.x == vPos.x 
			&& (*iter)->vPos.z == vPos.z)
		{
			::Safe_Delete(*iter);
			iter = m_MonsterData.erase(iter);

			return;
		}
		else
			++iter;
	}
}