#include "stdafx.h"
#include "GreenDragon.h"
#include "Tile.h"
#include "BeatMgr.h"
#include "ObjMgr.h"
#include "CamMgr.h"

CGreenDragon::CGreenDragon()
{
}

CGreenDragon::~CGreenDragon()
{
}

HRESULT CGreenDragon::Initialize()
{
	m_tUnitInfo = UNIT(4, 8, 0);

	MonsterInit();

	m_fScale = 2.25f;

	m_fFrameSpeed = 2.f;

	m_bCubeBreak = true;
	m_bIsFly = true;

	m_fBaseY = 2.5f;
	m_fMegaY = 2.5f;
	m_fJumpPow = 0.3f;

	m_strAttackSound = TEXT("Dragon_Attack");
	m_strHitSound = TEXT("Dragon_Hit");
	m_strDeathSound = TEXT("Dragon_Death");
	m_strMoveSound = TEXT("Dragon_Move");
	m_strInitSound = TEXT("Dragon_Cry");

	return S_OK;
}

const int CGreenDragon::Progress()
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

void CGreenDragon::Render()
{
	MonsterRender();
}

void CGreenDragon::Pattren()
{
	if(m_pBeatMgr->GetBeatBoomBoom())
	{
		if(m_iCount == 2)
		{
			m_pCamMgr->SetShake();

			m_bJump = true;

			MoveChase();

			m_iCount = 0;
		}
	}
}