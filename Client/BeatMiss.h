#pragma once

#include "Effect.h"

class CBeatMiss : public CEffect
{
public:
	CBeatMiss(void);
	~CBeatMiss(void);

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	BYTE m_byAlpha;
};