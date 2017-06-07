#include "stdafx.h"
#include "BossStage.h"
#include "Device.h"
#include "Player.h"
#include "Octoboss.h"
#include "MiniMap.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "BeatMgr.h"
#include "Harpoon.h"
#include "Coin_Multi.h"

CBossStage::CBossStage()
: m_bInit(false)
, m_bActionCam(false)
, m_bBossEnding(false)
, m_iIndex(0)
, m_pBoss(NULL)
, m_pHarppon(NULL)
{
}

CBossStage::~CBossStage()
{
}

HRESULT CBossStage::Initialize()
{
	FileLoad(TEXT("../Data/Map/Boss/Boss.dat"));
	m_pObjMgr->SetTileSize(m_iTileX, m_iTileZ);

	if(!m_bStageInit)
		StageBaseObjInit();

	AllDataLoad(TEXT("../Data/Map/Boss/Boss.dat"));

	m_pObjMgr->GetPlayer()->SetPos(D3DXVECTOR3(12.f, 0.f, 2.f));
	((CPlayer*)m_pObjMgr->GetPlayer())->SetPlayerIndex(D3DXVECTOR3(12.f, 0.f, 2.f));
	m_pObjMgr->SetIsLight(true);

	m_pObjMgr->AddObj(OBJ_EFFECT, CObjFactory<CCoin_Multi>::CreateObj());

	m_pObjMgr->AddObj(OBJ_UI, CObjFactory<CMiniMap>::CreateObj());
	m_pObjMgr->SetStageStart(true);
	m_bBossStage = true;

	StartActionCamPush(); // 액션캠 셋팅

	m_vecAction.reserve(4);

	return S_OK;
}

void CBossStage::Progress()
{
	m_pObjMgr->Progress();

	ActionCamPlay();

	if(!m_bInit)
	{
		if(m_pObjMgr->GetPlayer()->GetInfo()->vPos.z >= 10.f)
		{
			m_pObjMgr->ResetUIVtx();
			m_pBoss = (COctoboss*)CObjFactory<COctoboss>::CreateObj(D3DXVECTOR3(11.f, -5.f, 24.f));
			m_pObjMgr->ResetCubeTex();
			m_pHarppon = (CHarppon*)CObjFactory<CHarppon>::CreateObj(D3DXVECTOR3(12.f, 6.f, -20.f));

			m_pObjMgr->AddObj(OBJ_MONSTER, m_pBoss);

			m_pObjMgr->AddObj(OBJ_EFFECT, m_pHarppon);
			m_pHarppon->SetBoss(m_pBoss);
			m_pHarppon->SetBossStage(this);

			m_pDevice->SoundPlay(TEXT("Boss_1"), 1);

			m_pBeatMgr->SetBeatFrame(BEAT_ZONE1_1);	//노래 박자 세팅

			m_bInit = true;

			m_bActionCam = true;// 액션캠 셋팅
			m_iIndex = 0;
			m_pObjMgr->SetActionCam(true);
			((CPlayer*)m_pObjMgr->GetPlayer())->SetActionCamSwitch(true);
		}
		return;
	}
}

void CBossStage::Render()
{
	m_pObjMgr->Render();
}

void CBossStage::StopBgm()
{
	m_pDevice->SoundStop(TEXT("Boss_1"));
}
void CBossStage::NextStage()
{
	DestroyWindow(g_hWnd);
}

void CBossStage::StartActionCamPush()
{	
	D3DXVECTOR3 vAt[4];
	m_vecAction.clear();

	vAt[0] = m_pObjMgr->GetPlayer()->GetInfo()->vPos;
	vAt[1] = D3DXVECTOR3(11.f, 3.f, 24.f);
	vAt[2] = D3DXVECTOR3(11.f, 3.f, 24.f);
	vAt[3] = m_pObjMgr->GetPlayer()->GetInfo()->vPos;

	m_vecAction.push_back(ACTION(m_pCamMgr->GetEyePos(vAt[0], 0.f, 60.f, 35.f),	vAt[0], 1.f));
	m_vecAction.push_back(ACTION(m_pCamMgr->GetEyePos(vAt[1], 0.f, 20.f, 35.f),	vAt[1], 4.f));
	m_vecAction.push_back(ACTION(m_pCamMgr->GetEyePos(vAt[2], 0.f, 20.f, 35.f),	vAt[2], 1.f));
	m_vecAction.push_back(ACTION(m_pCamMgr->GetEyePos(vAt[3], 0.f, 60.f, 35.f),	vAt[3], 1.f));
}

void CBossStage::EndActionCamPush(const D3DXVECTOR3& vPos)
{
	D3DXVECTOR3 vAt[4];
	float fAngle = ((CPlayer*)m_pObjMgr->GetPlayer())->GetCamAngleY();
	fAngle = D3DXToDegree(fAngle);
	m_vecAction.clear();

	vAt[0] = vPos;
	vAt[1] = m_pBoss->GetInfo()->vPos;
	vAt[1].y = vAt[0].y;
	vAt[2] = vAt[1];
	vAt[3] = vAt[1];

	m_vecAction.push_back(ACTION(m_pCamMgr->GetEyePos(vAt[0], -30.f + fAngle, 30.f, 35.f),	vAt[0], 1.5f));
	m_vecAction.push_back(ACTION(m_pCamMgr->GetEyePos(vAt[1], -30.f + fAngle, 30.f, 35.f),	vAt[1], 2.6f));
	m_vecAction.push_back(ACTION(m_pCamMgr->GetEyePos(vAt[2], -30.f + fAngle, 20.f, 20.f),	vAt[2], 1.f));
	m_vecAction.push_back(ACTION(m_pCamMgr->GetEyePos(vAt[3], -30.f + fAngle, 20.f, 20.f),	vAt[3], 1.f));
}

void CBossStage::ActionCamPlay()
{
	if(m_bActionCam && m_pCamMgr->ActionPlayData(m_vecAction[m_iIndex], m_vecAction[m_iIndex + 1]))
	{
		++m_iIndex;

		if(m_iIndex + 1 >= m_vecAction.size())
		{
			m_iIndex = 0;
			m_bActionCam = false;
			m_pObjMgr->SetActionCam(false);
			((CPlayer*)m_pObjMgr->GetPlayer())->SetActionCamSwitch(false);
			if(m_bBossEnding)
			{
				m_pBoss->SetDelete();
				m_pObjMgr->SetStageEnd(true);
			}
		}
	}
}

void CBossStage::EndingActionCamSetting(const D3DXVECTOR3& vPos)
{
	EndActionCamPush(vPos);
	m_bActionCam = true;// 액션캠 셋팅
	m_bBossEnding = true;
	m_iIndex = 0;
	m_pObjMgr->SetActionCam(true);
	((CPlayer*)m_pObjMgr->GetPlayer())->SetActionCamSwitch(true);
}