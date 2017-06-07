#pragma once

#include "ViBuffer.h"

class CUITex : public CVIBuffer
{
public:
	CUITex();
	~CUITex();

public:
	virtual HRESULT CreateBuffer();
};