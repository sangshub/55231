#include "stdafx.h"
#include "YellowSkeleton.h"
#include "Tile.h"
#include "Device.h"
#include "BeatMgr.h"
#include "ObjMgr.h"

CYellowSkeleton::CYellowSkeleton()
: m_bChange(false)
, m_iRunaway(0)
{
}

CYellowSkeleton::~CYellowSkeleton()
{
}

HRESULT CYellowSkeleton::Initialize()
{
	m_tUnitInfo = UNIT(3, 4, 0);

	MonsterInit();

	m_fFrameSpeed = 0.5f;

	m_strAttackSound = TEXT("Skel_Attack");
	m_strHitSound = TEXT("Skel_Hit");

	return S_OK;
}

const int CYellowSkeleton::Progress()
{
	MonsterProgress();

	if(m_tUnitInfo.iHp == 2)
	{
		if(!m_bChange)
		{
			if(m_tInfo.vPos.x > m_pObjMgr->GetPlayer()->GetInfo()->vPos.x)
				m_iRunaway = 0;
			else if(m_tInfo.vPos.x < m_pObjMgr->GetPlayer()->GetInfo()->vPos.x)
				m_iRunaway = 1;
			else if(m_tInfo.vPos.z > m_pObjMgr->GetPlayer()->GetInfo()->vPos.z)
				m_iRunaway = 2;
			else if(m_tInfo.vPos.z < m_pObjMgr->GetPlayer()->GetInfo()->vPos.z)
				m_iRunaway = 3;

			m_iCount = 0;
			m_tUnitInfo.iAtt = 0;
			m_fJumpPow = 0.3f;
			m_tFrame.fFrame = 0.f;
			m_wstrObjKey = TEXT("Yellow_Skeleton_NoHead");
			m_bChange = true;

			m_pDevice->SoundPlay(TEXT("Skel_Head_Loss"), 0);
		}
	}

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

void CYellowSkeleton::Render()
{
	MonsterRender();
}

void CYellowSkeleton::Pattren()
{
	if(m_pBeatMgr->GetBeatBoomBoom())
	{
		if(m_wstrObjKey == TEXT("Yellow_Skeleton_NoHead"))
		{
			if(m_iCount == 1)
			{
				m_bJump = true;

				switch(m_iRunaway)
				{
				case 0:
					MoveRight();
					break;
				case 1:
					MoveLeft();
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
			return;
		}
		if(m_iCount == 2)
		{
			m_bJump = true;

			MoveChase();

			m_iCount = 0;
		}
	}
}