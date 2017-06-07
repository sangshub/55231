#pragma once

#include "Obj.h"

class CTentacle : public CObj
{
public:
	CTentacle(void);
	~CTentacle(void);

public:
	void SetShort();

public:
	virtual HRESULT Initialize(void);
	virtual const int Progress(void);
	virtual void Render(void);

public:
	void Release(void);

private:
	virtual void SetMatrix(const float fScale = 1.f);

private:
	bool m_bShort;
};