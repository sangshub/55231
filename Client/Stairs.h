#pragma once

#include "Trap.h"

class CStairs : public CTrap
{
public:
	CStairs();
	~CStairs();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();
};