#pragma once
#include "vibuffer.h"

class CUnitTex :
	public CVIBuffer
{
public:
	CUnitTex(void);
	~CUnitTex(void);

	virtual HRESULT CreateBuffer();

	//virtual void Render();

};
