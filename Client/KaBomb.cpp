#include "stdafx.h"
#include "KaBomb.h"
#include "EffectBomb.h"
#include "Tile.h"
#include "Device.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "BeatMgr.h"
#include "TextureMgr.h"

CKaBomb::CKaBomb(void)
{
}

CKaBomb::~CKaBomb(void)
{
}

HRESULT CKaBomb::Initialize()
{
	m_wstrObjKey = TEXT("Bomb");
	m_wstrStateKey = TEXT("Normal");

	/*m_tFrame.fCnt = 0;
	m_tFrame.fFrame = 0;
	m_tFrame.fMax= 0;*/

	EffectInit();

	m_eObjId = OBJ_EFFECT;

	return S_OK;
}

const int CKaBomb::Progress()
{
	float fMaxCnt = (float)m_pTextureMgr->GetImgCount(m_wstrObjKey, m_wstrStateKey, TEXT(""));

	SetMatrix();
	SetFrame((1.f / fMaxCnt) * m_pBeatMgr->GetBeatMax() * 3.f);
	FrameMove();
	
 	if(m_tFrame.fFrame > m_tFrame.fMax - 0.5f)
	{
		CObj* pEffectBomb = NULL;

		for(int i = 0; i < 3; ++i)
		{
			for(int j = 0; j < 3; ++j)
			{
				pEffectBomb = CObjFactory<CEffectBomb>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x - TILESIZE +(i * TILESIZE), 1.f ,m_tInfo.vPos.z- TILESIZE +(j * TILESIZE)));
				m_pObjMgr->AddObj(OBJ_EFFECT, pEffectBomb);

				CTile* pTile = m_pObjMgr->GetTile(m_pObjMgr->GetIndex(D3DXVECTOR3(m_tInfo.vPos.x - TILESIZE +(i * TILESIZE), 1.f ,m_tInfo.vPos.z- TILESIZE +(j * TILESIZE))));

				if(pTile->GetObj() == NULL)
					continue;
				else
				{
					if(pTile->GetTileObjId() == OBJ_CUBE)
					{
						if((pTile->GetObj())->GetCubeInfo().dwType & CUBE_NOTB)
							continue;

						pTile->GetObj()->SetDelete();
						pTile->ResetObj();
					}
					else if(pTile->GetTileObjId() == OBJ_MONSTER || pTile->GetTileObjId() == OBJ_PLAYER)
						pTile->GetObj()->SetDamage(4);
					/*else
						continue;*/
				}
			}
		}
		m_pDevice->SoundPlay(TEXT("Bomb"), 0);

		return 1;
	}
	return 0;
}

void CKaBomb::Render()
{
	EffectRender();	
}