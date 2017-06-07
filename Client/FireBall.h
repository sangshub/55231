#pragma once

#include "Effect.h"

class CFireBall : public CEffect
{
public:
	CFireBall();
	~CFireBall();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	void Release();

protected:
	void SetMatrix();
	void SetScreenMode();

protected:
	bool m_bInit;
	bool m_bLeft;
	int m_iScreenMode;
	vector<int> m_vecDamageTile;
};