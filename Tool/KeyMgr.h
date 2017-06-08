#pragma once

#include "Include.h"

const WORD	wKeyCount = 50;

extern const WORD wKeyCount;

class CKeyMgr
{
public:
	DECLARE_SINGLETON(CKeyMgr)

private:
	CKeyMgr(void);

public:
	~CKeyMgr(void);

public:
	void KeyCheck(void);
	KEYID KeyPressed(const DWORD dwKey, const int iIndex = 0);

private:
	DWORD m_dwKey;
	DWORD m_dwKeyPressed[wKeyCount];
};