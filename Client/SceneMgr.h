#pragma once

#include "Include.h"

class CSceneObj;

class CSceneMgr
{
public:
	DECLARE_SINGLETON(CSceneMgr)

private:
	CSceneMgr();

public:
	~CSceneMgr();

public:
	HRESULT SetSceneChange(const SCENEID& eSceneId);
	void SetNextScene();
	void StopBgm();
	CSceneObj* GetSceneObj(void) const;

public:
	void Progress();
	void Render();

private:
	void Release();

private:
	CSceneObj* m_pSceneObj;
};