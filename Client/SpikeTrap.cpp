#include "stdafx.h"
#include "SpikeTrap.h"
#include "Tile.h"
#include "Monster.h"
#include "Device.h"

CSpikeTrap::CSpikeTrap()
: m_bDamage(false)
{
}

CSpikeTrap::~CSpikeTrap()
{
}

HRESULT CSpikeTrap::Initialize()
{
	TrapInit();

	m_iDrawId = 0;

	return S_OK;
}

const int CSpikeTrap::Progress()
{
	TrapProgress();

	Pattren();

	return 0;
}

void CSpikeTrap::Render()
{
	TrapRender();
}

void CSpikeTrap::Pattren()
{
	if(m_pObj != NULL && !m_bDamage)
	{
		if(m_pObj->GetObjId() == OBJ_MONSTER)
			if(((CMonster*)m_pObj)->GetIsFly())
				return;

		m_iDrawId = 2;

		m_pObj->SetDamage(2);

		m_bDamage = true;

		if(!m_pDevice->SoundPlaying(TEXT("Trap_Spike")))
			m_pDevice->SoundPlay(TEXT("Trap_Spike"), 0);
	}

	if(m_pObj == NULL)
		m_bDamage = false;
}