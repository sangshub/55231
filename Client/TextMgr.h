#pragma once

#include "Include.h"

class CDevice;
class CTextureMgr;

class CTextMgr
{
public:
	DECLARE_SINGLETON(CTextMgr)

private:
	CTextMgr(void);

public:
	~CTextMgr(void);

private:
	CDevice*		m_pDevice;
	CTextureMgr*	m_pTextureMgr;
	
public:
	void TextDraw(float fX, float fY, wstring wstrStateKey,float fScaleX = 1.f, float fScaleY = 1.f);
};