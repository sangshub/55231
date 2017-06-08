#include "stdafx.h"
#include "TileMap.h"
#include "Tile.h"
#include "Monster.h"
#include "TimeMgr.h"
#include "ObjFactory.h"
#include "ObjMgr.h"
#include "Mouse.h"
#include "BufferMgr.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "KeyMgr.h"
#include "Frustum.h"

IMPLEMENT_SINGLETON(CTileMap)

struct RenderObj
{
	void operator()(CObj* pObj)
	{
		pObj->Render();
	}
};

CTileMap::CTileMap()
: m_bTwinkle(false)
, m_iTileX(30)
, m_iTileZ(30)
, m_iDrawId(0)
, m_fTime(0.f)
, m_pTimeMgr(CTimeMgr::GetInstance())
, m_pMouse(CMouse::GetInstance())
, m_pBufferMgr(CBufferMgr::GetInstance())
, m_pKeyMgr(CKeyMgr::GetInstance())
, m_pFrustum(CFrustum::GetInstance())
, m_pVtxTex(NULL)
, m_pVtxTex2(NULL)
, m_pEtcTool(NULL)
, m_bCubeMode(false)
, m_bObjMode(false)
, m_bItemMode(false)
, m_bEtcMode(false)
{
}

CTileMap::~CTileMap()
{
	Release();

	::Safe_Delete_Array(m_pVtxTex);
	::Safe_Delete_Array(m_pVtxTex2);
	::Safe_Destroy(m_pMouse);
}

HRESULT CTileMap::Initialize()
{
	for(int i = 0; i < m_iTileX; ++i)
	{
		for(int j = 0; j < m_iTileZ; ++j)
		{
			CObj* pObj = CObjFactory<CTile>::CreateObj(D3DXVECTOR3(float(i * VTXITV * 2), 0.f, float(j * VTXITV * 2)));

			TILE* pTile = new TILE;

			pTile->vPos = pObj->GetInfo()->vPos;
			pTile->iOption = ((CTile*)pObj)->GetOption();
			pTile->iDrawId = ((CTile*)pObj)->GetDrawId();
			pTile->iCubeCnt = ((CTile*)pObj)->GetCubeCnt();
			pTile->iObjCnt = ((CTile*)pObj)->GetObjCnt();

			m_vecFullTile.push_back(pObj);
			m_vecTileInfo.push_back(pTile);
		}
	}

	m_pVtxTex = new VTXTEX[4];
	m_pBufferMgr->GetVtxInfo(TEXT("RcTex"), m_pVtxTex);

	m_pVtxTex2 = new VTXTEX[4];
	m_pBufferMgr->GetVtxInfo(TEXT("UnitTex"), m_pVtxTex2);

	m_pCubeTool = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->GetCubeTool();
	m_pObjTool= ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->GetObjTool();
	m_pItemTool = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->GetItemTool();
	m_pEtcTool = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->GetEtcTool();

	return S_OK;
}

