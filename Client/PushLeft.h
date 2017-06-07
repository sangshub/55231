#pragma once

#include "Trap.h"

class CPushLeft : public CTrap
{
public:
	CPushLeft();
	~CPushLeft();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();

private:
	int m_iNextIndex;
};