#pragma once

#include "Etc.h"

class CDoor : public CEtc
{
public:
	CDoor();
	~CDoor();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();
};