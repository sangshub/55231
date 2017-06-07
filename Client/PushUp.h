#pragma once

#include "Trap.h"

class CPushUp : public CTrap
{
public:
	CPushUp();
	~CPushUp();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();

private:
	int m_iNextIndex;
};