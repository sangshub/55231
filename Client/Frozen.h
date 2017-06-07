#pragma once
#include "effect.h"

class CObj;

class CFrozen :
	public CEffect
{
public:
	CFrozen(void);
	~CFrozen(void);

public:
	void SetFrozenUnit(CObj* pObj);
	void SetMatrix();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	CObj* m_pObj;
	float m_fScale;
};