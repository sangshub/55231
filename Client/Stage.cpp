#include "stdafx.h"
#include "Stage.h"
#include "Tile.h"
#include "Item.h"
#include "Cube.h"
#include "BlueSlime.h"
#include "GreenSlime.h"
#include "BlueBat.h"
#include "RedBat.h"
#include "Ghost.h"
#include "Monkey.h"
#include "Skeleton.h"
#include "YellowSkeleton.h"
#include "GreenDragon.h"
#include "Wraith.h"
#include "YellowDireBat.h"
#include "LightMinotaur.h"
#include "BlueBanshee.h"
#include "Golem.h"
#include "BlackSkeleton.h"
#include "OrangeSlime.h"
#include "PushLeft.h"
#include "PushRight.h"
#include "PushUp.h"
#include "PushDown.h"
#include "SpikeTrap.h"
#include "Stairs.h"
#include "Shopkeeper.h"
#include "Chest.h"
#include "Coin_Num.h"
#include "Coin_Multi.h"
#include "Diamond_Num.h"
#include "Door.h"
#include "Heart.h"
#include "Diamond.h"
#include "Coin.h"
#include "Hpbar.h"
#include "Device.h"
#include "BufferMgr.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "Player.h"
#include "Markup.h"
#include "ItemTableMgr.h"
#include "BeatMgr.h"
#include "Rand.h"

bool CStage::m_bStageInit = false;

CStage::CStage()
: m_bBossStage(false)
, m_iTileX(0)
, m_iTileZ(0)
, m_iDrawId(0)
, m_iVolume(0)
, m_pObjMgr(CObjMgr::GetInstance())
, m_pBufferMgr(CBufferMgr::GetInstance())
, m_pBeatMgr(CBeatMgr::GetInstance())
, m_pMiniMap(NULL)
, m_StartMap(NULL)
, m_EndMap(NULL)
, m_vSort(D3DXVECTOR3(0.f, 0.f, 0.f))
{
	m_vecTileInfo.reserve(1000);
	m_vecRandomMap.reserve(25);
	m_vecChooseMap.reserve(25);
}

CStage::~CStage()
{
	Release();
}

void CStage::Release()
{
	if(m_StartMap != NULL)
		for_each(m_StartMap->vecConnect.begin(), m_StartMap->vecConnect.end(), ::CDeleteObj());
	if(m_EndMap != NULL)
		for_each(m_EndMap->vecConnect.begin(), m_EndMap->vecConnect.end(), ::CDeleteObj());

	::Safe_Delete(m_StartMap);
	::Safe_Delete(m_EndMap);

	size_t iSize = m_vecRandomMap.size();

	for(size_t i = 0; i < iSize; ++i)
	{
		for_each(m_vecRandomMap[i]->vecConnect.begin(), m_vecRandomMap[i]->vecConnect.end(), ::CDeleteObj());	
	}

	for_each(m_vecRandomMap.begin(), m_vecRandomMap.end(), ::CDeleteObj());
	vector<MAP*>().swap(m_vecRandomMap);

	for_each(m_vecTileInfo.begin(), m_vecTileInfo.end(), ::CDeleteObj());
	vector<TILE*>().swap(m_vecTileInfo);

	iSize = m_vecChooseMap.size();

	for(size_t i = 0; i < iSize; ++i)
	{
		for_each(m_vecChooseMap[i]->vecConnect.begin(), m_vecChooseMap[i]->vecConnect.end(), ::CDeleteObj());	
	}

	for_each(m_vecChooseMap.begin(), m_vecChooseMap.end(), ::CDeleteObj());
	vector<MAP*>().swap(m_vecChooseMap);
}

const int CStage::GetTileX() const
{
	return m_iTileX;
}

const int CStage::GetTileZ() const
{
	return m_iTileZ;
}

bool CStage::GetStageInit()
{
	return m_bStageInit;
}

void CStage::FileLoad(const wstring& wstrFilePath)
{
	// 타일 로드
	vector<TILE*> vecTileInfo;

	char szPath[MAX_PATH] = "";
	WideCharToMultiByte(CP_ACP, NULL, wstrFilePath.c_str(), -1, szPath, MAX_PATH, NULL, FALSE);

	FILE* pFile = NULL;
	fopen_s(&pFile, szPath, "rb");

	if(pFile == NULL)
		ERR_MSG(TEXT("File Open Fail"));

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
		ERR_MSG(TEXT("File Delete Fail"));

	size_t iSize = vecTileInfo.size();

	for(size_t i = 0; i < iSize; ++i)
	{
		CTile* pTile = (CTile*)CObjFactory<CTile>::CreateObj(vecTileInfo[i]->vPos);

		pTile->SetDrawId(vecTileInfo[i]->iDrawId);

		m_pObjMgr->SetTile(pTile);
	}

	for_each(vecTileInfo.begin(), vecTileInfo.end(), ::CDeleteObj());

	// 타일 정보 로드
	CMarkup xml;

	CString strFileName = wstrFilePath.c_str();

	PathFindFileName(strFileName);
	strFileName.TrimRight(TEXT("dat"));

	strFileName = strFileName + TEXT("xml");

	xml.Load((wstring)strFileName);

	xml.FindElem(TEXT("TileInfo"));

	while(xml.FindChildElem(TEXT("TileX")))
	{
		m_iTileX = _ttoi(xml.GetChildData().c_str());
	}
	while(xml.FindChildElem(TEXT("TileZ")))
	{
		m_iTileZ = _ttoi(xml.GetChildData().c_str());
	}
}

