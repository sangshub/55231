#include "stdafx.h"
#include "BlueBanshee.h"
#include "Tile.h"
#include "Device.h"
#include "BeatMgr.h"
#include "ObjMgr.h"

CBlueBanshee::CBlueBanshee()
: m_bCry(false)
{
}

CBlueBanshee::~CBlueBanshee()
{
}

HRESULT CBlueBanshee::Initialize()
{
	m_tUnitInfo = UNIT(4, 6, 0);

	MonsterInit();

	m_fScale = 1.62f;

	m_bIsFly = true;

	m_fBaseY = 2.f;
	m_fMegaY = 2.f;

	m_strAttackSound = TEXT("Banshee_Attack");
	m_strHitSound = TEXT("Banshee_Hit");
	m_strDeathSound = TEXT("Banshee_Death");
	m_strInitSound = TEXT("Banshee_Cry");

	return S_OK;
}

const int CBlueBanshee::Progress()
{
	MonsterProgress();

	if(m_bIsDelete)
	{
		m_pDevice->SoundStop(TEXT("Banshee_Loop"));

		m_pDevice->SetSoundVolume(TEXT("Zone1_1"), 0);
		m_pDevice->SetSoundVolume(TEXT("Zone2_1"), 0);
		m_pDevice->SetSoundVolume(TEXT("Zone3_1"), 0);

		return 1;
	}

	if(m_bFreeze)
		return 0;

	if(!m_bCry)
	{
		if(m_tUnitInfo.iHp <= (m_tUnitInfo.iMaxHp<<1) - 1)
		{
			m_pDevice->SoundPlay(TEXT("Banshee_Loop"), 1);

			m_pDevice->SetSoundVolume(TEXT("Zone1_1"), -3000);
			m_pDevice->SetSoundVolume(TEXT("Zone2_1"), -3000);
			m_pDevice->SetSoundVolume(TEXT("Zone3_1"), -3000);

			m_wstrObjKey = TEXT("Blue_Banshee_Cry");

			m_bCry = true;
		}
	}

	Pattren();
	MegaJump();

	if(m_bBeat)
		Move(m_vTarget);

	return 0;
}

void CBlueBanshee::Render()
{
	MonsterRender();
}

void CBlueBanshee::Pattren()
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