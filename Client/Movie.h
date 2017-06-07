#pragma once

#include "SceneObj.h"

class CMediaMgr;

class CMovie : public CSceneObj
{
public:
	CMovie(void);
	~CMovie(void);

public:
	virtual HRESULT Initialize();
	virtual void Progress();
	virtual void Render();

private:
	virtual void Release();

public:
	virtual void StopBgm();
	virtual void NextStage();

private:
	CMediaMgr* m_pMediaMgr;

	HANDLE m_hThread;
	CRITICAL_SECTION m_Crt;
};