void CStage::CubeLoad(const wstring& wstrCubePath, MAP* pMap)
{
	CString strCubePath = wstrCubePath.c_str();
	strCubePath.TrimRight(TEXT(".dat"));
	strCubePath = strCubePath + TEXT("_Cube.txt");

	HANDLE hFile = NULL;
	DWORD dwByte = 0;

	hFile = CreateFile(strCubePath, GENERIC_READ, 0, NULL, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL, NULL);

	while(true)
	{
		CUBE tCubeData;
		ReadFile(hFile, &tCubeData, sizeof(CUBE), &dwByte, NULL);

		if(dwByte == 0)
			break;

		if(pMap != NULL)
			tCubeData.vPos = tCubeData.vPos + D3DXVECTOR3(pMap->fModifyX, 0.f, pMap->fModifyZ) - m_vSort;

		CCube* pCube = (CCube*)CObjFactory<CCube>::CreateObj();
		pCube->SetCubeInfo(tCubeData);

		m_pObjMgr->AddObj(OBJ_CUBE, pCube);
	}
	CloseHandle(hFile);
}

void CStage::ItemLoad(const wstring& wstrItemPath, MAP* pMap)
{
	CString strItemPath = wstrItemPath.c_str();
	strItemPath.TrimRight(TEXT(".dat"));
	strItemPath = strItemPath + TEXT("_Item.txt");

	// 아이템 로드
	vector<ITEMDATA*> vecItemInfo;

	char szPath[MAX_PATH] = "";
	WideCharToMultiByte(CP_ACP, NULL, strItemPath, -1, szPath, MAX_PATH, NULL, FALSE);

	FILE* pFile = NULL;
	fopen_s(&pFile, szPath, "rb");

	if(pFile == NULL)
		ERR_MSG(TEXT("File Open Fail"));

	while(!feof((pFile)))
	{
		ITEMDATA* pItem = new ITEMDATA;

		int iCheck = fread(pItem, sizeof(ITEMDATA), 1, pFile);

		if(iCheck == 0)
		{
			::Safe_Delete(pItem);

			break;
		}

		if(pMap != NULL)
			pItem->vPos = pItem->vPos + D3DXVECTOR3(pMap->fModifyX, 0.f, pMap->fModifyZ) - m_vSort;

		vecItemInfo.push_back(pItem);
	}

	int iState = fclose(pFile);

	if(iState != 0)
		ERR_MSG(TEXT("File Delete Fail"));

	size_t iSize = vecItemInfo.size();

	for(size_t i = 0; i < iSize; ++i)
	{
		CItem* pItem = (CItem*)CObjFactory<CItem>::CreateObj(D3DXVECTOR3(vecItemInfo[i]->vPos.x, 1.f, vecItemInfo[i]->vPos.z));

		pItem->SetObjKey(vecItemInfo[i]->szName);
		pItem->InitSetType();
		pItem->SetSelling(vecItemInfo[i]->bSell);
		pItem->SetSellPrice();
		if(pItem->GetisSelling())
			pItem->SetPrice();

		m_pObjMgr->AddObj(OBJ_ITEM, pItem);
	}

	for_each(vecItemInfo.begin(), vecItemInfo.end(), ::CDeleteObj());
}

