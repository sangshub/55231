#include "stdafx.h"
#include "Wraith.h"
#include "Tile.h"
#include "BeatMgr.h"
#include "ObjMgr.h"
#include "Device.h"

CWratih::CWratih()
{
}

CWratih::~CWratih()
{
}

HRESULT CWratih::Initialize()
{
	m_tUnitInfo = UNIT(1, 2, 0);

	MonsterInit();

	m_fFrameSpeed = 0.5f;

	m_bIsFly = true;

	m_strAttackSound = TEXT("Wraith_Attack");
	m_strDeathSound = TEXT("Wraith_Death");

	return S_OK;
}

const int CWratih::Progress()
{
	if(m_bHide)
	{
		D3DXVECTOR3 vDir = m_pObjMgr->GetPlayer()->GetInfo()->vPos - m_tInfo.vPos;
		float fDistance = D3DXVec3Length(&vDir);

		if(fDistance < 10.f)
		{
			m_bHide = false;

			m_pDevice->SoundPlay(TEXT("Wraith_Cry"), 0);
		}

		return 0;
	}

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

void CWratih::Render()
{
	MonsterRender();
}

void CWratih::Pattren()
{
	if(m_pBeatMgr->GetBeatBoomBoom())
	{
		if(m_iCount == 1)
		{
			m_bJump = true;

			MoveChase();

			m_iCount = 0;
		}
	}
}