#include "stdafx.h"
#include "SceneObj.h"
#include "SceneMgr.h"
#include "Device.h"
#include "TextureMgr.h"
#include "CamMgr.h"

CSceneObj::CSceneObj()
: m_pSceneMgr(CSceneMgr::GetInstance())
, m_pDevice(CDevice::GetInstance())
, m_pTextureMgr(CTextureMgr::GetInstance())
, m_pCamMgr(CCamMgr::GetInstance())
{
}

CSceneObj::~CSceneObj()
{
}