void CStage::MonsterLoad(const wstring& wstrItemPath, MAP* pMap)
{
	CString strItemPath = wstrItemPath.c_str();
	strItemPath.TrimRight(TEXT(".dat"));
	strItemPath = strItemPath + TEXT("_Obj.txt");

	vector<SLIME*> vecMonsterInfo;

	char szPath[MAX_PATH] = "";
	WideCharToMultiByte(CP_ACP, NULL, strItemPath, -1, szPath, MAX_PATH, NULL, FALSE);

	FILE* pFile = NULL;
	fopen_s(&pFile, szPath, "rb");

	if(pFile == NULL)
		ERR_MSG(TEXT("File Open Fail"));

	while(!feof((pFile)))
	{
		SLIME* pMonster = new SLIME;

		int iCheck = fread(pMonster, sizeof(SLIME), 1, pFile);

		if(iCheck == 0)
		{
			::Safe_Delete(pMonster);

			break;
		}

		if(pMap != NULL)
			pMonster->vPos = pMonster->vPos + D3DXVECTOR3(pMap->fModifyX, 0.f, pMap->fModifyZ) - m_vSort;

		vecMonsterInfo.push_back(pMonster);
	}

	int iState = fclose(pFile);

	if(iState != 0)
		ERR_MSG(TEXT("File Delete Fail"));

	size_t iSize = vecMonsterInfo.size();

	for(size_t i = 0; i < iSize; ++i)
	{
		CMonster* pMonster = NULL;

		wstring wstrObjKey = vecMonsterInfo[i]->szObjkey;

		if(wstrObjKey == TEXT("Blue_Slime"))
			pMonster = (CMonster*)CObjFactory<CBlueSlime>::CreateObj(vecMonsterInfo[i]->vPos);
		else if(wstrObjKey == TEXT("Blue_Bat"))
			pMonster = (CMonster*)CObjFactory<CBlueBat>::CreateObj(vecMonsterInfo[i]->vPos);
		else if(wstrObjKey == TEXT("Red_Bat"))
			pMonster = (CMonster*)CObjFactory<CRedBat>::CreateObj(vecMonsterInfo[i]->vPos);
		else if(wstrObjKey == TEXT("Ghost"))
			pMonster = (CMonster*)CObjFactory<CGhost>::CreateObj(vecMonsterInfo[i]->vPos);
		else if(wstrObjKey == TEXT("Monkey"))
			pMonster = (CMonster*)CObjFactory<CMonkey>::CreateObj(vecMonsterInfo[i]->vPos);
		else if(wstrObjKey == TEXT("Skeleton"))
			pMonster = (CMonster*)CObjFactory<CSkeleton>::CreateObj(vecMonsterInfo[i]->vPos);
		else if(wstrObjKey == TEXT("Yellow_Skeleton"))
			pMonster = (CMonster*)CObjFactory<CYellowSkeleton>::CreateObj(vecMonsterInfo[i]->vPos);
		else if(wstrObjKey == TEXT("Wraith"))
			pMonster = (CMonster*)CObjFactory<CWratih>::CreateObj(vecMonsterInfo[i]->vPos);
		else if(wstrObjKey == TEXT("Green_Dragon"))
			pMonster = (CMonster*)CObjFactory<CGreenDragon>::CreateObj(vecMonsterInfo[i]->vPos);
		else if(wstrObjKey == TEXT("YellowDireBat"))
			pMonster = (CMonster*)CObjFactory<CYellowDireBat>::CreateObj(vecMonsterInfo[i]->vPos);
		else if(wstrObjKey == TEXT("Light_Minotaur"))
			pMonster = (CMonster*)CObjFactory<CLightMinotaur>::CreateObj(vecMonsterInfo[i]->vPos);
		else if(wstrObjKey == TEXT("Blue_Banshee"))
			pMonster = (CMonster*)CObjFactory<CBlueBanshee>::CreateObj(vecMonsterInfo[i]->vPos);
		else if(wstrObjKey == TEXT("Golem"))
			pMonster = (CMonster*)CObjFactory<CGolem>::CreateObj(vecMonsterInfo[i]->vPos);
		else if(wstrObjKey == TEXT("Black_Skeleton"))
			pMonster = (CMonster*)CObjFactory<CBlackSkeleton>::CreateObj(vecMonsterInfo[i]->vPos);
		else if(wstrObjKey == TEXT("Orange_Slime"))
			pMonster = (CMonster*)CObjFactory<COrangeSlime>::CreateObj(vecMonsterInfo[i]->vPos);
		else
			pMonster = (CMonster*)CObjFactory<CGreenSlime>::CreateObj(vecMonsterInfo[i]->vPos);

		pMonster->SetObjKey(vecMonsterInfo[i]->szObjkey);

		m_pObjMgr->AddObj(OBJ_MONSTER, pMonster);
	}

	for_each(vecMonsterInfo.begin(), vecMonsterInfo.end(), ::CDeleteObj());
}

void CStage::ObjLoad(const wstring& wstrItemPath, MAP* pMap)
{
	CString strObjPath = wstrItemPath.c_str();
	strObjPath.TrimRight(TEXT(".dat"));
	strObjPath = strObjPath + TEXT("_Etc.txt");

	vector<SLIME*> vecObjectInfo;

	char szPath[MAX_PATH] = "";
	WideCharToMultiByte(CP_ACP, NULL, strObjPath, -1, szPath, MAX_PATH, NULL, FALSE);

	FILE* pFile = NULL;
	fopen_s(&pFile, szPath, "rb");

	if(pFile == NULL)
		ERR_MSG(TEXT("File Open Fail"));

	while(!feof((pFile)))
	{
		SLIME* pMonster = new SLIME;

		int iCheck = fread(pMonster, sizeof(SLIME), 1, pFile);

		if(iCheck == 0)
		{
			::Safe_Delete(pMonster);

			break;
		}

		if(pMap != NULL)
			pMonster->vPos = pMonster->vPos + D3DXVECTOR3(pMap->fModifyX, 0.f, pMap->fModifyZ) - m_vSort;

		vecObjectInfo.push_back(pMonster);
	}

	int iState = fclose(pFile);

	if(iState != 0)
		ERR_MSG(TEXT("File Delete Fail"));

	size_t iSize = vecObjectInfo.size();

	for(size_t i = 0; i < iSize; ++i)
	{
		CTrap* pTrap = NULL;
		CEtc* pEtc = NULL;

		wstring wstrObjKey = vecObjectInfo[i]->szObjkey;

		D3DXVECTOR3 vPos = vecObjectInfo[i]->vPos;
		vPos.y = 0.3f;

		if(wstrObjKey == TEXT("Push_Left"))
			pTrap = (CTrap*)CObjFactory<CPushLeft>::CreateObj(vPos);
		else if(wstrObjKey == TEXT("Push_Right"))
			pTrap = (CTrap*)CObjFactory<CPushRight>::CreateObj(vPos);
		else if(wstrObjKey == TEXT("Push_Up"))
			pTrap = (CTrap*)CObjFactory<CPushUp>::CreateObj(vPos);
		else if(wstrObjKey == TEXT("Push_Down"))
			pTrap = (CTrap*)CObjFactory<CPushDown>::CreateObj(vPos);
		else if(wstrObjKey == TEXT("Trap"))
			pTrap = (CTrap*)CObjFactory<CSpikeTrap>::CreateObj(vPos);
		else if(wstrObjKey == TEXT("Stairs"))
			pTrap = (CTrap*)CObjFactory<CStairs>::CreateObj(vPos);
		else if(wstrObjKey == TEXT("Start"))
			m_vStart = vPos;

		vPos.y = 1.f;

		if(wstrObjKey == TEXT("Shopkeeper"))
		{
			pEtc = (CEtc*)CObjFactory<CShopkeeper>::CreateObj(vPos);
			m_vecShop = vPos;
		}
		else if(wstrObjKey == TEXT("Chest"))
			pEtc = (CEtc*)CObjFactory<CChest>::CreateObj(vPos);
		else if(wstrObjKey == TEXT("Door"))
			pEtc = (CEtc*)CObjFactory<CDoor>::CreateObj(vPos);
		else if(wstrObjKey == TEXT("Door2"))
		{
			pEtc = (CEtc*)CObjFactory<CDoor>::CreateObj(vPos);
			pEtc->SetAngle(D3DXToRadian(90.f));
		}

		if(pTrap != NULL)
		{
			pTrap->SetObjKey(vecObjectInfo[i]->szObjkey);

			m_pObjMgr->AddObj(OBJ_TRAP, pTrap);
		}
		if(pEtc != NULL)
		{
			pEtc->SetObjKey(vecObjectInfo[i]->szObjkey);

			m_pObjMgr->AddObj(OBJ_ETC, pEtc);
		}
	}

	for_each(vecObjectInfo.begin(), vecObjectInfo.end(), ::CDeleteObj());
}

