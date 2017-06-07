#include "stdafx.h"
#include "BlueSlime.h"
#include "BeatMgr.h"

CBlueSlime::CBlueSlime()
{
}

CBlueSlime::~CBlueSlime()
{
}

HRESULT CBlueSlime::Initialize()
{
	m_tUnitInfo = UNIT(2, 4, 0);

	MonsterInit();

	m_strAttackSound = TEXT("Slime_Attack");
	m_strHitSound = TEXT("Slime_Hit");
	m_strDeathSound = TEXT("Slime_Death");

	return S_OK;
}

const int CBlueSlime::Progress()
{
	MonsterProgress();

	if(m_bIsDelete)
		return 1;

	if(m_bFreeze)
		return 0;

	Pattren();
	Jump();
	MegaJump();

	if(m_bBeat)
		Move(m_vTarget);

	return 0;
}

void CBlueSlime::Render()
{
	MonsterRender();
}

void CBlueSlime::Pattren()
{
	if(m_pBeatMgr->GetBeatBoomBoom())
	{
		switch(m_iCount)
		{
		case 2:
			m_bJump = true;

			MoveDown();

			if(!m_bBeat)
				m_iCount = 0;
			break;
		case 4:
			m_bJump = true;

			MoveUp();

			if(!m_bBeat)
				m_iCount = 2;
			else
				m_iCount = 0;
			break;
		}
	}
}