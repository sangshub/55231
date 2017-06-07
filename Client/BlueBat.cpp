#include "stdafx.h"
#include "BlueBat.h"
#include "Rand.h"
#include "BeatMgr.h"

CBlueBat::CBlueBat()
{
}

CBlueBat::~CBlueBat()
{
}

HRESULT CBlueBat::Initialize()
{
	m_tUnitInfo = UNIT(1, 2, 0);

	MonsterInit();

	m_bIsFly = true;

	m_strAttackSound = TEXT("Bat_Attack");
	m_strHitSound = TEXT("Bat_Hit");
	m_strDeathSound = TEXT("Bat_Death");

	return S_OK;
}

const int CBlueBat::Progress()
{
	MonsterProgress();

	if(m_bIsDelete)
		return 1;

	if(m_bFreeze)
		return 0;

	Pattren();

	if(m_bBeat)
		Move(m_vTarget);

	MegaJump();

	return 0;
}

void CBlueBat::Render()
{
	MonsterRender();
}

void CBlueBat::Pattren()
{
	if(m_pBeatMgr->GetBeatBoomBoom())
	{
		if(m_iCount == 2)
		{
			int iRand = CRand::Random(0, 3);

			switch(iRand)
			{
			case 0:
				MoveLeft();
				break;
			case 1:
				MoveRight();
				break;
			case 2:
				MoveUp();
				break;
			case 3:
				MoveDown();
				break;
			}
			m_iCount = 0;
		}
	}
}