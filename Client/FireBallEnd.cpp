#include "stdafx.h"
#include "FireBallEnd.h"
#include "Tile.h"
#include "ObjMgr.h"

CFireBallEnd::CFireBallEnd()
: iRange(7)
{
}

CFireBallEnd::~CFireBallEnd()
{
}

HRESULT CFireBallEnd::Initialize()
{
	m_wstrObjKey = TEXT("FireBall");
	m_wstrStateKey = TEXT("FireBall_UL");

	EffectInit();

	m_eObjId = OBJ_EFFECT;

	return S_OK;
}

const int CFireBallEnd::Progress()
{
	if(!m_bInit)
	{
		SetScreenMode();

		if(m_bLeft)
		{
			if(m_iScreenMode == 0 || m_iScreenMode == 2)
			{
				int iNewIndex = m_pObjMgr->GetIndex(m_tInfo.vPos) - (m_pObjMgr->GetTileZ() * iRange);

				if(iNewIndex < 0 || iNewIndex > m_pObjMgr->GetTileX() * m_pObjMgr->GetTileZ() - 1)
				{
					--iRange;

					if(iRange > 2)
						Progress();
					else
						return 1;

					return 0;
				}

				m_tInfo.vPos = m_pObjMgr->GetTile(iNewIndex)->GetInfo()->vPos;

				m_vecDamageTile.push_back(iNewIndex + m_pObjMgr->GetTileZ());
				m_vecDamageTile.push_back(iNewIndex);
				m_vecDamageTile.push_back(iNewIndex - m_pObjMgr->GetTileZ());
				m_vecDamageTile.push_back(iNewIndex - (m_pObjMgr->GetTileZ()<<1));
			}
			else
			{
				int iNewIndex = m_pObjMgr->GetIndex(m_tInfo.vPos) + iRange;

				if(iNewIndex < 0 || iNewIndex > m_pObjMgr->GetTileX() * m_pObjMgr->GetTileZ() - 1)
				{
					--iRange;

					if(iRange > 2)
						Progress();
					else
						return 1;

					return 0;
				}

				m_tInfo.vPos = m_pObjMgr->GetTile(iNewIndex)->GetInfo()->vPos;
				
				m_vecDamageTile.push_back(iNewIndex - 1);
				m_vecDamageTile.push_back(iNewIndex);
				m_vecDamageTile.push_back(iNewIndex + 1);
				m_vecDamageTile.push_back(iNewIndex + 2);
			}
		}
		else
		{
			if(m_iScreenMode == 0 || m_iScreenMode == 2)
			{
				int iNewIndex = m_pObjMgr->GetIndex(m_tInfo.vPos) + (m_pObjMgr->GetTileZ() * iRange);

				if(iNewIndex < 0 || iNewIndex > m_pObjMgr->GetTileX() * m_pObjMgr->GetTileZ() - 1)
				{
					--iRange;

					if(iRange > 2)
						Progress();
					else
						return 1;

					return 0;
				}

				m_tInfo.vPos = m_pObjMgr->GetTile(iNewIndex)->GetInfo()->vPos;

				m_vecDamageTile.push_back(iNewIndex - m_pObjMgr->GetTileZ());
				m_vecDamageTile.push_back(iNewIndex);
				m_vecDamageTile.push_back(iNewIndex + m_pObjMgr->GetTileZ());
				m_vecDamageTile.push_back(iNewIndex + (m_pObjMgr->GetTileZ()<<1));
			}
			else
			{
				int iNewIndex = m_pObjMgr->GetIndex(m_tInfo.vPos) - iRange;

				if(iNewIndex < 0 || iNewIndex > m_pObjMgr->GetTileX() * m_pObjMgr->GetTileZ() - 1)
				{
					++iRange;

					if(iRange > 2)
						Progress();
					else
						return 1;

					return 0;
				}

				m_tInfo.vPos = m_pObjMgr->GetTile(iNewIndex)->GetInfo()->vPos;

				m_vecDamageTile.push_back(iNewIndex + 1);
				m_vecDamageTile.push_back(iNewIndex);
				m_vecDamageTile.push_back(iNewIndex - 1);
				m_vecDamageTile.push_back(iNewIndex - 2);
			}
		}

		m_tInfo.vPos.y = 2.5f;

		for(int i = 0; i < 4; ++i)
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

void CFireBallEnd::Render()
{
	EffectRender();
}