#include "stdafx.h"
#include "Monkey.h"
#include "Player.h"
#include "Tile.h"
#include "SmallHeart.h"
#include "Device.h"
#include "BeatMgr.h"
#include "ObjMgr.h"
#include "ObjFactory.h"

CMonkey::CMonkey()
: m_bGrab(false)
{
}

CMonkey::~CMonkey()
{
}

HRESULT CMonkey::Initialize()
{
	m_tUnitInfo = UNIT(0, 2, 0);

	MonsterInit();

	m_strHitSound = TEXT("Monkey_Hit");

	return S_OK;
}

const int CMonkey::Progress()
{
	if(!m_bGrab)
	{
		if(m_bIsDelete)
			return 1;
	}
	else
	{
		if(m_tUnitInfo.iHp <= 0)
		{
			m_bIsDelete = true;
			((CPlayer*)m_pObjMgr->GetPlayer())->SetIsMonkey(NULL);
		}
	}

	MonsterProgress();

	if(m_bIsDelete)
		return 1;

	if(m_bFreeze)
		return 0;

	Pattren();
	MegaJump();

	if(m_bBeat && !m_bGrab)
		Move(m_vTarget);

	return 0;
}

void CMonkey::Render()
{
	MonsterRender();
}

void CMonkey::Pattren()
{
	if(m_bGrab)
	{
		switch(m_iSceenMode)
		{
		case 0:
			m_tInfo.vPos = m_pObjMgr->GetPlayer()->GetInfo()->vPos + D3DXVECTOR3(0.f, 2.f, -1.f);
			break;
		case 1:
			m_tInfo.vPos = m_pObjMgr->GetPlayer()->GetInfo()->vPos + D3DXVECTOR3(-1.f, 2.f, 0.f);
			break;
		case 2:
			m_tInfo.vPos = m_pObjMgr->GetPlayer()->GetInfo()->vPos + D3DXVECTOR3(0.f, 2.f, 1.f);
			break;
		case 3:
			m_tInfo.vPos = m_pObjMgr->GetPlayer()->GetInfo()->vPos + D3DXVECTOR3(1.f, 2.f, 0.f);
			break;
		}

		m_wstrStateKey = TEXT("Grab");

		SetFrame();

		return;
	}

	if(m_pBeatMgr->GetBeatBoomBoom())
	{
		if(m_iCount == 1)
		{
			MoveChase();
			Grab();

			m_iCount = 0;
		}
	}
}

void CMonkey::Grab()
{
	if(m_pObjMgr->GetTile(m_iNewIndex)->GetObj() == NULL)
		return;

	if(m_pObjMgr->GetTile(m_iNewIndex)->GetTileObjId() == OBJ_PLAYER)
	{
		if(((CPlayer*)m_pObjMgr->GetPlayer())->GetIsMonkey() != NULL)
			return;

		if(m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->GetObj() == this)
			m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->ResetObj();

		D3DXVECTOR3 vPos = m_pObjMgr->GetTile(m_iNewIndex)->GetInfo()->vPos;
		vPos.y = 2.f;

		m_tUnitInfo = UNIT(0, 8, 0);

		for(int i = 0; i < 3; ++i)
		{
			CSmallHeart* pHp = (CSmallHeart*)CObjFactory<CSmallHeart>::CreateObj();
			m_pObjMgr->AddObj(OBJ_EFFECT, pHp);
			m_vecHp.push_back(pHp);
		}

		m_tFrame.fFrame = 0.f;

		m_bGrab = true;

		((CPlayer*)m_pObjMgr->GetPlayer())->SetIsMonkey(this);

		m_pDevice->SoundPlay(TEXT("Monkey_Grab"), 0);

		return;
	}
}