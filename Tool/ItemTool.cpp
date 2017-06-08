// ItemTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "ItemTool.h"
#include "FileInfo.h"
#include "Markup.h"
#include "ObjMgr.h"
#include "TextureMgr.h"
#include "BufferMgr.h"
#include "ObjFactory.h"
#include "Item.h"
#include "TileMap.h"
#include "Tile.h"

// CItemTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CItemTool, CDialog)

CItemTool::CItemTool(CWnd* pParent /*=NULL*/)
	: CDialog(CItemTool::IDD, pParent)
	, m_pObjMgr(CObjMgr::GetInstance())
	, m_pTextureMgr(CTextureMgr::GetInstance())
	, m_pBufferMgr(CBufferMgr::GetInstance())
	, m_pTileMap(CTileMap::GetInstance())
	, m_strObjKey(TEXT(""))
	, m_pVtxTex(NULL)
{
}

CItemTool::~CItemTool()
{
	Release();
}

void CItemTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	DDX_Control(pDX, IDC_CHECK1, m_CheckSell);
}


BEGIN_MESSAGE_MAP(CItemTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CItemTool::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON2, &CItemTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON1, &CItemTool::OnBnClickedDelete)
	ON_WM_DROPFILES()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CItemTool 메시지 처리기입니다.

BOOL CItemTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	//DataLoad();

	CFileInfo::DirtoryInfoExtraction_Ex( L"../Texture/Item", m_ImgPathlist);
	wstring		wstrCombine = L"";
	TCHAR		szCount[MIN_STR] = L"";

	for(list<IMGPATH*>::iterator iter = m_ImgPathlist.begin();
		iter != m_ImgPathlist.end(); iter++)
	{
		_itow_s((*iter)->iCount, szCount, 10);

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

			m_ListBox.AddString((*iter)->wstrObjKey.c_str());
		}
	}

	m_pVtxTex = new VTXTEX[4];

	CBufferMgr::GetInstance()->GetVtxInfo(TEXT("UnitTex"), m_pVtxTex);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CItemTool::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_pTileMap->SetItemMode(true);
}

void CItemTool::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_pTileMap->SetItemMode(false);

	CDialog::OnClose();
}

void CItemTool::OnLbnSelchangeList1()
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

	m_strObjKey = strSelectName;

	UpdateData(FALSE);
}

void CItemTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CMarkup xml;

	xml.SetDoc(TEXT("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n\n"));

	xml.AddElem(TEXT("Items"));
	xml.AddChildElem(TEXT("Item"));
	xml.IntoElem();
	for(list<wstring>::iterator iter = m_ImgPathList.begin();
		iter != m_ImgPathList.end(); ++iter)
	{
		xml.AddChildElem(TEXT("Item"), (*iter).c_str());
	}
	xml.OutOfElem();

	xml.Save(TEXT("../Data/Item/ItemInfo.xml"));

	UpdateData(FALSE);
}
void CItemTool::OnBnClickedDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int iSelectIndex = m_ListBox.GetCurSel();

	if(iSelectIndex == -1)
		return;

	CString strSelectName = TEXT("");
	m_ListBox.GetText(iSelectIndex, strSelectName);

	for(vector<ITEMDATA*>::iterator iter = m_vecItem.begin();
		iter != m_vecItem.end();)
	{
		if((*iter)->szName == strSelectName)
		{
			m_vecItem.erase(iter);
			::Safe_Delete(*iter);

			break;
		}
		else
			++iter;
	}

	m_ListBox.DeleteString(iSelectIndex);

	if(iSelectIndex != 0)
		m_ListBox.SetCurSel(iSelectIndex - 1);
	else
		m_ListBox.SetCurSel(0);

	UpdateData(FALSE);
}

void CItemTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);

	TCHAR szFullPath[MAX_PATH] = TEXT("");
	int iFileNum = DragQueryFile(hDropInfo, -1, NULL, 0);

	for(int i = 0; i < iFileNum; ++i)
	{
		DragQueryFile(hDropInfo, i, (LPWSTR)szFullPath, MAX_PATH);

		wstring wstrRelativePath = CFileInfo::ConvertRelativePath(szFullPath);

		CString strFileName = CFileInfo::GetFileName(szFullPath).TrimRight(TEXT("0"));

		map<CString, CImage*>::iterator iter = m_MapImage.find(strFileName);

		if(iter == m_MapImage.end())
		{
			CImage* pImage = new CImage;
			pImage->Load(wstrRelativePath.c_str());

			m_MapImage.insert(map<CString, CImage*>::value_type(strFileName, pImage));
			m_ImgPathList.push_back(wstrRelativePath);

			m_ListBox.AddString(strFileName);
		}

		if(FAILED(m_pTextureMgr->InsertTexture(wstrRelativePath, (wstring)strFileName, TEXTYPE_MULTI, TEXT("Normal"), TEXT(""), 1)))
			return;
	}

	UpdateData(FALSE);

	CDialog::OnDropFiles(hDropInfo);
}

void CItemTool::CreateItem(const D3DXVECTOR3& vPos)
{
	CItem* pItem = (CItem*)CObjFactory<CItem>::CreateObj(D3DXVECTOR3(vPos.x, 0.1f, vPos.z));

	pItem->SetObjKey((wstring)m_strObjKey);
	m_pObjMgr->AddObj(OBJ_ITEM, pItem);

	ITEMDATA* pItemData = new ITEMDATA;

	_tcscpy_s(pItemData->szName, m_strObjKey.GetBuffer(m_strObjKey.GetLength()));
	m_strObjKey.ReleaseBuffer();

	pItemData->vPos = vPos;

	if(m_CheckSell.GetCheck())
		pItemData->bSell = true;
	else
		pItemData->bSell = false;

	m_vecItem.push_back(pItemData);

	vector<CObj*> vecTile = m_pTileMap->GetFullTile();

	int iSize = vecTile.size();

	for(int i = 0; i < iSize; ++i)
	{
		if(vecTile[i]->GetInfo()->vPos.x == m_vecItem.back()->vPos.x
			&& vecTile[i]->GetInfo()->vPos.z == m_vecItem.back()->vPos.z)
		{
			((CTile*)vecTile[i])->SetTileObj(pItem);

			return;
		}
	}
}

void CItemTool::DataLoad()
{
	UpdateData(TRUE);

	CMarkup xml;

	if(!xml.Load(TEXT("../Data/Item/ItemInfo.xml")))
		return;

	xml.FindElem(TEXT("Items"));
	xml.IntoElem();

	while(xml.FindChildElem(TEXT("Item")))
	{
		CString strFileName = CFileInfo::GetFileName(wstring(xml.GetChildData()), 1).TrimRight(TEXT("0"));;

		map<CString, CImage*>::iterator iter = m_MapImage.find(strFileName);

		if(iter == m_MapImage.end())
		{
			CImage* pImage = new CImage;
			pImage->Load(xml.GetChildData());

			m_MapImage.insert(map<CString, CImage*>::value_type(strFileName, pImage));
			m_ImgPathList.push_back(wstring(xml.GetChildData()));

			m_ListBox.AddString(strFileName);
		}

		if(FAILED(m_pTextureMgr->InsertTexture((wstring)xml.GetChildData(), (wstring)strFileName, TEXTYPE_MULTI, TEXT("Normal"), TEXT(""), 1)))
			return;
	}
	xml.OutOfElem();

	UpdateData(FALSE);
}

void CItemTool::Release()
{
	for_each(m_vecItem.begin(), m_vecItem.end(), ::CDeleteObj());
	vector<ITEMDATA*>().swap(m_vecItem);

	if(m_pVtxTex != NULL)
		::Safe_Delete_Array(m_pVtxTex);

	for_each(m_ImgPathlist.begin(), m_ImgPathlist.end(), CDeleteObj());
	m_ImgPathlist.clear();

	for(map<CString, CImage*>::iterator iter = m_MapImage.begin();
		iter != m_MapImage.end(); ++iter)
	{
		iter->second->Destroy();
		::Safe_Delete(iter->second);
	}
	m_MapImage.clear();

	m_ImgPathList.clear();
}

