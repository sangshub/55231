#pragma once

#include "Monster.h"

class CBlueBanshee : public CMonster
{
public:
	CBlueBanshee();
	~CBlueBanshee();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();

private:
	bool m_bCry;
};