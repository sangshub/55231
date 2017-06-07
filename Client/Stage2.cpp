#include "stdafx.h"
#include "Stage2.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "SceneMgr.h"
#include "ItemTableMgr.h"
#include "Item.h"
#include "Coin_Multi.h"
#include "MiniMap.h"
#include "BeatMgr.h"

CStage2::CStage2()
{
}

CStage2::~CStage2()
{
}

HRESULT CStage2::Initialize()
{
	CItemTableMgr::GetInstance()->RefreshItemTable();

	MapLoad(TEXT("../Data/Map/YKM/YKM_Star.dat"), 0);
	MapLoad(TEXT("../Data/Map/YKM/YKM_En.dat"), 1);
	for(int i = 1; i < 20; ++i)
	{
		CString strPath = TEXT("");
		strPath.Format(TEXT("../Data/Map/YKM/YKM_%d.dat"), i);
		MapLoad((wstring)strPath);
	}
	MapLoad(TEXT("../Data/Map/SS/SS_Part_Shop.dat"));

	MapCreate();

	if(!m_bStageInit)
		StageBaseObjInit();

	m_pDevice->SoundPlay(TEXT("Zone2_1"), 1);
	m_pDevice->SoundPlay(TEXT("Zone2_1_Shop"), 0);

	m_pBeatMgr->SetBeatFrame(BEAT_ZONE2_1);	//노래 박자 세팅

	m_pMiniMap = CObjFactory<CMiniMap>::CreateObj();

	m_pObjMgr->AddObj(OBJ_UI, m_pMiniMap);		//미니맵

	m_pObjMgr->AddObj(OBJ_EFFECT, CObjFactory<CCoin_Multi>::CreateObj());
	m_pObjMgr->SetStageStart(true);

	ShopItem();

	return S_OK;
}

void CStage2::Progress()
{
	ShopVolume();

	if(m_pDevice->SoundPlaying(TEXT("Zone2_1")))
		m_pDevice->SetSoundVolume(TEXT("Zone2_1_Shop"), m_iVolume);

	m_pObjMgr->Progress();

	if(GetAsyncKeyState('K') & 0x0001)
	{
		m_pObjMgr->ChangeStage();
		m_pSceneMgr->SetSceneChange(SCENE_STAGE2);
		CItemTableMgr::GetInstance()->RefreshItemTable();

		return;
	}
}

void CStage2::Render()
{
	m_pObjMgr->Render();
}

void CStage2::StopBgm()
{
	m_pDevice->SoundStop(TEXT("Zone2_1"));
	m_pDevice->SoundStop(TEXT("Zone2_1_Shop"));
}

void CStage2::NextStage()
{
	m_pObjMgr->ChangeStage();
	m_pSceneMgr->SetSceneChange(SCENE_BOSS);
}