#pragma once

#include "Effect.h"

class CBossHp : public CEffect
{
public:
	CBossHp();
	~CBossHp();

public:
	void SetDrawId(const int iDrawId);

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	void SetMatrix(const float fScale = 1.f);

private:
	int m_iDrawId;
};