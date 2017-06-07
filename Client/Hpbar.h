#pragma once
#include "ui.h"

class CHP;
class CHpbar :
	public CUI
{
public:
	CHpbar(void);
	virtual ~CHpbar(void);

public:
	virtual HRESULT Initialize(void);
	virtual const int Progress(void);
	virtual void Render(void);

private:
	virtual void Release(void);

public:
	void SetMaxHp(int iMaxHp);
	void SetHp(int iHp);
	void AddHp(int HpNum);
	void PopHp();
	void CheckHp();
	void RefreshHp(int iHp, int iMaxHp);

private:
	vector<CHP*>	m_vecHpBar;
	int				m_iHp;
	int				m_iMaxHpNum;
	




};
