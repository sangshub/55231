#include "StdAfx.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr(void)
: m_dwKey(0)
{
	ZeroMemory(m_dwKeyPressed, sizeof(DWORD)*wKeyCount);
}

CKeyMgr::~CKeyMgr(void)
{
}

void CKeyMgr::KeyCheck(void)
{
	m_dwKey = 0;

	if(GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwKey |= KEY_LEFT;
	if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwKey |= KEY_RIGHT;
	if(GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwKey |= KEY_UP;
	if(GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwKey |= KEY_DOWN;
	if(GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwKey |= KEY_SPACE;
	if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwKey |= KEY_LBUTTON;
	if(GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwKey |= KEY_RBUTTON;
}

// 키확인 함수
KEYID CKeyMgr::KeyPressed(const DWORD dwKey, const int iIndex/* = 0*/)
{
	if(m_dwKey & dwKey)
	{
		if(!(m_dwKeyPressed[iIndex] & dwKey))
		{
			m_dwKeyPressed[iIndex] |= dwKey;

			return KEYDOWN; // 처음 눌렀을때
		}
		else if(m_dwKeyPressed[iIndex] & dwKey)
			return KEYDRAG; // 누르고 있을때
	}
	else if(!(m_dwKey & dwKey) && m_dwKeyPressed[iIndex] & dwKey)
	{
		m_dwKeyPressed[iIndex] = m_dwKey;

		return KEYUP; // 키를 땠을때
	}
	return KEYEND; // 아무것도 아닌상태
}

/* 사용 하는법

if(CKeyMgr::GetInstance()->KeyPressed() == KEYDOWN) // 눌렀을때확인
{
}

if(CKeyMgr::GetInstance()->KeyPressed() == KEYDRAG) // 드래그중 확인
{
}

if(CKeyMgr::GetInstance()->KeyPressed() == KEYUP) // 땠을때 확인
{
}
*/