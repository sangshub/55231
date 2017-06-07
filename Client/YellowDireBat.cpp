#include "stdafx.h"
#include "YellowDireBat.h"
#include "Rand.h"
#include "BeatMgr.h"

CYellowDireBat::CYellowDireBat()
{
}

CYellowDireBat::~CYellowDireBat()
{
}

HRESULT CYellowDireBat::Initialize()
{
	m_tUnitInfo = UNIT(3, 4, 0);

	MonsterInit();

	m_fWidth = 1.5f;

	m_bIsFly = true;

	m_strInitSound = TEXT("BigBat_Attack");
	m_strAttackSound = TEXT("BigBat_Attack");
	m_strHitSound = TEXT("BigBat_Hit");
	m_strDeathSound = TEXT("BigBat_Death");

	return S_OK;
}

const int CYellowDireBat::Progress()
{
	MonsterProgress();

	if(m_bIsDelete)
		return 1;

	if(m_bFreeze)
		return 0;

	Pattren();
	MegaJump();

	if(m_bBeat)
		Move(m_vTarget);

	return 0;
}

void CYellowDireBat::Render()
{
	MonsterRender();
}

void CYellowDireBat::Pattren()
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