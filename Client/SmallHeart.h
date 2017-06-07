#pragma once

#include "Effect.h"

class CSmallHeart : public CEffect
{
public:
	CSmallHeart();
	~CSmallHeart();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

public:
	void SetDamage();
};