void CStage::AllDataLoad(const wstring& wstrPath, MAP* pMap)
{
	CubeLoad(wstrPath, pMap);
	ItemLoad(wstrPath, pMap);
	MonsterLoad(wstrPath, pMap);
	ObjLoad(wstrPath, pMap);
}

void CStage::ShopVolume()
{
	D3DXVECTOR3 vDir = m_vecShop - m_pObjMgr->GetPlayer()->GetInfo()->vPos;

	float fDistance = D3DXVec3Length(&vDir);

	m_iVolume = (int)-fDistance * 80;
	if(m_iVolume < -3000)
		m_iVolume = -3000;
}

void CStage::StageBaseObjInit()
{
	CItemTableMgr::GetInstance()->Initialize();		//아이템 테이블 준비

	m_pObjMgr->AddObj(OBJ_PLAYER, CObjFactory<CPlayer>::CreateObj());

	m_pObjMgr->AddObj(OBJ_UI, CObjFactory<CHeart>::CreateObj());
	m_pObjMgr->AddObj(OBJ_UI, CObjFactory<CDiamond>::CreateObj());
	m_pObjMgr->AddObj(OBJ_UI, CObjFactory<CCoin>::CreateObj());
	m_pObjMgr->AddObj(OBJ_UI, CObjFactory<CCoin_Num>::CreateObj());
	m_pObjMgr->AddObj(OBJ_UI, CObjFactory<CDiamond_Num>::CreateObj());
	m_pObjMgr->AddObj(OBJ_EFFECT, CObjFactory<CCoin_Multi>::CreateObj());

	m_bStageInit = true;
}

void CStage::MapLoad(const wstring& wstrFilePath, const int iOption)
{
	// 타일 정보 로드
	int iTileX = 0;
	int iTileZ = 0;

	CMarkup xml;

	CString strFileName = wstrFilePath.c_str();

	PathFindFileName(strFileName);
	strFileName.TrimRight(TEXT("dat"));

	strFileName = strFileName + TEXT("xml");

	xml.Load((wstring)strFileName);

	xml.FindElem(TEXT("TileInfo"));

	while(xml.FindChildElem(TEXT("TileX")))
	{
		iTileX = _ttoi(xml.GetChildData().c_str());
	}
	while(xml.FindChildElem(TEXT("TileZ")))
	{
		iTileZ = _ttoi(xml.GetChildData().c_str());
	}

	MAP* pMap = new MAP;
	pMap->wstrMapPath = wstrFilePath;
	pMap->iTileX = iTileX;
	pMap->iTileZ = iTileZ;
	pMap->fModifyX = 0.f;
	pMap->fModifyZ = 0.f;

	CString strItemPath = wstrFilePath.c_str();

	PathFindFileName(strItemPath);
	strItemPath.TrimRight(TEXT(".dat"));

	strItemPath = strItemPath + TEXT("_Etc.txt");

	char szPath[MAX_PATH] = "";
	WideCharToMultiByte(CP_ACP, NULL, strItemPath, -1, szPath, MAX_PATH, NULL, FALSE);

	FILE* pFile = NULL;
	fopen_s(&pFile, szPath, "rb");

	if(pFile == NULL)
		ERR_MSG(TEXT("File Open Fail"));

	while(!feof((pFile)))
	{
		SLIME* pEtc = new SLIME;

		int iCheck = fread(pEtc, sizeof(SLIME), 1, pFile);

		if(iCheck == 0)
		{
			::Safe_Delete(pEtc);

			break;
		}

		wstring wstrObjKey = pEtc->szObjkey;

		if(wstrObjKey == TEXT("Connect"))
		{
			CONNECT* pConnect = new CONNECT;

			pConnect->bIsConnect = false;
			pConnect->vPos = pEtc->vPos;
			if(pEtc->vPos.x == 0.f)
				pConnect->eDir = CON_LEFT;
			else if(pEtc->vPos.x == float((iTileX<<1) - 2))
				pConnect->eDir = CON_RIGHT;
			else if(pEtc->vPos.z == 0.f)
				pConnect->eDir = CON_DOWN;
			else if(pEtc->vPos.z == float((iTileZ<<1) - 2))
				pConnect->eDir = CON_UP;

			pMap->vecConnect.push_back(pConnect);
		}

		::Safe_Delete(pEtc);
	}

	int iState = fclose(pFile);

	if(iState != 0)
		ERR_MSG(TEXT("File Delete Fail"));

	switch(iOption)
	{
	case 0:
		m_StartMap = pMap;
		break;
	case 1:
		m_EndMap = pMap;
		break;
	case 2:
		m_vecRandomMap.push_back(pMap);
		break;
	}
}

