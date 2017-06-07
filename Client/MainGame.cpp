#include "StdAfx.h"
#include "MainGame.h"
#include "Device.h"
//Object
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "CamMgr.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "SceneMgr.h"
#include "ObjMgr.h"
#include "BeatMgr.h"
#include "Rand.h"
#include "Stage.h"
#include "FireBall.h"
#include "ObjFactory.h"
#include "ParticleMgr.h"
#include "ComboMgr.h"
#include "ItemTableMgr.h"
#include "MediaMgr.h"
//Mgr

CMainGame::CMainGame(void)
: m_iFpsCnt(0)
, m_fTime(0.f)
, m_pDevice(CDevice::GetInstance())
, m_pTimeMgr(CTimeMgr::GetInstance())
, m_pKeyMgr(CKeyMgr::GetInstance())
, m_pCamMgr(CCamMgr::GetInstance())
, m_pBufferMgr(CBufferMgr::GetInstance())
, m_pTextureMgr(CTextureMgr::GetInstance())
, m_pSceneMgr(CSceneMgr::GetInstance())
, m_pObjMgr(CObjMgr::GetInstance())
, m_pBeatMgr(CBeatMgr::GetInstance())
, m_pParticleMgr(CParticleMgr::GetInstance())
, m_pComboMgr(CComboMgr::GetInstance())
{
	ZeroMemory(m_szFps, sizeof(TCHAR) * MIN_STR);
}

CMainGame::~CMainGame(void)
{
	Release();
}

HRESULT CMainGame::Initialize(void)
{
	// Rand 매니저 랜덤 시드값
	unsigned int iRandInit[16] = {0};
	srand(GetTickCount());

	for(int i = 0; i < 16; ++i)
	{
		iRandInit[i] = rand()%256;
	}
	CRand::InitWELLRNG512a(iRandInit);

	if(FAILED(m_pDevice->InitDevice(MODE_WIN))) // MODE_WIN / MODE_FULL
		return E_FAIL;

	if(FAILED(m_pSceneMgr->SetSceneChange(SCENE_MOVIE)))
		return E_FAIL;

	m_pCamMgr->InitCamera();
	m_pTimeMgr->InitTime();
	m_pBeatMgr->Initialize();
	m_pParticleMgr->Initialize();
	
	return S_OK;
}

void CMainGame::Progress(void)
{
	m_pKeyMgr->KeyCheck();

	m_pTimeMgr->SetTime();

	m_pSceneMgr->Progress();

	m_pCamMgr->Progress();

	// FPS
	if(CStage::GetStageInit())
	{
		m_fTime += m_pTimeMgr->GetTime();
		++m_iFpsCnt;

		if(m_fTime > 1.f)
		{
			wsprintf(m_szFps, TEXT("FPS : %d"), m_iFpsCnt);
			m_iFpsCnt = 0;
			m_fTime = 0.f;
		}
	}
}

void CMainGame::Render(void)
{
	m_pDevice->Render_Begin();

	m_pSceneMgr->Render();

	// FPS
	SetWindowText(g_hWnd, m_szFps);

	m_pDevice->Render_End();
}

void CMainGame::Release(void)
{
	::Safe_Destroy(m_pDevice);
	::Safe_Destroy(m_pTimeMgr);
	::Safe_Destroy(m_pCamMgr);
	::Safe_Destroy(m_pKeyMgr);
	::Safe_Destroy(m_pBufferMgr);
	::Safe_Destroy(m_pTextureMgr);
	::Safe_Destroy(m_pSceneMgr);
	::Safe_Destroy(m_pObjMgr);
	::Safe_Destroy(m_pBeatMgr);
	::Safe_Destroy(m_pParticleMgr);
	::Safe_Destroy(m_pComboMgr);
	::Safe_Destroy(m_pItemTableMgr);
}