#pragma once

#include "Effect.h"

class CCoin_Multi :	public CEffect
{
public:
	CCoin_Multi(void);
	~CCoin_Multi(void);

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();
};