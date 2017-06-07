#pragma once

#include "Effect.h"

class CMissed : public CEffect
{
public:
	CMissed();
	~CMissed();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	void SetMatrix(const float fScale = 1.f);

private:
	float m_fTime;
};