void CStage::MapCreate()
{
	MapConnect();
	
	size_t iSize = m_vecTileInfo.size();

	for(size_t i = 0; i < iSize; ++i)
	{
		m_vecTileInfo[i]->vPos = m_vecTileInfo[i]->vPos - m_vSort;
	}

	float fMaxX = m_vecTileInfo[0]->vPos.x;
	float fMaxZ = m_vecTileInfo[0]->vPos.z;

	for(size_t i = 0; i < iSize; ++i)
	{
		if(m_vecTileInfo[i]->vPos.x > fMaxX)
			fMaxX = m_vecTileInfo[i]->vPos.x;
		if(m_vecTileInfo[i]->vPos.z > fMaxZ)
			fMaxZ = m_vecTileInfo[i]->vPos.z;
	}

	m_iTileX = int(fMaxX * 0.5f) + 1;
	m_iTileZ = int(fMaxZ * 0.5f) + 1;

	// 사이즈가 홀수가 나오면 짝수로 맞춰줌(피버시 깜빡임)
	if((m_iTileX & 1) == 1)
		m_iTileX += 1;
	if((m_iTileZ & 1) == 1)
		m_iTileZ += 1;

	m_pObjMgr->SetTileSize(m_iTileX, m_iTileZ);

	for(int i = 0; i < m_iTileX; ++i)
	{
		for(int j = 0; j < m_iTileZ; ++j)
		{
			CTile* pTile = (CTile*)CObjFactory<CTile>::CreateObj(D3DXVECTOR3(float(i * VTXITV<<1), 0.f, float(j * VTXITV<<1)));

			pTile->SetDrawId(16);
			
			m_pObjMgr->SetTile(pTile);
		}
	}

	vector<CObj*> vecFullTile = m_pObjMgr->GetFullTile();

	for(size_t i = 0; i < iSize; ++i)
	{
		CTile* pTile = (CTile*)vecFullTile[m_pObjMgr->GetIndex(m_vecTileInfo[i]->vPos)];

		pTile->SetDrawId(m_vecTileInfo[i]->iDrawId);
	}

	AllDataLoad(m_StartMap->wstrMapPath, m_StartMap);
	AllDataLoad(m_EndMap->wstrMapPath, m_EndMap);

	size_t iEndCount = m_EndMap->vecConnect.size();

	for(size_t i = 0; i < iEndCount; ++i)
	{
		if(!m_EndMap->vecConnect[i]->bIsConnect)
		{
			CUBE tCubeData;

			tCubeData.vPos = m_EndMap->vecConnect[i]->vPos - m_vSort;
			tCubeData.dwType = CUBE_NOTB;
			tCubeData.iDrawId = 10;

			CCube* pCube = (CCube*)CObjFactory<CCube>::CreateObj();
			pCube->SetCubeInfo(tCubeData);

			m_pObjMgr->AddObj(OBJ_CUBE, pCube);
		}
	}

	size_t iMapCount = m_vecChooseMap.size();

	for(size_t i = 0; i < iMapCount; ++i)
	{
		AllDataLoad(m_vecChooseMap[i]->wstrMapPath, m_vecChooseMap[i]);

		int iConnect = m_vecChooseMap[i]->vecConnect.size();

		for(int j = 0; j < iConnect; ++j)
		{
			if(!m_vecChooseMap[i]->vecConnect[j]->bIsConnect)
			{
				CUBE tCubeData;

				tCubeData.vPos = m_vecChooseMap[i]->vecConnect[j]->vPos - m_vSort;
				tCubeData.dwType = CUBE_NOTB;
				tCubeData.iDrawId = 10;

				CCube* pCube = (CCube*)CObjFactory<CCube>::CreateObj();
				pCube->SetCubeInfo(tCubeData);

				m_pObjMgr->AddObj(OBJ_CUBE, pCube);
			}
		}
	}

	m_pObjMgr->GetPlayer()->SetPos(m_vStart);
	((CPlayer*)m_pObjMgr->GetPlayer())->SetPlayerIndex(m_vStart);
}

const bool CStage::MapInput(MAP* pMap)
{
	wstring wstrFilePath = pMap->wstrMapPath;

	vector<TILE*> vecTileInfo;

	char szPath[MAX_PATH] = "";
	WideCharToMultiByte(CP_ACP, NULL, wstrFilePath.c_str(), -1, szPath, MAX_PATH, NULL, FALSE);

	FILE* pFile = NULL;
	fopen_s(&pFile, szPath, "rb");

	if(pFile == NULL)
	{
		ERR_MSG(TEXT("File Open Fail"));

		return false;
	}

	while(!feof((pFile)))
	{
		TILE* pTile = new TILE;

		int iCheck = fread(pTile, sizeof(TILE), 1, pFile);

		if(iCheck == 0)
		{
			::Safe_Delete(pTile);

			break;
		}

		pTile->vPos = pTile->vPos + D3DXVECTOR3(pMap->fModifyX, 0.f, pMap->fModifyZ);

		int iSize = m_vecTileInfo.size();

		for(int i = 0; i < iSize; ++i)
		{
			if(pTile->vPos == m_vecTileInfo[i]->vPos)
			{
				for_each(vecTileInfo.begin(), vecTileInfo.end(), ::CDeleteObj());

				::Safe_Delete(pTile);

				return false;
			}
		}

		vecTileInfo.push_back(pTile);
	}

	int iSize = vecTileInfo.size();

	for(int i = 0; i < iSize; ++i)
	{
		m_vecTileInfo.push_back(vecTileInfo[i]);
	}

	int iState = fclose(pFile);

	if(iState != 0)
	{
		ERR_MSG(TEXT("File Delete Fail"));

		return false;
	}

	return true;
}

