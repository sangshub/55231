#pragma once

#include "VIBuffer.h"

class CRcCol : public CVIBuffer
{
public:
	CRcCol();
	~CRcCol();

public:
	virtual HRESULT CreateBuffer();
};