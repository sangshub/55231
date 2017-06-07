#pragma once

#include "Trap.h"

class CSpikeTrap : public CTrap
{
public:
	CSpikeTrap();
	~CSpikeTrap();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();

private:
	bool m_bDamage;
};