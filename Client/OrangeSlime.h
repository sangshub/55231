#pragma once

#include "Monster.h"

class COrangeSlime : public CMonster
{
public:
	COrangeSlime();
	~COrangeSlime();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();
};