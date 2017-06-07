#pragma once

#include "Monster.h"

class CGreenDragon : public CMonster
{
public:
	CGreenDragon();
	~CGreenDragon();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();
};