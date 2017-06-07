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
	virtual void StopBgm(); // Bgm���߱�
	virtual void NextStage(); // ���� ��������

public:
	void ActionCamPush();
	void ActionCamPlay();

private:
	bool m_bActionCam;
	size_t m_iIndex;

	CItemTableMgr* m_pItemTableMgr;
	vector<ACTION> m_vecAction;
};