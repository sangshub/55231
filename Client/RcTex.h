#pragma once;

#include "VIBuffer.h"

class CRcTex : public CVIBuffer
{
public:
	CRcTex();
	~CRcTex();

public:
	virtual HRESULT CreateBuffer();
};