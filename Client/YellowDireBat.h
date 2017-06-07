#pragma once

#include "Monster.h"

class CYellowDireBat : public CMonster
{
public:
	CYellowDireBat();
	~CYellowDireBat();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();
};