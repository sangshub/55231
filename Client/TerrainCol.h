#pragma once

#include "VIBuffer.h"

class CTerrainCol : public CVIBuffer
{
public:
	CTerrainCol();
	~CTerrainCol();

public:
	virtual HRESULT CreateBuffer();
};