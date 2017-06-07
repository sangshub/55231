#pragma once

#include "VIBuffer.h"

class CCubeTex : public CVIBuffer
{
public:
	CCubeTex();
	~CCubeTex();

public:
	virtual HRESULT CreateBuffer();
};