void CTileMap::Progress()
{
	int iCullX = int(m_pFrustum->GetMinX())>>1;
	int iCullZ = int(m_pFrustum->GetMinZ())>>1;

	size_t iSize = m_vecFullTile.size();

	for(int i = 0; i < int(m_pFrustum->GetMaxZ())>>1; ++i)
	{
		for(int j = 0; j < int(m_pFrustum->GetMaxX())>>1; ++j)
		{
			int iIndex = (j + iCullX) * m_iTileZ + (i + iCullZ);

			if(iIndex < 0 || iIndex >= m_iTileX * m_iTileZ)
				break;

			if(m_pFrustum->FrustumCulling(m_vecFullTile[iIndex]->GetInfo()->vPos))
			{
				m_vecTile.push_back(m_vecFullTile[iIndex]);

				m_vecFullTile[iIndex]->Progress();
			}
		}
	}

	Twinkle();

	if(GetAsyncKeyState('P') & 0x0001)
	{
		if(m_bTwinkle)
			m_bTwinkle = false;
		else
			m_bTwinkle = true;
	}

	if(m_pKeyMgr->KeyPressed(KEY_LBUTTON, 0) == KEYDOWN)
	{
		size_t iSize = m_vecTile.size();

		for(size_t i = 0; i < iSize; ++i)
		{
			if(m_pMouse->PickObject(OBJ_TERRAIN, m_vecTile[i]))
			{
				if(m_bCubeMode)
				{
					if(((CTile*)m_vecTile[i])->GetObjCnt() != 0 || ((CTile*)m_vecTile[i])->GetCubeCnt() != 0)
						return;
					//D3DXVECTOR3 vTemp = m_vecTile[i]->GetInfo()->vPos;
					//vTemp.y += ((CTile*)m_vecTile[i])->GetCubeCnt() * 2 + 1;

					m_pCubeTool->CreateCube(m_vecTile[i]->GetInfo()->vPos, ((CTile*)m_vecTile[i])->GetCubeCnt());
					//((CTile*)m_vecTile[i])->SetCubeCnt(((CTile*)m_vecTile[i])->GetCubeCnt() + 1);
				}
				else if(m_bObjMode)
				{
					if(((CTile*)m_vecTile[i])->GetObjCnt() != 0 || ((CTile*)m_vecTile[i])->GetCubeCnt() != 0)
						return;

					SetResetVtx();
					
					m_pObjTool->CreateObj(m_vecTile[i]->GetInfo()->vPos);

					((CTile*)m_vecTile[i])->SetObjCnt(1);
				}
				else if(m_bItemMode)
				{
					if(((CTile*)m_vecTile[i])->GetObjCnt() != 0 || ((CTile*)m_vecTile[i])->GetCubeCnt() != 0)
						return;

					SetResetVtx();

					m_pItemTool->CreateItem(m_vecTile[i]->GetInfo()->vPos);

					((CTile*)m_vecTile[i])->SetObjCnt(1);
				}
				else if(m_bEtcMode)
				{
					if(((CTile*)m_vecTile[i])->GetObjCnt() != 0 || ((CTile*)m_vecTile[i])->GetCubeCnt() != 0)
						return;

					SetResetVtx();

					m_pEtcTool->CreateObj(m_vecTile[i]->GetInfo()->vPos);

					((CTile*)m_vecTile[i])->SetObjCnt(1);
				}
				/*else
				{
					((CTile*)m_vecTile[i])->SetDrawId(m_iDrawId);
				}*/
			}
		}
	}
	if(m_pKeyMgr->KeyPressed(KEY_LBUTTON, 0) == KEYDRAG)
	{
		size_t iSize = m_vecTile.size();

		for(size_t i = 0; i < iSize; ++i)
		{
			if(!m_bCubeMode && !m_bObjMode && !m_bItemMode && !m_bEtcMode)
			{
				if(m_pMouse->PickObject(OBJ_TERRAIN, m_vecTile[i]))
					((CTile*)m_vecTile[i])->SetDrawId(m_iDrawId);
			}
		}
	}
	if(m_pKeyMgr->KeyPressed(KEY_RBUTTON, 0) == KEYDOWN)
	{
		size_t iSize = m_vecTile.size();

		for(size_t i = 0; i < iSize; ++i)
		{
			if(m_pMouse->PickObject(OBJ_TERRAIN, m_vecTile[i]))
			{
				if(((CTile*)m_vecTile[i])->GetTileObj() == NULL)
					return;

				if(m_bCubeMode)
				{
					((CTile*)m_vecTile[i])->GetTileObj()->SetIsDelete();
					((CTile*)m_vecTile[i])->SetTileObj(NULL);
					((CTile*)m_vecTile[i])->SetCubeCnt(0);
					m_pCubeTool->DeleteCubeData(((CTile*)m_vecTile[i])->GetInfo()->vPos);
				}
				else if(m_bObjMode)
				{
					((CTile*)m_vecTile[i])->GetTileObj()->SetIsDelete();
					((CTile*)m_vecTile[i])->SetTileObj(NULL);
					((CTile*)m_vecTile[i])->SetObjCnt(0);
					m_pObjTool->DeleteObjData(((CTile*)m_vecTile[i])->GetInfo()->vPos);
				}
				else if(m_bItemMode)
				{
					((CTile*)m_vecTile[i])->GetTileObj()->SetIsDelete();
					((CTile*)m_vecTile[i])->SetTileObj(NULL);
					((CTile*)m_vecTile[i])->SetObjCnt(0);
					m_pItemTool->DeleteItemData(((CTile*)m_vecTile[i])->GetInfo()->vPos);
				}
				else if(m_bEtcMode)
				{
					((CTile*)m_vecTile[i])->GetTileObj()->SetIsDelete();
					((CTile*)m_vecTile[i])->SetTileObj(NULL);
					((CTile*)m_vecTile[i])->SetObjCnt(0);
					m_pEtcTool->DeleteEtcData(((CTile*)m_vecTile[i])->GetInfo()->vPos);
				}
			}
		}
	}
}

void CTileMap::Render()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), RenderObj());
	m_vecTile.clear();
}

void CTileMap::Release()
{
	for_each(m_vecFullTile.begin(), m_vecFullTile.end(), ::CDeleteObj());
	vector<CObj*>().swap(m_vecFullTile);

	for_each(m_vecTileInfo.begin(), m_vecTileInfo.end(), ::CDeleteObj());
	vector<TILE*>().swap(m_vecTileInfo);

	vector<CObj*>().swap(m_vecTile);
}

const int CTileMap::GetTileX() const
{
	return m_iTileX;
}

const int CTileMap::GetTileZ() const
{
	return m_iTileZ;
}

const vector<TILE*>* CTileMap::GetTileInfo() const
{
	return &m_vecTileInfo;
}

