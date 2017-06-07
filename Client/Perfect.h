#pragma once

#include "Effect.h"

class CPerfect : public CEffect
{
public:
	CPerfect();
	~CPerfect();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	void SetMatrix(const float fScale = 1.f);

private:
	float m_fTime;
};