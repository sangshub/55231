#pragma once

#include "UI.h"

class CHP :public CUI
{
public:
	CHP(void);
	virtual ~CHP(void);
public:
	virtual HRESULT Initialize(void);
	virtual const int Progress(void);
	virtual void Render(void);

private:
	virtual void Release(void);

private:
	int		m_iHpNum;
	int		m_iMaxHpNum;
	int		m_iCount;
	int		m_iPlayerHp;
	int		m_iFrame;
public:
	void	SetHpNumber(int hpNum);
	void	SetVtxTex(VTXTEX*	pVtxTex);
	void	SetMaxHpNum(int iMaxHpNum);
	void	SetPlayerHp(int iPlayerHp);
	int		ChooseHpState(void);
};
