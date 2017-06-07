#include "stdafx.h"
#include "Ghost.h"
#include "Player.h"
#include "BeatMgr.h"
#include "ObjMgr.h"

CGhost::CGhost()
{
}

CGhost::~CGhost()
{
}

HRESULT CGhost::Initialize()
{
	m_tUnitInfo = UNIT(2, 2, 0);

	MonsterInit();

	m_bIsFly = true;

	m_strAttackSound = TEXT("Ghost_Attack");
	m_strDeathSound = TEXT("Ghost_Death");

	return S_OK;
}

const int CGhost::Progress()
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

void CGhost::Render()
{
	MonsterRender();
}

void CGhost::Pattren()
{
	D3DXVECTOR3 vPos = m_pObjMgr->GetPlayer()->GetInfo()->vPos - m_tInfo.vPos;

	if(m_pBeatMgr->GetBeatBoomBoom())
	{
		if(m_iCount == 1)
		{
			float fAngle = D3DXToDegree(m_pObjMgr->GetPlayer()->GetAngleY());

			float fPlayerAngle = D3DXToDegree(((CPlayer*)m_pObjMgr->GetPlayer())->GetCamAngleY());

			fAngle += fPlayerAngle;

			if(fAngle >= 360.f)
			{
				int i = int(fAngle / 360.f);

				fAngle = fAngle - 360.f * i;
			}

			if(fAngle < 0.f)
			{
				int i = int(-fAngle / 360.f);

				fAngle = 360.f * (i + 1) + fAngle;
			}

			if(fAngle == 360.f)
				fAngle = 0.f;

			m_iAlpha = 70;
			m_bHide = true;

			if(fAngle == 270.f)
			{
				if(m_tInfo.vPos.x >= m_pObjMgr->GetPlayer()->GetInfo()->vPos.x)
				{
					MoveChase();
					m_iAlpha = 255;
					m_bHide = false;
				}
			}
			else if(fAngle == 0.f)
			{
				if(m_tInfo.vPos.z <= m_pObjMgr->GetPlayer()->GetInfo()->vPos.z)
				{
					MoveChase();
					m_iAlpha = 255;
					m_bHide = false;
				}

			}
			else if(fAngle == 90.f)
			{
				if(m_tInfo.vPos.x <= m_pObjMgr->GetPlayer()->GetInfo()->vPos.x)
				{
					MoveChase();
					m_iAlpha = 255;
					m_bHide = false;
				}
			}
			else if(fAngle == 180.f)
			{
				if(m_tInfo.vPos.z >= m_pObjMgr->GetPlayer()->GetInfo()->vPos.z)
				{
					MoveChase();
					m_iAlpha = 255;
					m_bHide = false;
				}
			}
			m_iCount = 0;
		}
	}
}