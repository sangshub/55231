#pragma once
#include "ui.h"

class CCoin :
	public CUI
{
public:
	CCoin(void);
	virtual ~CCoin(void);

	virtual HRESULT Initialize(void);

	virtual const int Progress(void);

	virtual void Render(void);

	virtual void Release(void);


};
