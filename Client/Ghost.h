#pragma once

#include "Monster.h"

class CGhost : public CMonster
{
public:
	CGhost();
	~CGhost();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();
};