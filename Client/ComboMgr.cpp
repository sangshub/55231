#include "stdafx.h"
#include "ComboMgr.h"
#include "Device.h"
#include "ObjMgr.h"

IMPLEMENT_SINGLETON(CComboMgr)

CComboMgr::CComboMgr(void)
: m_iCombo(0)
, m_pDevice(CDevice::GetInstance())
, m_pObjMgr(CObjMgr::GetInstance())
{
}

CComboMgr::~CComboMgr(void)
{
}

const int CComboMgr::GetCombo(void) const
{
	return m_iCombo;
}

void CComboMgr::SetCombo(const int iCombo)
{
	if(m_iCombo)
		m_pDevice->SoundPlay(TEXT("ComboStop"), 0);

	m_iCombo = iCombo;
	m_pObjMgr->SetTwinkle(false);
}

void CComboMgr::AddCombo(void)
{
	if(!m_iCombo)
		m_pDevice->SoundPlay(TEXT("ComboStart"), 0);

	if(m_iCombo < 2)
		m_iCombo += 1;

	m_pObjMgr->SetTwinkle(true);
}