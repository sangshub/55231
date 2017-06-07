#pragma once

#include "FireBall.h"

class CFireBallStart : public CFireBall
{
public:
	CFireBallStart();
	~CFireBallStart();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();
};