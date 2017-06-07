#pragma once

#include "Monster.h"

class CRedBat : public CMonster
{
public:
	CRedBat();
	~CRedBat();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();
};