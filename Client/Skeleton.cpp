#include "stdafx.h"
#include "Skeleton.h"
#include "Tile.h"
#include "BeatMgr.h"
#include "ObjMgr.h"

CSkeleton::CSkeleton()
{
}

CSkeleton::~CSkeleton()
{
}

HRESULT CSkeleton::Initialize()
{
	m_tUnitInfo = UNIT(1, 2, 0);

	MonsterInit();

	m_fFrameSpeed = 0.5f;

	m_strAttackSound = TEXT("Skel_Attack");
	m_strHitSound = TEXT("Skel_Hit");

	return S_OK;
}

const int CSkeleton::Progress()
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

void CSkeleton::Render()
{
	MonsterRender();
}

void CSkeleton::Pattren()
{
	if(m_pBeatMgr->GetBeatBoomBoom())
	{
		if(m_iCount == 2)
		{
			m_bJump = true;

			MoveChase();

			m_iCount = 0;
		}
	}
}