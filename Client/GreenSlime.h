#pragma once

#include "Monster.h"

class CGreenSlime : public CMonster
{
public:
	CGreenSlime();
	~CGreenSlime();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();
	void JumpHere();
};