#pragma once

#include "Effect.h"

class CNum : public CEffect
{
public:
	CNum(void);
	~CNum(void);

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();
};