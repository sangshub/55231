#pragma once

#include "Etc.h"

class CShopkeeper : public CEtc
{
public:
	CShopkeeper();
	~CShopkeeper();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();
};