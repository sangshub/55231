#pragma once

#include "Monster.h"

class CMonkey : public CMonster
{
public:
	CMonkey();
	~CMonkey();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();
	void Grab();

private:
	bool m_bGrab;
};