#include "stdafx.h"
#include "FireBallStart.h"
#include "Player.h"
#include "Tile.h"
#include "ObjMgr.h"

CFireBallStart::CFireBallStart()
{
}

CFireBallStart::~CFireBallStart()
{
}

HRESULT CFireBallStart::Initialize()
{
	m_wstrObjKey = TEXT("FireBall");
	m_wstrStateKey = TEXT("FireBall");

	EffectInit();

	m_eObjId = OBJ_EFFECT;

	return S_OK;
}

const int CFireBallStart::Progress()
{
	if(!m_bInit)
	{
		SetScreenMode();

		if(m_bLeft)
		{
			if(m_iScreenMode == 0 || m_iScreenMode == 2)
			{
				int iNewIndex = m_pObjMgr->GetIndex(m_tInfo.vPos) - (m_pObjMgr->GetTileZ()<<1);

				if(iNewIndex < 0 || iNewIndex > m_pObjMgr->GetTileX() * m_pObjMgr->GetTileZ() - 1)
					return 1;

				m_tInfo.vPos = m_pObjMgr->GetTile(iNewIndex)->GetInfo()->vPos;

				m_vecDamageTile.push_back(iNewIndex + m_pObjMgr->GetTileZ());
				m_vecDamageTile.push_back(iNewIndex);
				m_vecDamageTile.push_back(iNewIndex - m_pObjMgr->GetTileZ());
				m_vecDamageTile.push_back(iNewIndex - (m_pObjMgr->GetTileZ()<<1));
				m_vecDamageTile.push_back(iNewIndex - (m_pObjMgr->GetTileZ() * 3));
			}
			else
			{
				int iNewIndex = m_pObjMgr->GetIndex(m_tInfo.vPos) + 2;

				if(iNewIndex < 0 || iNewIndex > m_pObjMgr->GetTileX() * m_pObjMgr->GetTileZ() - 1)
					return 1;

				m_tInfo.vPos = m_pObjMgr->GetTile(iNewIndex)->GetInfo()->vPos;

				m_vecDamageTile.push_back(iNewIndex - 1);
				m_vecDamageTile.push_back(iNewIndex);
				m_vecDamageTile.push_back(iNewIndex + 1);
				m_vecDamageTile.push_back(iNewIndex + 2);
				m_vecDamageTile.push_back(iNewIndex + 3);
			}
		}
		else
		{
			if(m_iScreenMode == 0 || m_iScreenMode == 2)
			{
				int iNewIndex = m_pObjMgr->GetIndex(m_tInfo.vPos) + (m_pObjMgr->GetTileZ()<<1);

				if(iNewIndex < 0 || iNewIndex > m_pObjMgr->GetTileX() * m_pObjMgr->GetTileZ() - 1)
					return 1;

				m_tInfo.vPos = m_pObjMgr->GetTile(iNewIndex)->GetInfo()->vPos;

				m_vecDamageTile.push_back(iNewIndex - m_pObjMgr->GetTileZ());
				m_vecDamageTile.push_back(iNewIndex);
				m_vecDamageTile.push_back(iNewIndex + m_pObjMgr->GetTileZ());
				m_vecDamageTile.push_back(iNewIndex + (m_pObjMgr->GetTileZ()<<1));
				m_vecDamageTile.push_back(iNewIndex + (m_pObjMgr->GetTileZ() * 3));
			}
			else
			{
				int iNewIndex = m_pObjMgr->GetIndex(m_tInfo.vPos) - 2;

				if(iNewIndex < 0 || iNewIndex > m_pObjMgr->GetTileX() * m_pObjMgr->GetTileZ() - 1)
					return 1;

				m_tInfo.vPos = m_pObjMgr->GetTile(iNewIndex)->GetInfo()->vPos;

				m_vecDamageTile.push_back(iNewIndex + 1);
				m_vecDamageTile.push_back(iNewIndex);
				m_vecDamageTile.push_back(iNewIndex - 1);
				m_vecDamageTile.push_back(iNewIndex - 2);
				m_vecDamageTile.push_back(iNewIndex - 3);
			}
		}

		m_tInfo.vPos.y = 2.5f;

		for(int i = 0; i < 5; ++i)
		{
			if(m_vecDamageTile[i] < 0 || m_vecDamageTile[i] > m_pObjMgr->GetTileX() * m_pObjMgr->GetTileZ() - 1)
				continue;

			if(m_pObjMgr->GetTile(m_vecDamageTile[i])->GetObj() != NULL && m_pObjMgr->GetTile(m_vecDamageTile[i])->GetTileObjId() != OBJ_PLAYER)
				m_pObjMgr->GetTile(m_vecDamageTile[i])->GetObj()->SetDamage(2);
		}

		m_bInit = true;
	}

	SetMatrix();
	FrameMove();
	SetFrame();

	if(m_tFrame.fFrame > m_tFrame.fMax - 0.5f)
		return 1;

	return 0;
}

void CFireBallStart::Render()
{
	EffectRender();
}