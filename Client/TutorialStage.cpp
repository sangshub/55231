#include "stdafx.h"
#include "TutorialStage.h"
#include "Player.h"
#include "MiniMap.h"
#include "Item.h"
#include "Chest.h"
#include "Device.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "SceneMgr.h"
#include "CamMgr.h"
#include "TutorialText.h"
#include "ItemTableMgr.h"
#include "BeatMgr.h"

CTutorialStage::CTutorialStage(void)
: m_bActionCam(true)
, m_iIndex(0)
, m_pItemTableMgr(CItemTableMgr::GetInstance())
{
}

CTutorialStage::~CTutorialStage(void)
{
	Release();
}

HRESULT CTutorialStage::Initialize()
{
	FileLoad(TEXT("../Data/Map/Tuto.dat"));
	m_pObjMgr->SetTileSize(m_iTileX, m_iTileZ);

	if(!m_bStageInit)
		StageBaseObjInit();

	CubeLoad(TEXT("../Data/Map/Tuto.dat"));
	MonsterLoad(TEXT("../Data/Map/Tuto.dat"));
	ObjLoad(TEXT("../Data/Map/Tuto.dat"));

	m_pObjMgr->ResetUIVtx();
	m_pObjMgr->ResetCubeTex();

	m_pObjMgr->GetPlayer()->SetPos(D3DXVECTOR3(10.f, 0.f, 52.f));
	((CPlayer*)m_pObjMgr->GetPlayer())->SetPlayerIndex(D3DXVECTOR3(10.f, 0.f, 52.f));

	m_pDevice->SoundPlay(TEXT("Tutorial"), 1);
	m_pDevice->SetSoundVolume(TEXT("Tutorial"), -400);

	m_pBeatMgr->SetBeatFrame(BEAT_TUTORIAL);	//노래 박자 세팅

	CTutorialText* pText = (CTutorialText*)CObjFactory<CTutorialText>::CreateObj();
	pText->SetTextImageNumSize(0, 3.f, 1.f);
	pText->SetPos(D3DXVECTOR3(10.f, 0.f, 48.f));
	m_pObjMgr->AddObj(OBJ_EFFECT, pText);

	pText = (CTutorialText*)CObjFactory<CTutorialText>::CreateObj();
	pText->SetTextImageNumSize(7, 3.f, 1.f);
	pText->SetPos(D3DXVECTOR3(10.f, 0.f, 54.f));
	m_pObjMgr->AddObj(OBJ_EFFECT, pText);

	pText = (CTutorialText*)CObjFactory<CTutorialText>::CreateObj();
	pText->SetTextImageNumSize(5, 3.f, 1.f);
	pText->SetPos(D3DXVECTOR3(34.f, 0.f, 54.f));
	m_pObjMgr->AddObj(OBJ_EFFECT, pText);

	pText = (CTutorialText*)CObjFactory<CTutorialText>::CreateObj();
	pText->SetTextImageNumSize(1, 1.f, 1.f);
	pText->SetPos(D3DXVECTOR3(10.f, 0.f, 44.f));
	m_pObjMgr->AddObj(OBJ_EFFECT, pText);

	pText = (CTutorialText*)CObjFactory<CTutorialText>::CreateObj();
	pText->SetTextImageNumSize(2, 4.f, 0.6f);
	pText->SetPos(D3DXVECTOR3(8.f, 0.f, 30.f));
	m_pObjMgr->AddObj(OBJ_EFFECT, pText);

	pText = (CTutorialText*)CObjFactory<CTutorialText>::CreateObj();
	pText->SetTextImageNumSize(4, 4.f, 0.6f);
	pText->SetPos(D3DXVECTOR3(22.f, 0.f, 29.f));
	m_pObjMgr->AddObj(OBJ_EFFECT, pText);

	pText = (CTutorialText*)CObjFactory<CTutorialText>::CreateObj();
	pText->SetTextImageNumSize(3, 4.f, 1.f);
	pText->SetPos(D3DXVECTOR3(34.f, 0.f, 36.f));
	m_pObjMgr->AddObj(OBJ_EFFECT, pText);

	pText = (CTutorialText*)CObjFactory<CTutorialText>::CreateObj();
	pText->SetTextImageNumSize(9, 2.f, 1.f);
	pText->SetPos(D3DXVECTOR3(29.f, 0.f, 20.f));
	m_pObjMgr->AddObj(OBJ_EFFECT, pText);

	pText = (CTutorialText*)CObjFactory<CTutorialText>::CreateObj();
	pText->SetTextImageNumSize(8, 3.f, 1.f);
	pText->SetPos(D3DXVECTOR3(49.f, 0.f, 32.f));
	m_pObjMgr->AddObj(OBJ_EFFECT, pText);

	CItem* pItem = (CItem*)CObjFactory<CItem>::CreateObj();
	pItem->SetObjKey(m_pItemTableMgr->GetTableItem(TEXT("Broad_Sword"))); // 아이템 이름 생성
	pItem->InitSetType();
	pItem->SetPos(D3DXVECTOR3(34.f, 0.f, 32.f));
	m_pObjMgr->AddObj(OBJ_ITEM, pItem);

	pItem = (CItem*)CObjFactory<CItem>::CreateObj();
	pItem->SetObjKey(TEXT("Bomb")); // 아이템 이름 생성
	pItem->InitSetType();
	pItem->SetPos(D3DXVECTOR3(30.f, 0.f, 30.f));
	m_pObjMgr->AddObj(OBJ_ITEM, pItem);

	pItem = (CItem*)CObjFactory<CItem>::CreateObj();
	pItem->SetObjKey(m_pItemTableMgr->GetTableItem(TEXT("Holster"))); // 아이템 이름 생성
	pItem->InitSetType();
	pItem->SetPos(D3DXVECTOR3(48.f, 0.f, 52.f));
	m_pObjMgr->AddObj(OBJ_ITEM, pItem);

	CChest* pChest = (CChest*)CObjFactory<CChest>::CreateObj();
	pChest->SetObjKey(TEXT("Chest"));
	m_pItemTableMgr->AddItem(((CChest*)pChest)->GetItemObjKey());
	pChest->SetItemObjKey(m_pItemTableMgr->GetTableItem(TEXT("Whip")));
	pChest->SetPos((D3DXVECTOR3(54.f, 1.f, 50.f)));
	m_pObjMgr->AddObj(OBJ_ETC, pChest);

	pItem = (CItem*)CObjFactory<CItem>::CreateObj();
	pItem->SetObjKey(m_pItemTableMgr->GetTableItem(TEXT("Food_Apple"))); // 아이템 이름 생성
	pItem->InitSetType();
	pItem->SetPos(D3DXVECTOR3(52.f, 0.f, 36.f));
	m_pObjMgr->AddObj(OBJ_ITEM, pItem);

	pItem = (CItem*)CObjFactory<CItem>::CreateObj();
	pItem->SetObjKey(TEXT("Torch_1")); // 아이템 이름 생성
	pItem->InitSetType();
	pItem->SetPos(D3DXVECTOR3(52.f, 0.f, 32.f));
	m_pObjMgr->AddObj(OBJ_ITEM, pItem);

	pChest = (CChest*)CObjFactory<CChest>::CreateObj();
	pChest->SetObjKey(TEXT("Chest"));
	m_pItemTableMgr->AddItem(((CChest*)pChest)->GetItemObjKey());
	pChest->SetItemObjKey(TEXT("Fireball"));
	pChest->SetPos((D3DXVECTOR3(46.f, 1.f, 20.f)));
	m_pObjMgr->AddObj(OBJ_ETC, pChest);

	m_pObjMgr->AddObj(OBJ_UI, CObjFactory<CMiniMap>::CreateObj());
	m_pObjMgr->SetStageStart(true);

	ActionCamPush(); // 액션캠 셋팅
	m_pObjMgr->SetActionCam(true);
	((CPlayer*)m_pObjMgr->GetPlayer())->SetActionCamSwitch(true);
	m_vecAction.reserve(8);
	
	return S_OK;
}