void CStage::MapConnect()
{
	MapInput(m_StartMap);

	int iSize = m_vecRandomMap.size();

	while(1)
	{
		bool bFind = false;
		MAP* pMap = m_vecRandomMap[CRand::Random(0, iSize - 1)];
		size_t iConnectSize = pMap->vecConnect.size();

		switch(m_StartMap->vecConnect.front()->eDir)
		{
		case CON_UP:
			for(size_t i = 0; i < iConnectSize; ++i)
			{
				if(pMap->vecConnect[i]->eDir == CON_DOWN)
				{
					pMap->fModifyX = m_StartMap->vecConnect.front()->vPos.x - pMap->vecConnect[i]->vPos.x;
					pMap->fModifyZ = m_StartMap->vecConnect.front()->vPos.z - pMap->vecConnect[i]->vPos.z + 2.f;
					pMap->vecConnect[i]->bIsConnect = true;

					bFind = true;
				}
			}
			break;
		case CON_DOWN:
			for(size_t i = 0; i < iConnectSize; ++i)
			{
				if(pMap->vecConnect[i]->eDir == CON_UP)
				{
					pMap->fModifyX = m_StartMap->vecConnect.front()->vPos.x - pMap->vecConnect[i]->vPos.x;
					pMap->fModifyZ = m_StartMap->vecConnect.front()->vPos.z - pMap->vecConnect[i]->vPos.z - 2.f;
					pMap->vecConnect[i]->bIsConnect = true;

					bFind = true;
				}
			}
			break;
		case CON_LEFT:
			for(size_t i = 0; i < iConnectSize; ++i)
			{
				if(pMap->vecConnect[i]->eDir == CON_RIGHT)
				{
					pMap->fModifyX = m_StartMap->vecConnect.front()->vPos.x - pMap->vecConnect[i]->vPos.x - 2.f;
					pMap->fModifyZ = m_StartMap->vecConnect.front()->vPos.z - pMap->vecConnect[i]->vPos.z;
					pMap->vecConnect[i]->bIsConnect = true;

					bFind = true;
				}
			}
			break;
		case CON_RIGHT:
			for(size_t i = 0; i < iConnectSize; ++i)
			{
				if(pMap->vecConnect[i]->eDir == CON_LEFT)
				{
					pMap->fModifyX = m_StartMap->vecConnect.front()->vPos.x - pMap->vecConnect[i]->vPos.x + 2.f;
					pMap->fModifyZ = m_StartMap->vecConnect.front()->vPos.z - pMap->vecConnect[i]->vPos.z;
					pMap->vecConnect[i]->bIsConnect = true;

					bFind = true;
				}
			}
			break;
		}
		if(bFind)
		{
			for(vector<MAP*>::iterator iter = m_vecRandomMap.begin();
				iter != m_vecRandomMap.end();)
			{
				if((*iter) == pMap)
				{
					m_vecRandomMap.erase(iter);

					break;
				}
				else
					++iter;
			}

			for(size_t i = 0; i < iConnectSize; ++i)
			{
				pMap->vecConnect[i]->vPos = pMap->vecConnect[i]->vPos + D3DXVECTOR3(pMap->fModifyX, 0.f, pMap->fModifyZ);
			}

			m_vecChooseMap.push_back(pMap);

			D3DXVECTOR3 vSort = D3DXVECTOR3(0.f, 0.f, 0.f);

			if(pMap->fModifyX < 0)
				vSort.x = pMap->fModifyX;
			if(pMap->fModifyZ < 0)
				vSort.z = pMap->fModifyZ;

			if(vSort.x != 0.f || vSort.z != 0.f)
				MapSort(vSort);

			MapInput(pMap);

			break;
		}
	}

	for(int i = 0; i < 40; ++i)
	{
		int iRand = CRand::Random(0, m_vecChooseMap.size() - 1);
		MAP* pMap = m_vecChooseMap[iRand];
		size_t iConnectSize = pMap->vecConnect.size();

		for(size_t j = 0; j < iConnectSize; ++j)
		{
			if(pMap->vecConnect[j]->bIsConnect)
				continue;

			bool bFind = false;
			int iSize = m_vecRandomMap.size();

			if(iSize == 0)
				return;

			MAP* pRandMap = m_vecRandomMap[CRand::Random(0, iSize - 1)];
			size_t iInConnectSize = pRandMap->vecConnect.size();

			switch(pMap->vecConnect[j]->eDir)
			{
			case CON_UP:
				for(size_t k = 0; k < iInConnectSize; ++k)
				{
					if(pRandMap->vecConnect[k]->eDir == CON_DOWN)
					{
						pRandMap->fModifyX = pMap->vecConnect[j]->vPos.x - pRandMap->vecConnect[k]->vPos.x;
						pRandMap->fModifyZ = pMap->vecConnect[j]->vPos.z - pRandMap->vecConnect[k]->vPos.z + 2.f;

						if(MapInput(pRandMap))
						{
							D3DXVECTOR3 vSort = D3DXVECTOR3(0.f, 0.f, 0.f);

							if(pRandMap->fModifyX < 0)
								vSort.x = pRandMap->fModifyX;
							if(pRandMap->fModifyZ < 0)
								vSort.z = pRandMap->fModifyZ;

							if(vSort.x != 0.f || vSort.z != 0.f)
								MapSort(vSort);

							pRandMap->vecConnect[k]->bIsConnect = true;

							bFind = true;
						}

						break;
					}
				}
				break;
			case CON_DOWN:
				for(size_t k = 0; k < iInConnectSize; ++k)
				{
					if(pRandMap->vecConnect[k]->eDir == CON_UP)
					{
						pRandMap->fModifyX = pMap->vecConnect[j]->vPos.x - pRandMap->vecConnect[k]->vPos.x;
						pRandMap->fModifyZ = pMap->vecConnect[j]->vPos.z - pRandMap->vecConnect[k]->vPos.z - 2.f;

						if(MapInput(pRandMap))
						{
							D3DXVECTOR3 vSort = D3DXVECTOR3(0.f, 0.f, 0.f);

							if(pRandMap->fModifyX < 0)
								vSort.x = pRandMap->fModifyX;
							if(pRandMap->fModifyZ < 0)
								vSort.z = pRandMap->fModifyZ;

							if(vSort.x != 0.f || vSort.z != 0.f)
								MapSort(vSort);

							pRandMap->vecConnect[k]->bIsConnect = true;

							bFind = true;
						}

						break;
					}
				}
				break;
			case CON_LEFT:
				for(size_t k = 0; k < iInConnectSize; ++k)
				{
					if(pRandMap->vecConnect[k]->eDir == CON_RIGHT)
					{
						pRandMap->fModifyX = pMap->vecConnect[j]->vPos.x - pRandMap->vecConnect[k]->vPos.x - 2.f;
						pRandMap->fModifyZ = pMap->vecConnect[j]->vPos.z - pRandMap->vecConnect[k]->vPos.z;

						if(MapInput(pRandMap))
						{
							D3DXVECTOR3 vSort = D3DXVECTOR3(0.f, 0.f, 0.f);

							if(pRandMap->fModifyX < 0)
								vSort.x = pRandMap->fModifyX;
							if(pRandMap->fModifyZ < 0)
								vSort.z = pRandMap->fModifyZ;

							if(vSort.x != 0.f || vSort.z != 0.f)
								MapSort(vSort);

							pRandMap->vecConnect[k]->bIsConnect = true;

							bFind = true;
						}

						break;
					}
				}
				break;
			case CON_RIGHT:
				for(size_t k = 0; k < iInConnectSize; ++k)
				{
					if(pRandMap->vecConnect[k]->eDir == CON_LEFT)
					{
						pRandMap->fModifyX = pMap->vecConnect[j]->vPos.x - pRandMap->vecConnect[k]->vPos.x + 2.f;
						pRandMap->fModifyZ = pMap->vecConnect[j]->vPos.z - pRandMap->vecConnect[k]->vPos.z;
					
						if(MapInput(pRandMap))
						{
							D3DXVECTOR3 vSort = D3DXVECTOR3(0.f, 0.f, 0.f);

							if(pRandMap->fModifyX < 0)
								vSort.x = pRandMap->fModifyX;
							if(pRandMap->fModifyZ < 0)
								vSort.z = pRandMap->fModifyZ;

							if(vSort.x != 0.f || vSort.z != 0.f)
								MapSort(vSort);

							pRandMap->vecConnect[k]->bIsConnect = true;

							bFind = true;
						}

						break;
					}
				}
				break;
			}

			if(bFind)
			{
				for(vector<MAP*>::iterator iter = m_vecRandomMap.begin();
					iter != m_vecRandomMap.end();)
				{
					if((*iter) == pRandMap)
					{
						m_vecRandomMap.erase(iter);

						break;
					}
					else
						++iter;
				}

				for(size_t k = 0; k < iInConnectSize; ++k)
				{
					pRandMap->vecConnect[k]->vPos = pRandMap->vecConnect[k]->vPos + D3DXVECTOR3(pRandMap->fModifyX, 0.f, pRandMap->fModifyZ);
				}

				pMap->vecConnect[j]->bIsConnect = true;

				m_vecChooseMap.push_back(pRandMap);

				bFind = false;
			}
		}
	}

	iSize = m_vecChooseMap.size();

	for(size_t i = iSize - 1; i > 0; --i)
	{
		MAP* pMap = m_vecChooseMap[i];
		size_t iConnectSize = pMap->vecConnect.size();

		bool bFind = false;

		for(size_t j = 0; j < iConnectSize; ++j)
		{
			if(pMap->vecConnect[j]->bIsConnect)
				continue;

			size_t iEndConnect = m_EndMap->vecConnect.size();

			for(size_t k = 0; k < iEndConnect; ++k)
			{
				switch(m_EndMap->vecConnect[k]->eDir)
				{
				case CON_UP:
					if(pMap->vecConnect[j]->eDir == CON_DOWN)
					{
						m_EndMap->fModifyX = pMap->vecConnect[j]->vPos.x - m_EndMap->vecConnect[k]->vPos.x;
						m_EndMap->fModifyZ = pMap->vecConnect[j]->vPos.z - m_EndMap->vecConnect[k]->vPos.z - 2.f;

						if(MapInput(m_EndMap))
						{
							D3DXVECTOR3 vSort = D3DXVECTOR3(0.f, 0.f, 0.f);

							if(m_EndMap->fModifyX < 0)
								vSort.x = m_EndMap->fModifyX;
							if(m_EndMap->fModifyZ < 0)
								vSort.z = m_EndMap->fModifyZ;

							if(vSort.x != 0.f || vSort.z != 0.f)
								MapSort(vSort);

							pMap->vecConnect[j]->bIsConnect = true;

							bFind = true;
						}
						break;
					}
					break;
				case CON_DOWN:
					if(pMap->vecConnect[j]->eDir == CON_UP)
					{
						m_EndMap->fModifyX = pMap->vecConnect[j]->vPos.x - m_EndMap->vecConnect[k]->vPos.x;
						m_EndMap->fModifyZ = pMap->vecConnect[j]->vPos.z - m_EndMap->vecConnect[k]->vPos.z + 2.f;

						if(MapInput(m_EndMap))
						{
							D3DXVECTOR3 vSort = D3DXVECTOR3(0.f, 0.f, 0.f);

							if(m_EndMap->fModifyX < 0)
								vSort.x = m_EndMap->fModifyX;
							if(m_EndMap->fModifyZ < 0)
								vSort.z = m_EndMap->fModifyZ;

							if(vSort.x != 0.f || vSort.z != 0.f)
								MapSort(vSort);

							pMap->vecConnect[j]->bIsConnect = true;

							bFind = true;
						}
						break;
					}
					break;
				case CON_LEFT:
					if(pMap->vecConnect[j]->eDir == CON_RIGHT)
					{
						m_EndMap->fModifyX = pMap->vecConnect[j]->vPos.x - m_EndMap->vecConnect[k]->vPos.x + 2.f;
						m_EndMap->fModifyZ = pMap->vecConnect[j]->vPos.z - m_EndMap->vecConnect[k]->vPos.z;

						if(MapInput(m_EndMap))
						{
							D3DXVECTOR3 vSort = D3DXVECTOR3(0.f, 0.f, 0.f);

							if(m_EndMap->fModifyX < 0)
								vSort.x = m_EndMap->fModifyX;
							if(m_EndMap->fModifyZ < 0)
								vSort.z = m_EndMap->fModifyZ;

							if(vSort.x != 0.f || vSort.z != 0.f)
								MapSort(vSort);

							pMap->vecConnect[j]->bIsConnect = true;

							bFind = true;
						}
						break;
					}
					break;
				case CON_RIGHT:
					if(pMap->vecConnect[j]->eDir == CON_LEFT)
					{
						m_EndMap->fModifyX = pMap->vecConnect[j]->vPos.x - m_EndMap->vecConnect[k]->vPos.x - 2.f;
						m_EndMap->fModifyZ = pMap->vecConnect[j]->vPos.z - m_EndMap->vecConnect[k]->vPos.z;

						if(MapInput(m_EndMap))
						{
							D3DXVECTOR3 vSort = D3DXVECTOR3(0.f, 0.f, 0.f);

							if(m_EndMap->fModifyX < 0)
								vSort.x = m_EndMap->fModifyX;
							if(m_EndMap->fModifyZ < 0)
								vSort.z = m_EndMap->fModifyZ;

							if(vSort.x != 0.f || vSort.z != 0.f)
								MapSort(vSort);

							pMap->vecConnect[j]->bIsConnect = true;

							bFind = true;
						}
						break;
					}
					break;
				}

				if(bFind)
				{
					for(size_t l = 0; l < iEndConnect; ++l)
					{
						m_EndMap->vecConnect[l]->vPos = m_EndMap->vecConnect[l]->vPos + D3DXVECTOR3(m_EndMap->fModifyX, 0.f, m_EndMap->fModifyZ);
					}

					m_EndMap->vecConnect[k]->bIsConnect = true;

					bFind = false;

					MapInput(m_EndMap);

					return;
				}
			}
		}
	}
}

