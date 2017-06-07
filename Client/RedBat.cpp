#include "stdafx.h"
#include "RedBat.h"
#include "Rand.h"
#include "BeatMgr.h"

CRedBat::CRedBat()
{
}

CRedBat::~CRedBat()
{
}

HRESULT CRedBat::Initialize()
{
	m_tUnitInfo = UNIT(2, 2, 0);

	MonsterInit();

	m_bIsFly = true;

	m_strAttackSound = TEXT("Bat_Attack");
	m_strHitSound = TEXT("Bat_Hit");
	m_strDeathSound = TEXT("Bat_Death");

	return S_OK;
}

const int CRedBat::Progress()
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

void CRedBat::Render()
{
	MonsterRender();
}

void CRedBat::Pattren()
{
	if(m_pBeatMgr->GetBeatBoomBoom())
	{
		if(m_iCount == 1)
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