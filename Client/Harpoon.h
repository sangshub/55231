#pragma once

#include "Obj.h"

class COctoboss;
class CBossStage;

class CHarppon : public CObj
{
public:
	CHarppon(void);
	~CHarppon(void);

public:
	void SetBoss(COctoboss* pBoss);
	void SetBossStage(CBossStage* pBossStage);

public:
	virtual HRESULT Initialize(void);
	virtual const int Progress(void);
	virtual void Render(void);

public:
	void Release(void);

private:
	virtual void SetMatrix(const float fScale = 1.f);
	void SetScreenMode();

public:
	const bool GetFire() const;

private:
	bool m_Ending;
	bool m_bFire;
	int m_iScreenMode;
	float m_fTime;

	CBossStage* m_pBossStage;
	COctoboss*	m_pBoss;

	D3DXVECTOR3 m_vOriPos;
};