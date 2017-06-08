#pragma once

#include "Obj.h"

class CItem : public CObj
{
public:
	CItem();
	~CItem();

public:
	virtual HRESULT Initialize();
	virtual int Progress();
	virtual void Render();

private:
	virtual void SetMatrix();

private:
	wstring m_wstrStateKey;
};