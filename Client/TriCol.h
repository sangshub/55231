#pragma once

#include "VIBuffer.h"

class CTriCol : public CVIBuffer
{
public:
	CTriCol();
	~CTriCol();

public:
	virtual HRESULT CreateBuffer();;
};