void CTutorialStage::Progress()
{
	ActionCamPlay();


	if(m_pDevice->SoundPlaying(TEXT("Player_FireBall")))
		m_pDevice->SetSoundVolume(TEXT("Tutorial"), -2200);
	else
		m_pDevice->SetSoundVolume(TEXT("Tutorial"), -800);

	m_pObjMgr->Progress();
}

void CTutorialStage::Render()
{
	m_pObjMgr->Render();	
}

void CTutorialStage::Release()
{
	m_vecAction.clear();
}

void CTutorialStage::StopBgm()
{
	m_pDevice->SoundStop(TEXT("Tutorial"));
}

void CTutorialStage::NextStage()
{
	m_pObjMgr->ChangeStage();
	m_pSceneMgr->SetSceneChange(SCENE_STAGE1);
}

void CTutorialStage::ActionCamPush()
{	
	D3DXVECTOR3 vAt[8];
	m_vecAction.clear();

	vAt[0] = D3DXVECTOR3(10.f, 0.f, 52.f);
	vAt[1] = D3DXVECTOR3(10.f, 0.f, 28.f);
	vAt[2] = D3DXVECTOR3(34.f, 0.f, 34.f);
	vAt[3] = D3DXVECTOR3(34.f, 0.f, 52.f);
	vAt[4] = D3DXVECTOR3(52.f, 0.f, 52.f);
	vAt[5] = D3DXVECTOR3(52.f, 0.f, 28.f);
	vAt[6] = D3DXVECTOR3(28.f, 0.f, 18.f);
	vAt[7] = m_pObjMgr->GetPlayer()->GetInfo()->vPos;

	for(int i = 0; i < 8; ++i)
		m_vecAction.push_back(ACTION(m_pCamMgr->GetEyePos(vAt[i], 0.f, 60.f, 35.f),	vAt[i], 1.f));
}

void CTutorialStage::ActionCamPlay()
{
	if(m_bActionCam && m_pCamMgr->ActionPlayData(m_vecAction[m_iIndex], m_vecAction[m_iIndex + 1]))
	{
		++m_iIndex;

		if(m_iIndex + 1 >= m_vecAction.size())
		{
			m_iIndex = 0;
			m_bActionCam = false;
			m_pObjMgr->SetDarknessSwitch(true);
			((CPlayer*)m_pObjMgr->GetPlayer())->SetActionCamSwitch(false);
		}
	}
}