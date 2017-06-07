#pragma once

#include "Monster.h"

class CWratih : public CMonster
{
public:
	CWratih();
	~CWratih();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();
};