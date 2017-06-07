#pragma once

#include "Monster.h"

class CLightMinotaur : public CMonster
{
public:
	CLightMinotaur();
	~CLightMinotaur();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();
	void Charge();

private:
	int m_iChargeDir;
};