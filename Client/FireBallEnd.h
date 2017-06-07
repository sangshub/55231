#pragma once

#include "FireBall.h"

class CFireBallEnd : public CFireBall
{
public:
	CFireBallEnd();
	~CFireBallEnd();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	int iRange;
};