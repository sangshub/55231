#pragma once

#include "Effect.h"

class CMini_Diamond : public CEffect
{
public:
	CMini_Diamond(void);
	~CMini_Diamond(void);

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();
};