#pragma once

#include "UI.h"
class CDiamond : public CUI
{
public:
	CDiamond(void);
	virtual ~CDiamond(void);
public:
	virtual HRESULT Initialize(void);
	virtual const int Progress(void);
	virtual void Render(void);

private:
	virtual void Release(void);


};
