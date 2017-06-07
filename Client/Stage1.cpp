#include "stdafx.h"
#include "Stage1.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "SceneMgr.h"
#include "ItemTableMgr.h"
#include "Item.h"
#include "Coin_Multi.h"
#include "MiniMap.h"
#include "BeatMgr.h"

CStage1::CStage1()
{
}

CStage1::~CStage1()
{
}

HRESULT CStage1::Initialize()
{
	CItemTableMgr::GetInstance()->RefreshItemTable();

	MapLoad(TEXT("../Data/Map/SS/SS_Star.dat"), 0);
	MapLoad(TEXT("../Data/Map/test11.dat"), 1);

	for(int i = 0; i < 25; ++i)
	{
		CString strPath = TEXT("");
		strPath.Format(TEXT("../Data/Map/SS/SS_Part%d.dat"), i);
		MapLoad((wstring)strPath);
	}
	MapLoad(TEXT("../Data/Map/SS/SS_Part_Shop.dat"));

	MapCreate();

	if(!m_bStageInit)
		StageBaseObjInit();

	m_pDevice->SoundPlay(TEXT("Zone1_1"), 1);
	m_pDevice->SoundPlay(TEXT("Zone1_1_Shop"), 0);

	m_pBeatMgr->SetBeatFrame(BEAT_ZONE1_1);	//노래 박자 세팅

	m_pMiniMap = CObjFactory<CMiniMap>::CreateObj();

	m_pObjMgr->AddObj(OBJ_UI, m_pMiniMap);		//미니맵

	m_pObjMgr->AddObj(OBJ_EFFECT, CObjFactory<CCoin_Multi>::CreateObj());
	m_pObjMgr->SetStageStart(true);

	ShopItem();

	return S_OK;
}

void CStage1::Progress()
{
	ShopVolume();

	if(m_pDevice->SoundPlaying(TEXT("Zone1_1")))
		m_pDevice->SetSoundVolume(TEXT("Zone1_1_Shop"), m_iVolume);

	m_pObjMgr->Progress();

	if(GetAsyncKeyState('K') & 0x0001)
	{
		m_pObjMgr->ChangeStage();
		m_pSceneMgr->SetSceneChange(SCENE_STAGE1);
		CItemTableMgr::GetInstance()->RefreshItemTable();

		return;
	}
}

void CStage1::Render()
{
	m_pObjMgr->Render();
}

void CStage1::StopBgm()
{
	m_pDevice->SoundStop(TEXT("Zone1_1"));
	m_pDevice->SoundStop(TEXT("Zone1_1_Shop"));
}

void CStage1::NextStage()
{
	m_pObjMgr->ChangeStage();
	m_pSceneMgr->SetSceneChange(SCENE_STAGE2);
}