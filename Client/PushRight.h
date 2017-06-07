#pragma once

#include "Trap.h"

class CPushRight : public CTrap
{
public:
	CPushRight();
	~CPushRight();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();

private:
	int m_iNextIndex;
};