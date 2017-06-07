#include "stdafx.h"
#include "GreenSlime.h"
#include "BeatMgr.h"
#include "TimeMgr.h"

CGreenSlime::CGreenSlime()
{
}

CGreenSlime::~CGreenSlime()
{
}

HRESULT CGreenSlime::Initialize()
{
	m_tUnitInfo = UNIT(0, 2, 0);

	MonsterInit();

	m_strHitSound = TEXT("Slime_Hit");
	m_strDeathSound = TEXT("Slime_Death");
	
	return S_OK;
}

const int CGreenSlime::Progress()
{
	MonsterProgress();

	if(m_bIsDelete)
		return 1;

	if(m_bFreeze)
		return 0;

	Pattren();
	MegaJump();

	return 0;
}

void CGreenSlime::Render()
{
	MonsterRender();
}

void CGreenSlime::Pattren()
{
	if(m_pBeatMgr->GetBeatBoomBoom())
	{
		if(m_iCount == 4)
		{
			m_bBeat = true;
			m_iCount = 0;
		}
	}
	if(m_bBeat)
		JumpHere();
}

void CGreenSlime::JumpHere()
{
	m_fJumpTime += m_pTimeMgr->GetTime() / m_pBeatMgr->GetBeatMax();

	if(m_fJumpTime < 0.5f)
		m_tInfo.vPos.y += m_pTimeMgr->GetTime() * 10.f;
	else if(m_fJumpTime < 1.f)
		m_tInfo.vPos.y -= m_pTimeMgr->GetTime() * 10.f;
	else
	{
		m_tInfo.vPos.y = 1.f;
		m_fJumpTime = 0.f;
		m_bBeat = false;
	}
}