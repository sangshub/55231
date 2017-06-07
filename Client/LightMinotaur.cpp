#include "stdafx.h"
#include "LightMinotaur.h"
#include "Tile.h"
#include "Device.h"
#include "BeatMgr.h"
#include "ObjMgr.h"

CLightMinotaur::CLightMinotaur()
: m_iChargeDir(0)
{
}

CLightMinotaur::~CLightMinotaur()
{
}

HRESULT CLightMinotaur::Initialize()
{
	m_tUnitInfo = UNIT(4, 6, 0);

	MonsterInit();

	m_fScale = 2.08f;

	m_bCubeBreak = true;
	m_bIsFly = true;

	m_fBaseY = 2.5f;
	m_fMegaY = 2.5f;
	m_fJumpPow = 0.3f;

	m_strAttackSound = TEXT("Minotaur_Attack");
	m_strHitSound = TEXT("Minotaur_Hit");
	m_strDeathSound = TEXT("Minotaur_Death");
	m_strInitSound = TEXT("Minotaur_Cry");

	return S_OK;
}

const int CLightMinotaur::Progress()
{
	MonsterProgress();

	if(m_bIsDelete)
		return 1;

	if(m_bFreeze)
		return 0;

	Pattren();

	if(m_wstrObjKey != TEXT("Light_Minotaur_Stun"))
		Jump();

	MegaJump();

	if(m_bBeat)
		Move(m_vTarget);

	return 0;
}

void CLightMinotaur::Render()
{
	MonsterRender();
}

void CLightMinotaur::Pattren()
{
	if(m_wstrObjKey == TEXT("Light_Minotaur_Charge"))
		m_tUnitInfo.iAtt = 4;
	else
		m_tUnitInfo.iAtt = 0;

	if(m_pBeatMgr->GetBeatBoomBoom())
	{
		switch(m_iCount)
		{
		case 1:
			m_bJump = true;

			if(m_wstrObjKey == TEXT("Light_Minotaur"))
			{
				MoveChase();
				Charge();

				m_iCount = 0;
			}
			else if(m_wstrObjKey == TEXT("Light_Minotaur_Charge"))
			{
				switch(m_iChargeDir)
				{
				case 0:
					MoveLeft();
					break;
				case 1:
					MoveRight();
					break;
				case 2:
					MoveDown();
					break;
				case 3:
					MoveUp();
					break;
				}

				m_iCount = 0;
			}
			break;
		case 2:
			m_wstrObjKey = TEXT("Light_Minotaur");

			m_iCount = 0;
			break;
		}
	}
}

void CLightMinotaur::Charge()
{
	if(m_tInfo.vPos.z == m_pObjMgr->GetPlayer()->GetInfo()->vPos.z)
	{
		m_tFrame.fFrame = 0.f;
		m_wstrObjKey = TEXT("Light_Minotaur_Charge");
		m_pDevice->SoundPlay(TEXT("Minotaur_Charge"), 0);

		if(m_tInfo.vPos.x > m_pObjMgr->GetPlayer()->GetInfo()->vPos.x) // 왼쪽
			m_iChargeDir = 0;
		else // 오른쪽
			m_iChargeDir = 1;
	}
	else if(m_tInfo.vPos.x == m_pObjMgr->GetPlayer()->GetInfo()->vPos.x)
	{
		m_tFrame.fFrame = 0.f;
		m_wstrObjKey = TEXT("Light_Minotaur_Charge");
		m_pDevice->SoundPlay(TEXT("Minotaur_Charge"), 0);

		if(m_tInfo.vPos.z > m_pObjMgr->GetPlayer()->GetInfo()->vPos.z) // 위
			m_iChargeDir = 2;
		else // 아래
			m_iChargeDir = 3;
	}
}