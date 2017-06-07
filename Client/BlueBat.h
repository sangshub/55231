#pragma once

#include "Monster.h"

class CBlueBat : public CMonster
{
public:
	CBlueBat();
	~CBlueBat();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();
};