void CItemTool::SaveToMyForm(const CString& strPath)
{
	CString strItemPath = strPath;
	CString strItemName = PathFindFileName(strItemPath);
	strItemName.Replace(TEXT(".dat"), TEXT(""));
	strItemName = strItemName + TEXT("_Item.txt");
	CString strOriginName = PathFindFileName(strItemPath);
	strItemPath.Replace(strOriginName, strItemName);

	char objch[MAX_PATH] = "";

	WideCharToMultiByte(CP_ACP, NULL, strItemPath, -1, objch, MAX_PATH, NULL, FALSE);

	FILE* pFile = NULL;
	fopen_s(&pFile, objch, "wb");

	if(pFile == NULL)
		AfxMessageBox(TEXT("File Open Fail"));

	size_t iSize = m_vecItem.size();

	for(size_t i = 0; i < iSize; ++i)
	{
		fwrite(m_vecItem[i], sizeof(ITEMDATA), 1, pFile);
	}

	int iState = fclose(pFile);

	if(iState != 0)
		AfxMessageBox(TEXT("File Delete Fail"));
}

void CItemTool::LoadToMyForm(const CString& strPath)
{
	m_pObjMgr->ObjDelete(OBJ_ITEM);

	for_each(m_vecItem.begin(), m_vecItem.end(), ::CDeleteObj());
	vector<ITEMDATA*>().swap(m_vecItem);

	CString strItemPath = strPath;
	CString strItemName = PathFindFileName(strItemPath);
	strItemName.Replace(TEXT(".dat"), TEXT(""));
	strItemName = strItemName + TEXT("_Item.txt");
	CString strOriginName = PathFindFileName(strItemPath);
	strItemPath.Replace(strOriginName, strItemName);

	char ch[MAX_PATH] = "";
	WideCharToMultiByte(CP_ACP, NULL, strItemPath, -1, ch, MAX_PATH, NULL, FALSE);

	FILE* pFile = NULL;
	fopen_s(&pFile, ch, "rb");

	if(pFile == NULL)
		AfxMessageBox(TEXT("File Open Fail"));

	while(!feof((pFile)))
	{
		ITEMDATA* pItemData = new ITEMDATA;

		int iCheck = fread(pItemData, sizeof(ITEMDATA), 1, pFile);

		if(iCheck == 0)
		{
			::Safe_Delete(pItemData);

			break;
		}

		m_vecItem.push_back(pItemData);
	}

	int iState = fclose(pFile);

	if(iState != 0)
		AfxMessageBox(TEXT("File Delete Fail"));

	SetItem();
}

void CItemTool::SetItem()
{
	vector<CObj*> vecTile = m_pTileMap->GetFullTile();

	int iSize2 = vecTile.size();

	size_t iSize = m_vecItem.size();

	for(size_t i = 0; i < iSize; ++i)
	{
		CItem* pItem = (CItem*)CObjFactory<CItem>::CreateObj(D3DXVECTOR3(m_vecItem[i]->vPos.x, 0.1f, m_vecItem[i]->vPos.z));

		pItem->SetObjKey(m_vecItem[i]->szName);
		m_pObjMgr->AddObj(OBJ_ITEM, pItem);

		for(int i = 0; i < iSize2; ++i)
		{
			if(vecTile[i]->GetInfo()->vPos.x == m_vecItem.back()->vPos.x
				&& vecTile[i]->GetInfo()->vPos.z == m_vecItem.back()->vPos.z)
			{
				((CTile*)vecTile[i])->SetTileObj(pItem);
			}
		}
	}
}

void CItemTool::DeleteItemData(D3DXVECTOR3 vPos)
{
	for(vector<ITEMDATA*>::iterator iter = m_vecItem.begin();
		iter != m_vecItem.end();)
	{
		if((*iter)->vPos.x == vPos.x 
			&& (*iter)->vPos.z == vPos.z)
		{
			::Safe_Delete(*iter);
			iter = m_vecItem.erase(iter);

			return;
		}
		else
			++iter;
	}
}