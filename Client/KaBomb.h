#pragma once

#include "Effect.h"

class CKaBomb :	public CEffect
{
public:
	CKaBomb(void);
	~CKaBomb(void);

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();
};