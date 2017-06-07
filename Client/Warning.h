#pragma once

#include "Effect.h"

class CWarning : public CEffect
{
public:
	CWarning();
	~CWarning();

public:
	void SetType(const int iType);

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

public:
	void SetMatrix(const float fScale = 1.f);

private:
	int m_iType;
};