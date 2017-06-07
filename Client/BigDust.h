#pragma once

#include "Effect.h"

class CBigDust : public CEffect
{
public:
	CBigDust();
	~CBigDust();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

public:
	void SetMatrix(const float fScale = 1.f);
};