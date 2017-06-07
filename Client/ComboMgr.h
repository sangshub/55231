#pragma once

#include "Include.h"

class CDevice;
class CObjMgr;

class CComboMgr
{
public:
	DECLARE_SINGLETON(CComboMgr)

private:
	CComboMgr(void);

public:
	~CComboMgr(void);

private:
	int			m_iCombo;
	CDevice*	m_pDevice;
	CObjMgr*	m_pObjMgr;

public:
	const int GetCombo(void) const;
	void SetCombo(const int iCombo);
	void AddCombo(void);
};