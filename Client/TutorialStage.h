#pragma once

#include "Stage.h"

class CItemTableMgr;

class CTutorialStage : public CStage
{
public:
	CTutorialStage(void);
	~CTutorialStage(void);

public:
	virtual HRESULT Initialize();
	virtual void Progress();
	virtual void Render();

private:
	void Release();

public:
	virtual void StopBgm(); // Bgm멈추기
	virtual void NextStage(); // 다음 스테이지

public:
	void ActionCamPush();
	void ActionCamPlay();

private:
	bool m_bActionCam;
	size_t m_iIndex;

	CItemTableMgr* m_pItemTableMgr;
	vector<ACTION> m_vecAction;
};