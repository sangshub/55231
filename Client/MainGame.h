#pragma once

#include "Include.h"

class CDevice;
class CTimeMgr;
class CKeyMgr;
class CCamMgr;
class CBufferMgr;
class CTextureMgr;
class CSceneMgr;
class CObjMgr;
class CBeatMgr;
class CParticleMgr;
class CComboMgr;
class CItemTableMgr;

class CMainGame
{
public:
	CMainGame(void);
	~CMainGame(void);

public:
	HRESULT Initialize(void);
	void Progress(void);
	void Render(void);

private:
	void Release(void);

private:
	int m_iFpsCnt;
	float m_fTime;
	TCHAR m_szFps[MIN_STR];

	CDevice*		m_pDevice;
	CTimeMgr*		m_pTimeMgr;
	CKeyMgr*		m_pKeyMgr;
	CCamMgr*		m_pCamMgr;
	CBufferMgr*		m_pBufferMgr;
	CTextureMgr*	m_pTextureMgr;
	CSceneMgr*		m_pSceneMgr;
	CObjMgr*		m_pObjMgr;
	CBeatMgr*		m_pBeatMgr;
	CParticleMgr*	m_pParticleMgr;
	CComboMgr*		m_pComboMgr;
	CItemTableMgr*	m_pItemTableMgr;
};