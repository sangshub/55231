#pragma once

#include "Obj.h"

class CEffect : public CObj
{
public:
	CEffect();
	virtual ~CEffect();

public:
	virtual HRESULT Initialize() PURE;
	virtual const int Progress() PURE;
	virtual void Render() PURE;

protected:
	virtual void SetMatrix(const float fScale = 1.f);
	void SetUIMatrix(const float fScale = 1.f);
	void EffectInit();
	void EffectRender(const BYTE byAlpha = 255);
};