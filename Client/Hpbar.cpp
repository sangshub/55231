#include "StdAfx.h"
#include "Hpbar.h"

#include "ObjFactory.h"

#include "HP.h"
CHpbar::CHpbar(void)
:m_iMaxHpNum(0)
,m_iHp(0)
{
}

CHpbar::~CHpbar(void)
{
	Release();
}

HRESULT CHpbar::Initialize(void)
{

	return S_OK;
}

const int CHpbar::Progress(void)
{
	CheckHp();

	for(size_t i = 0; i < m_vecHpBar.size(); i++)
	{
		m_vecHpBar[i]->Progress();
	}
	
	return 0;
}

void CHpbar::Render(void)
{
	for(size_t i = 0; i < m_vecHpBar.size(); i++)
	{
		m_vecHpBar[i]->Render();
	}
	}

void CHpbar::Release(void)
{
	::Safe_Delete_Array(m_OriVtxCnt);
	for_each(m_vecHpBar.begin(), m_vecHpBar.end(), CDeleteObj());

}

void CHpbar::SetMaxHp(int iMaxHp)
{
	m_iMaxHpNum = iMaxHp;

}

void CHpbar::SetHp(int iHp)
{
	m_iHp = iHp;

}

void CHpbar::AddHp(int HpNum)
{
	CHP* pObj = new CHP;
	pObj->SetHpNumber(HpNum);
	pObj->SetVtxTex(m_OriVtxCnt);
	pObj->SetMaxHpNum(m_iMaxHpNum);
	pObj->SetPlayerHp(m_iHp);
	pObj->Initialize();
	m_vecHpBar.push_back(pObj);

}
void CHpbar::PopHp()
{
	m_vecHpBar.pop_back();

}

void CHpbar::CheckHp()
{
	int iHpBar = m_vecHpBar.size();

	if(m_iMaxHpNum == iHpBar)
	{
		return;
	}
	else if(m_iMaxHpNum > iHpBar)
	{
		AddHp(iHpBar+1);
	}
	else
	{
		PopHp();
	}

}
void CHpbar::RefreshHp(int iHp, int iMaxHp)
{
	m_iHp = iHp;
	m_iMaxHpNum = iMaxHp;

	unsigned int iSize = m_vecHpBar.size();
	for(size_t i = 0; i < iSize; i++)
	{
		m_vecHpBar[i]->SetPlayerHp(m_iHp);
	}
}