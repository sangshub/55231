#pragma once

#include "Monster.h"

class CSkeleton : public CMonster
{
public:
	CSkeleton();
	~CSkeleton();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();
};