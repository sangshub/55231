#pragma once

#include "Effect.h"

class CEffectBomb :	public CEffect
{
public:
	CEffectBomb(void);
	~CEffectBomb(void);

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();
};