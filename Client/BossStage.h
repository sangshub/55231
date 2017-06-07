#pragma once

#include "Stage.h"

class COctoboss;
class CHarppon;

class CBossStage : public CStage
{
public:
	CBossStage();
	~CBossStage();

public:
	virtual HRESULT Initialize();
	virtual void Progress();
	virtual void Render();

public:
	virtual void StopBgm(); // Bgm멈추기
	virtual void NextStage(); // 다음 스테이지

private: // 액션캠 셋팅
	bool			m_bBossEnding;
	bool			m_bActionCam;
	COctoboss*		m_pBoss;
	CHarppon*		m_pHarppon;
	size_t			m_iIndex;
	vector<ACTION>	m_vecAction;

public:
	void StartActionCamPush();
	void EndActionCamPush(const D3DXVECTOR3& vPos);
	void EndingActionCamSetting(const D3DXVECTOR3& vPos);
	void ActionCamPlay();

private:
	bool m_bInit;
};