#pragma once

#include "Trap.h"

class CPushDown : public CTrap
{
public:
	CPushDown();
	~CPushDown();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();

private:
	int m_iNextIndex;
};