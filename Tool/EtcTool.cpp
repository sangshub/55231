// EtcTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "Monster.h"
#include "EtcTool.h"
#include "Tile.h"
#include "FileInfo.h"
#include "TextureMgr.h"
#include "BufferMgr.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "TileMap.h"

// CEtcTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CEtcTool, CDialog)

CEtcTool::CEtcTool(CWnd* pParent /*=NULL*/)
	: CDialog(CEtcTool::IDD, pParent)
	, m_pVtxTex(NULL)
	, m_pTextureMgr(CTextureMgr::GetInstance())
	, m_pBufferMgr(CBufferMgr::GetInstance())
	, m_pObjMgr(CObjMgr::GetInstance())
	, m_pTileMap(CTileMap::GetInstance())
{
}

CEtcTool::~CEtcTool()
{
	Release();
}

void CEtcTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}

BEGIN_MESSAGE_MAP(CEtcTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CEtcTool::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CEtcTool::OnBnClickedReturn)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CEtcTool �޽��� ó�����Դϴ�.
void CEtcTool::Release()
{
	for_each(m_ImgPathList.begin(), m_ImgPathList.end(), ::CDeleteObj());
	m_ImgPathList.clear();

	for_each(m_ObjDataList.begin(), m_ObjDataList.end(), ::CDeleteObj());
	m_ObjDataList.clear();

	for(map<CString, CImage*>::iterator iter = m_mapImage.begin();
		iter != m_mapImage.end(); ++iter)
	{
		iter->second->Destroy();
		::Safe_Delete(iter->second);
	}
	m_mapImage.clear();

	::Safe_Delete_Array(m_pVtxTex);
}

BOOL CEtcTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CFileInfo::DirtoryInfoExtraction_Ex(TEXT("../Texture/Obj"), m_ImgPathList);
	wstring wstrCombine = TEXT("");
	TCHAR szCount[MIN_STR] = TEXT("");

	for(list<IMGPATH*>::iterator iter = m_ImgPathList.begin();
		iter != m_ImgPathList.end(); iter++)
	{
		_itow_s((*iter)->iCount, szCount, 10);

		if(FAILED(m_pTextureMgr->InsertTexture((*iter)->wstrPath,(*iter)->wstrObjKey,
			TEXTYPE_MULTI,(*iter)->wstrStateKey,TEXT(""),(*iter)->iCount )))			
			return FALSE;

		if((*iter)->wstrStateKey == TEXT("Normal"))
		{
			wstring wstrFilePath = (*iter)->wstrPath;

			TCHAR szRelativePath[MIN_STR] = TEXT("");
			wsprintf(szRelativePath, wstrFilePath.c_str(), 0);

			map<CString, CImage*>::iterator iter2 = m_mapImage.find((*iter)->wstrObjKey.c_str());

			if(iter2 == m_mapImage.end())
			{
				CImage*	pImage = new CImage;
				pImage->Load(szRelativePath);

				m_mapImage.insert(map<CString, CImage*>::value_type((*iter)->wstrObjKey.c_str(), pImage));
			}
			m_ListBox.AddString((*iter)->wstrObjKey.c_str());
		}
	}

	m_pVtxTex = new VTXTEX[4];

	m_pBufferMgr->GetVtxInfo(TEXT("UnitTex"), m_pVtxTex);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CEtcTool::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_pTileMap->SetEtcMode(true);
}

void CEtcTool::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	m_pTileMap->SetEtcMode(false);

	CDialog::OnClose();
}

void CEtcTool::OnLbnSelchangeList1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strSelectName = TEXT("");

	int iSelectIdx = m_ListBox.GetCurSel();

	if(iSelectIdx  == -1)
		return;

	m_ListBox.GetText(iSelectIdx, strSelectName);

	map<CString, CImage*>::iterator iter = m_mapImage.find(strSelectName);

	m_wstrCurSelName = strSelectName;

	if(iter == m_mapImage.end())
		return;

	m_Picture.SetBitmap(*iter->second);	
}

void CEtcTool::OnBnClickedReturn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!m_ObjDataList.empty())
	{
		vector<CObj*> vecTile = m_pTileMap->GetFullTile();

		int iSize = vecTile.size();

		for(int i = 0; i < iSize; ++i)
		{
			if(m_ObjDataList.back()->vPos.x == vecTile[i]->GetInfo()->vPos.x
				&& m_ObjDataList.back()->vPos.z == vecTile[i]->GetInfo()->vPos.z)
				((CTile*)vecTile[i])->SetObjCnt(0);
		}

		::Safe_Delete(m_ObjDataList.back());
		m_ObjDataList.pop_back();
		m_pObjMgr->MonsterLastDelete();
	}
}