void CTileMap::SetTileInfo(const vector<TILE*>& vecTile)
{
	for_each(m_vecTileInfo.begin(), m_vecTileInfo.end(), ::CDeleteObj());
	vector<TILE*>().swap(m_vecTileInfo);

	m_vecTileInfo = vecTile;

	size_t iSize = m_vecTileInfo.size();

	for(size_t i = 0; i < iSize; ++i)
	{
		((CTile*)m_vecFullTile[i])->SetDrawId(m_vecTileInfo[i]->iDrawId);
		((CTile*)m_vecFullTile[i])->SetCubeCnt(m_vecTileInfo[i]->iCubeCnt);
		((CTile*)m_vecFullTile[i])->SetObjCnt(m_vecTileInfo[i]->iObjCnt);
		((CTile*)m_vecFullTile[i])->SetCubeCnt(m_vecTileInfo[i]->iCubeCnt);
		((CTile*)m_vecFullTile[i])->SetObjCnt(m_vecTileInfo[i]->iObjCnt);
	}
}

void CTileMap::SetDrawId(const int iDrawId)
{
	m_iDrawId = iDrawId;
}

void CTileMap::SetEtcMode(const bool bEtcMode)
{
	m_bEtcMode = bEtcMode;
}

void CTileMap::CreateNewMap(const int iTileX, const int iTileZ, const int iDrawId)
{
	SetResetVtx();

	Release();

	m_iTileX = iTileX;
	m_iTileZ = iTileZ;

	for(int i = 0; i < m_iTileX; ++i)
	{
		for(int j = 0; j < m_iTileZ; ++j)
		{
			CObj* pObj = CObjFactory<CTile>::CreateObj(D3DXVECTOR3(float(i * VTXITV * 2), 0.f, float(j * VTXITV * 2)));

			TILE* pTile = new TILE;

			pTile->vPos = pObj->GetInfo()->vPos;
			((CTile*)pObj)->SetDrawId(iDrawId);
			pTile->iOption = ((CTile*)pObj)->GetOption();
			pTile->iDrawId = ((CTile*)pObj)->GetDrawId();
			pTile->iCubeCnt = ((CTile*)pObj)->GetCubeCnt();
			pTile->iObjCnt = ((CTile*)pObj)->GetObjCnt();

			m_vecFullTile.push_back(pObj);
			m_vecTileInfo.push_back(pTile);
		}
	}
}

void CTileMap::SetCubeMode(bool _bCubeMode)
{
	m_bCubeMode = _bCubeMode;
}

void CTileMap::SetObjMode(bool _bObjMode)
{
	m_bObjMode = _bObjMode;
}

void CTileMap::SetItemMode(const bool bItemMode)
{
	m_bItemMode = bItemMode;
}

void CTileMap::InfoReset()
{
	size_t iSize = m_vecTileInfo.size();

	for(size_t i = 0; i < iSize; ++i)
	{
		m_vecTileInfo[i]->iOption = ((CTile*)m_vecFullTile[i])->GetOption();
		m_vecTileInfo[i]->iDrawId = ((CTile*)m_vecFullTile[i])->GetDrawId();
		m_vecTileInfo[i]->iCubeCnt = ((CTile*)m_vecFullTile[i])->GetCubeCnt();
		m_vecTileInfo[i]->iObjCnt = ((CTile*)m_vecFullTile[i])->GetObjCnt();
	}
}

void CTileMap::Twinkle()
{
	m_fTime += m_pTimeMgr->GetTime();

	if(m_fTime >= 1.f)
		m_fTime = 0.f;

	int iTemp = 1;

	size_t iSize = m_vecFullTile.size();

	if(!m_bTwinkle)
	{
		for(size_t i = 0; i < iSize; ++i)
		{
			((CTile*)m_vecFullTile[i])->SetStateKey(TEXT("Tile"));
		}

		return;
	}

	for(size_t i = 0; i < iSize; ++i)
	{
		if(i % m_iTileZ == 0)
			iTemp *= -1;

		if(m_fTime <= 0.5f)
		{
			if((i & 1) == 0)
			{
				if(iTemp == 1)
					((CTile*)m_vecFullTile[i])->SetStateKey(TEXT("TileA"));
				if(iTemp == -1)
					((CTile*)m_vecFullTile[i])->SetStateKey(TEXT("Tile"));
			}
			else
			{
				if(iTemp == 1)
					((CTile*)m_vecFullTile[i])->SetStateKey(TEXT("Tile"));
				if(iTemp == -1)
					((CTile*)m_vecFullTile[i])->SetStateKey(TEXT("TileA"));
			}
		}
		else if(m_fTime >= 0.5f)
		{
			if((i & 1) == 0)
			{
				if(iTemp == 1)
					((CTile*)m_vecFullTile[i])->SetStateKey(TEXT("Tile"));
				if(iTemp == -1)
					((CTile*)m_vecFullTile[i])->SetStateKey(TEXT("TileB"));
			}
			else
			{
				if(iTemp == 1)
					((CTile*)m_vecFullTile[i])->SetStateKey(TEXT("TileB"));
				if(iTemp == -1)
					((CTile*)m_vecFullTile[i])->SetStateKey(TEXT("Tile"));
			}
		}
	}
}

void CTileMap::SetResetVtx()
{
	m_pBufferMgr->SetVtxInfo(TEXT("RcTex"), m_pVtxTex);
	m_pBufferMgr->SetVtxInfo(TEXT("UnitTex"), m_pVtxTex2);
}

vector<CObj*> CTileMap::GetFullTile() const
{
	return m_vecFullTile;
}