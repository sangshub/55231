#pragma once

#include "Monster.h"

class CBlueSlime : public CMonster
{
public:
	CBlueSlime();
	~CBlueSlime();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();
};