void CStage::MapSort(const D3DXVECTOR3& vPos)
{
	if(vPos.x < m_vSort.x)
		m_vSort.x = vPos.x;
	if(vPos.z < m_vSort.z)
		m_vSort.z = vPos.z;
}

const bool CStage::GetBossStage() const
{
	return m_bBossStage;
}

void CStage::ShopItem(void)
{
	CItem* pItem = (CItem*)CObjFactory<CItem>::CreateObj();
	pItem->SetObjKey(CItemTableMgr::GetInstance()->GetRandomItem());// 아이템 이름 생성
	pItem->SetPos(D3DXVECTOR3(m_vecShop.x - 2.f, 1.f, m_vecShop.z - 4.f));
	pItem->InitSetType();
	pItem->SetSelling(true);
	pItem->SetSellPrice();
	pItem->SetPrice();
	m_pObjMgr->AddObj(OBJ_ITEM, pItem);

	pItem = (CItem*)CObjFactory<CItem>::CreateObj();
	pItem->SetObjKey(CItemTableMgr::GetInstance()->GetRandomItem());// 아이템 이름 생성
	pItem->SetPos(D3DXVECTOR3(m_vecShop.x , 1.f, m_vecShop.z - 4.f));
	pItem->InitSetType();
	pItem->SetSelling(true);
	pItem->SetSellPrice();
	pItem->SetPrice();
	m_pObjMgr->AddObj(OBJ_ITEM, pItem);

	pItem = (CItem*)CObjFactory<CItem>::CreateObj();
	pItem->SetObjKey(CItemTableMgr::GetInstance()->GetRandomItem());// 아이템 이름 생성
	pItem->SetPos(D3DXVECTOR3(m_vecShop.x + 2.f, 1.f, m_vecShop.z - 4.f));
	pItem->InitSetType();
	pItem->SetSelling(true);
	pItem->SetSellPrice();
	pItem->SetPrice();
	m_pObjMgr->AddObj(OBJ_ITEM, pItem);
}