void CEtcTool::CreateObj(D3DXVECTOR3 vPos)
{
	int iSelect = m_ListBox.GetCurSel();

	if(iSelect  == -1)
		return;

	//m_pBufferMgr->SetVtxInfo(TEXT("UnitTex"), m_pVtxTex);

	vPos.y = 1.f;

	CObj* pObj = (CMonster*)CObjFactory<CMonster>::CreateObj(); // �����ؾ���
	pObj->SetPos(vPos);
	pObj->SetObjKey(m_wstrCurSelName);

	CObjMgr::GetInstance()->AddObj(OBJ_MONSTER, pObj);

	SLIME* tData = new SLIME;

	tData->vPos = vPos;

	wcscpy_s(tData->szObjkey,m_wstrCurSelName.c_str());

	m_ObjDataList.push_back(tData);

	vector<CObj*> vecTile = m_pTileMap->GetFullTile();

	int iSize = vecTile.size();

	for(int i = 0; i < iSize; ++i)
	{
		if(vecTile[i]->GetInfo()->vPos.x == m_ObjDataList.back()->vPos.x
			&& vecTile[i]->GetInfo()->vPos.z == m_ObjDataList.back()->vPos.z)
		{
			((CTile*)vecTile[i])->SetTileObj(pObj);

			return;
		}
	}
}

void CEtcTool::SaveToMyForm(const CString& strPath)
{
	CString strObjPath = strPath;
	CString strObjName = PathFindFileName(strObjPath);
	strObjName.Replace(TEXT(".dat"), TEXT(""));
	strObjName = strObjName + TEXT("_Etc.txt");
	CString strOriginName = PathFindFileName(strObjPath);
	strObjPath.Replace(strOriginName, strObjName);

	HANDLE hFile = CreateFile(strObjPath, GENERIC_WRITE
		, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD	dwByte;

	for(list<SLIME*>::iterator iter = m_ObjDataList.begin();
		iter != m_ObjDataList.end(); iter++)
	{
		WriteFile(hFile, (*iter), sizeof(SLIME), &dwByte, NULL);
	}
	CloseHandle(hFile);
}

void CEtcTool::LoadToMyForm(const CString& strPath)
{
	//m_pObjMgr->MonsterDataReset();
	for_each(m_ObjDataList.begin(), m_ObjDataList.end(), CDeleteObj());
	m_ObjDataList.clear();

	CString strObjPath = strPath;
	CString strObjName = PathFindFileName(strObjPath);
	strObjName.Replace(TEXT(".dat"), TEXT(""));
	strObjName = strObjName + TEXT("_Etc.txt");
	CString strOriginName = PathFindFileName(strObjPath);
	strObjPath.Replace(strOriginName, strObjName);

	HANDLE	hFile = CreateFile(strObjPath, GENERIC_READ
		, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD dwByte;

	vector<CObj*> vecTile = m_pTileMap->GetFullTile();

	int iSize = vecTile.size();

	while(1)
	{
		SLIME* pData = new SLIME;

		ReadFile(hFile, pData, sizeof(SLIME), &dwByte, NULL);

		if(dwByte == 0)
		{
			Safe_Delete(pData);
			break;
		}

		m_pBufferMgr->SetVtxInfo(TEXT("UnitTex"), m_pVtxTex);

		pData->vPos.y = 1.f;

		CObj* pObj = (CMonster*)CObjFactory<CMonster>::CreateObj();
		pObj->SetPos(pData->vPos);
		pObj->SetObjKey(pData->szObjkey);

		m_pObjMgr->AddObj(OBJ_MONSTER, pObj);

		m_ObjDataList.push_back(pData);

		for(int i = 0; i < iSize; ++i)
		{
			if(vecTile[i]->GetInfo()->vPos.x == m_ObjDataList.back()->vPos.x
				&& vecTile[i]->GetInfo()->vPos.z == m_ObjDataList.back()->vPos.z)
				((CTile*)vecTile[i])->SetTileObj(pObj);
		}
	}
	CloseHandle(hFile);
}

void CEtcTool::DeleteEtcData(D3DXVECTOR3 vPos)
{
	for(list<SLIME*>::iterator iter = m_ObjDataList.begin();
		iter != m_ObjDataList.end();)
	{
		if((*iter)->vPos.x == vPos.x 
			&& (*iter)->vPos.z == vPos.z)
		{
			::Safe_Delete(*iter);
			iter = m_ObjDataList.erase(iter);

			return;
		}
		else
			++iter;
	}
}