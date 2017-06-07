#pragma once

#include "Monster.h"

class CGolem : public CMonster
{
public:
	CGolem();
	~CGolem();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	void Release();

private:
	virtual void SetMatrix(const float fScale = 1.f);
	virtual void Pattren();
};