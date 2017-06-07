#include "stdafx.h"
#include "OrangeSlime.h"
#include "BeatMgr.h"

COrangeSlime::COrangeSlime()
{
}

COrangeSlime::~COrangeSlime()
{
}

HRESULT COrangeSlime::Initialize()
{
	m_tUnitInfo = UNIT(1, 2, 0);

	MonsterInit();

	m_strAttackSound = TEXT("Slime_Attack");
	m_strHitSound = TEXT("Slime_Hit");
	m_strDeathSound = TEXT("Slime_Death");

	m_fJumpPow = 0.3f;

	return S_OK;
}

const int COrangeSlime::Progress()
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

void COrangeSlime::Render()
{
	MonsterRender();
}

void COrangeSlime::Pattren()
{
	if(m_pBeatMgr->GetBeatBoomBoom())
	{
		switch(m_iCount)
		{
		case 1:
			m_bJump = true;

			MoveRight();

			if(!m_bBeat)
				m_iCount = 0;
			break;
		case 2:
			m_bJump = true;

			MoveDown();

			if(!m_bBeat)
				m_iCount = 1;
			break;
		case 3:
			m_bJump = true;

			MoveLeft();

			if(!m_bBeat)
				m_iCount = 2;
			break;
		case 4:
			m_bJump = true;

			MoveUp();

			if(!m_bBeat)
				m_iCount = 3;
			else
				m_iCount = 0;
			break;
		}
	}
}