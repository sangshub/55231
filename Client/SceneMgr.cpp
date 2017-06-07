#include "stdafx.h"
#include "SceneMgr.h"
#include "Logo.h"
#include "Stage1.h"
#include "Stage2.h"
#include "BossStage.h"
#include "TutorialStage.h"
#include "Movie.h"

IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr()
: m_pSceneObj(NULL)
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Progress()
{
	m_pSceneObj->Progress();
}

void CSceneMgr::Render()
{
	m_pSceneObj->Render();
}

void CSceneMgr::Release()
{
	::Safe_Delete(m_pSceneObj);
}

HRESULT CSceneMgr::SetSceneChange(const SCENEID& eSceneId)
{
	if(m_pSceneObj)
	{
		m_pSceneObj->StopBgm();
		::Safe_Delete(m_pSceneObj);	
	}

	switch(eSceneId)
	{
	case SCENE_LOGO:
		m_pSceneObj = new CLogo;
		break;
	case SCENE_STAGE1:
		m_pSceneObj = new CStage1;
		break;
	case SCENE_STAGE2:
		m_pSceneObj = new CStage2;
		break;
	case SCENE_BOSS:
		m_pSceneObj = new CBossStage;
		break;
	case SCENE_TUTORIAL:
		m_pSceneObj = new CTutorialStage;
		break;
	case SCENE_MOVIE:
		m_pSceneObj = new CMovie;
		break;
	}

	if(FAILED(m_pSceneObj->Initialize()))
	{
		ERR_MSG(TEXT("Scene Change Error"));

		return E_FAIL;
	}

	return S_OK;
}

void CSceneMgr::SetNextScene()
{
	m_pSceneObj->NextStage();
}

void CSceneMgr::StopBgm()
{
	m_pSceneObj->StopBgm();
}

CSceneObj* CSceneMgr::GetSceneObj(void) const
{
	return m_pSceneObj;
}