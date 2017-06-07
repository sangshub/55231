#include "stdafx.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "Player.h"
#include "Device.h"
#include "BufferMgr.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "ParticleMgr.h"
#include "Frustum.h"
#include "BeatMgr.h"
#include "ObjFactory.h"
#include "ComboMgr.h"
#include "Monster.h"
#include "Tile.h"
#include "Rand.h"
#include "Item.h"
#include "MathMgr.h"
#include "Stage.h"

IMPLEMENT_SINGLETON(CObjMgr)

struct ObjRender
{
	void operator()(CObj* pObj)
	{
		pObj->Render();
	}
};

struct TileStateKeyTile
{
	void operator()(CObj* pObj)
	{
		pObj->SetStateKey(TEXT("Tile"));
	}
};

CObjMgr::CObjMgr()
: m_bTwinkle(false)
, m_bLight(false)
, m_bStageEnd(false)
, m_bStageStart(false)
, m_bActionCam(false)
, m_bDarkness(false)
, m_bDeadCheck(false)
, m_bJump(false)
, m_iTileX(0)
, m_iTileZ(0)
, m_iTorchDis(0)
, m_fLightDis(0.f)
, m_fTime(0.f)
, m_pOriRcTex(NULL)
, m_pOriUIVtx(NULL)
, m_pOriRcCol(NULL)
, m_pOriCubeTex(NULL)
, m_pDevice(CDevice::GetInstance())
, m_pBufferMgr(CBufferMgr::GetInstance())
, m_pTimeMgr(CTimeMgr::GetInstance())
, m_pFrustum(CFrustum::GetInstance())
, m_pBeatMgr(CBeatMgr::GetInstance())
, m_pParticleMgr(CParticleMgr::GetInstance())
, m_pSceneMgr(CSceneMgr::GetInstance())
{
}

CObjMgr::~CObjMgr()
{
	Release();
}

HRESULT CObjMgr::Initialize()
{
	m_pOriRcCol = new VTXCOL[4];
	m_pOriRcTex = new VTXTEX[4];
	m_pOriUIVtx = new VTXTEX[4];
	m_pOriCubeTex = new VTXCUBE[8];

	m_pBufferMgr->GetVtxInfo(TEXT("RcCol"), m_pOriRcCol);
	m_pBufferMgr->GetVtxInfo(TEXT("RcTex"), m_pOriRcTex);
	m_pBufferMgr->GetVtxInfo(TEXT("UITex"), m_pOriUIVtx);
	m_pBufferMgr->GetVtxInfo(TEXT("CubeTex"), m_pOriCubeTex);

	m_vecFullTile.reserve(1000);

	return S_OK;
}

void CObjMgr::Progress()
{
	if(!m_bStageEnd)
		CubeCulling();

	TileCulling();

	m_bDeadCheck = false;

	Twinkle();

	if(GetAsyncKeyState('P') & 0x0001)
	{
		if(m_bTwinkle)
			m_bTwinkle = false;
		else
			m_bTwinkle = true;
	}

	int iResult = 0;

	for(int i = 0; i < OBJ_END; ++i)
	{
		if((!m_bStageEnd && !m_bStageStart) && (i == OBJ_TERRAIN || i == OBJ_CUBE))
			continue;

		for(list<CObj*>::iterator iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end();)
		{
			SetObjectStageEnd(*iter);

			if(i == OBJ_PLAYER)
				m_bJump = false;

			iResult = (*iter)->Progress();

			if(m_bJump)
				(*iter)->SetMegaJump(true);

			if(iResult == 1)
			{
				::Safe_Delete(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}

	if(m_ObjList[OBJ_CUBE].empty())
	{
		m_bStageEnd = false;
		m_pSceneMgr->SetNextScene();
	}

	if(m_bStageStart && !m_bActionCam
		&& m_pBeatMgr->GetBeatBoomBoom()) // 빛거리 계산
	{
		m_fLightDis += 0.2f;
		if(m_fLightDis > 1.f)
		{
			m_fLightDis = 1.f;
			m_bStageStart = false;
		}
	}
 	//OBB tItem =m_ObjList[OBJ_ITEM].front()->GetObjOBB();
	OBB tPlayer = GetPlayer()->GetObjOBB();

	for(list<CObj*>::iterator iter = m_ObjList[OBJ_ITEM].begin();
		iter != m_ObjList[OBJ_ITEM].end(); iter++)
	{
		if( CheckOBBCollision(&tPlayer , &(*iter)->GetObjOBB()))
			int i = 0;
	}


	/*if( CheckOBBCollision(&tPlayer , &tItem ) )
		int u = 1;*/
}

void CObjMgr::Render()
{
	if(!m_bStageEnd && !m_bStageStart)
		for_each(m_CubeList.begin(), m_CubeList.end(), ObjRender());
	for_each(m_TileList.begin(), m_TileList.end(), ObjRender());

	for(int i = 0; i < OBJ_END; ++i)
	{
		if((!m_bStageEnd && !m_bStageStart && (i == OBJ_TERRAIN || i == OBJ_CUBE)) || i == OBJ_UI)
			continue;

		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), ObjRender());
	}

	CObj* pHeart = NULL;
	for (list<CObj*>::iterator iter = m_ObjList[OBJ_UI].begin();
		iter != m_ObjList[OBJ_UI].end(); ++iter)
	{
		if((*iter)->GetObjkey() == TEXT("Heart"))
		{
			pHeart = (*iter);
			continue;
		}
		(*iter)->Render();
	}
	pHeart->Render();

	m_TileList.clear();
	m_CubeList.clear();
}

void CObjMgr::Release()
{
	for(int i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), ::CDeleteObj());
		m_ObjList[i].clear();
	}

	for_each(m_vecFullTile.begin(), m_vecFullTile.end(), ::CDeleteObj());

	m_TileList.clear();
	m_CubeList.clear();
	vector<CObj*>().swap(m_vecFullTile);

	::Safe_Destroy(m_pFrustum);

	::Safe_Delete_Array(m_pOriRcCol);
	::Safe_Delete_Array(m_pOriRcTex);
	::Safe_Delete_Array(m_pOriUIVtx);
	::Safe_Delete_Array(m_pOriCubeTex);
}

void CObjMgr::AddObj(const OBJID& eObjId, CObj* pObj)
{
	m_ObjList[eObjId].push_back(pObj);
}

CObj* CObjMgr::GetObj(const OBJID& eObjId)
{
	return m_ObjList[eObjId].front();
}

CObj* CObjMgr::GetPlayer()
{
	return m_ObjList[OBJ_PLAYER].front();
}

CTile* CObjMgr::GetTile(const int iIndex)
{
	return (CTile*)m_vecFullTile[iIndex];
}

const int CObjMgr::GetTileX() const
{
	return m_iTileX;
}

const int CObjMgr::GetTileZ() const
{
	return m_iTileZ;
}

const int CObjMgr::GetIndex(const D3DXVECTOR3& vPos) const
{
	D3DXVECTOR3 vTemp = vPos + D3DXVECTOR3(0.979f, 0.f, 0.979f);

	int iIndex = ((int)vTemp.x / TILESIZE) * m_iTileZ + (int)vTemp.z / TILESIZE;

	if(iIndex < 0 || iIndex > m_iTileX * m_iTileZ - 1)
		return 0;

	return iIndex;
}

vector<CObj*> CObjMgr::GetFullTile() const
{
	return m_vecFullTile;
}

void CObjMgr::SetTile(CObj* pObj)
{
	m_vecFullTile.push_back(pObj);
}

void CObjMgr::SetTileSize(const int iTileX, const int iTileZ)
{
	m_iTileX = iTileX;
	m_iTileZ = iTileZ;
}

void CObjMgr::SetTwinkle(const bool bTwinkle)
{
	m_bTwinkle = bTwinkle;
}

void CObjMgr::TileCulling()
{
	int iCullX = int(m_pFrustum->GetMinX())>>1;
	int iCullZ = int(m_pFrustum->GetMinZ())>>1;

	size_t iSize = m_vecFullTile.size();

	if(!m_bActionCam)
	{
		for(int i = 0; i < int(m_pFrustum->GetMaxZ())>>1; ++i)
		{
			for(int j = 0; j < int(m_pFrustum->GetMaxX())>>1; ++j)
			{
				int iIndex = (j + iCullX) * m_iTileZ + (i + iCullZ);

				if(iIndex < 0 || iIndex >= m_iTileX * m_iTileZ)
					break;

				if(m_pFrustum->FrustumCulling(m_vecFullTile[iIndex]->GetInfo()->vPos))
				{
					m_vecFullTile[iIndex]->Progress();
					m_TileList.push_back(m_vecFullTile[iIndex]);
					SetObjectStageEnd(m_vecFullTile[iIndex]);
				}
			}
		}
	}
	else if(m_bActionCam)
	{
		for(size_t i = 0; i < m_vecFullTile.size(); ++i)
		{
			m_vecFullTile[i]->Progress();
			m_TileList.push_back(m_vecFullTile[i]);
		}
	}
	
	if(!((CStage*)m_pSceneMgr->GetSceneObj())->GetBossStage()
		&& !m_bActionCam)
		TileLightOfSight();
}

void CObjMgr::CubeCulling()
{
	int iResult = 0;

	for(list<CObj*>::iterator iter = m_ObjList[OBJ_CUBE].begin();
		iter != m_ObjList[OBJ_CUBE].end();)
	{
		if(m_pFrustum->FrustumCulling((*iter)->GetInfo()->vPos))
		{
			iResult = (*iter)->Progress();
			if(m_bJump)
				(*iter)->SetMegaJump(true);

			if(iResult == 1)
			{
				::Safe_Delete(*iter);
				iter = m_ObjList[OBJ_CUBE].erase(iter);
			}
			else
			{
				m_CubeList.push_back(*iter);
				++iter;
			}
		}
		else
			++iter;
	}
}

list<CObj*>* CObjMgr::GetObjList(const OBJID& eObjid)
{
	return &m_ObjList[eObjid];
}

void CObjMgr::Twinkle()
{
	m_fTime += m_pTimeMgr->GetTime() / m_pBeatMgr->GetBeatMax();

	if(m_fTime >= 2.f)
		m_fTime = 0.f;

	int iTemp = 1;

	if(!m_bTwinkle)
	{
		for_each(m_vecFullTile.begin(), m_vecFullTile.end(), TileStateKeyTile());

		return;
	}

	size_t iSize = m_vecFullTile.size();

	for(size_t i = 0; i < iSize; ++i)
	{
		if(i % m_iTileZ == 0)
			iTemp *= -1;

		if(m_fTime <= 1.f)
		{
			if((i & 1) == 0)
			{
				if(iTemp == 1)
					m_vecFullTile[i]->SetStateKey(TEXT("TileA"));
				if(iTemp == -1)
					m_vecFullTile[i]->SetStateKey(TEXT("Tile"));
			}
			else
			{
				if(iTemp == 1)
					m_vecFullTile[i]->SetStateKey(TEXT("Tile"));
				if(iTemp == -1)
					m_vecFullTile[i]->SetStateKey(TEXT("TileA"));
			}
		}
		else if(m_fTime >= 1.f)
		{
			if((i & 1) == 0)
			{
				if(iTemp == 1)
					m_vecFullTile[i]->SetStateKey(TEXT("Tile"));
				if(iTemp == -1)
					m_vecFullTile[i]->SetStateKey(TEXT("TileB"));
			}
			else
			{
				if(iTemp == 1)
					m_vecFullTile[i]->SetStateKey(TEXT("TileB"));
				if(iTemp == -1)
					m_vecFullTile[i]->SetStateKey(TEXT("Tile"));
			}
		}
	}
}

void CObjMgr::ResetRcCol()
{
	m_pBufferMgr->SetVtxInfo(TEXT("RcCol"), m_pOriRcCol);
}

void CObjMgr::ResetRcTex()
{
	m_pBufferMgr->SetVtxInfo(TEXT("RcTex"), m_pOriRcTex);
}

void CObjMgr::ResetUIVtx()
{
	m_pBufferMgr->SetVtxInfo(TEXT("UITex"), m_pOriUIVtx);
}

void CObjMgr::ResetCubeTex()
{
	m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pOriCubeTex);
}

const bool CObjMgr::GetDeadCheck() const
{
	return m_bDeadCheck;
}

void CObjMgr::GoldDrop(D3DXVECTOR3 vPos, const int iCnt)
{
	int iIndex = GetIndex(vPos);
	CItem* pItem = GetTile(iIndex)->GetItem();

	if(!pItem)
	{
		ResetUIVtx();

		CItem* pGold = (CItem*)CObjFactory<CItem>::CreateObj();
		vPos.y = 0.5f;
		pGold->SetPos(vPos);
		pGold->SetType(ITEM_GOLD);
		pGold->SetScale(0.8f);


		TCHAR szGoldNum[MIN_STR];
		int iNum = 0;
		if(iCnt)
			iNum = iCnt;
		else
			iNum = CRand::Random(1, 7);

		wsprintf(szGoldNum, TEXT("Coin%d"), iNum);
		pGold->SetObjKey(szGoldNum);
		pGold->SetPrice(iNum);

		AddObj(OBJ_ITEM, pGold);
		GetTile(iIndex)->SetItem(pGold);
	}
	else if(pItem->GetType() == ITEM_GOLD)
	{
		int iNum = 0;
		if(iCnt)
			iNum = iCnt;
		else
			iNum = CRand::Random(1, 7);

		int iGold = pItem->GetPrice() + iNum;

		TCHAR szGoldNum[MIN_STR];

		if(iGold < 11)
			wsprintf(szGoldNum, TEXT("Coin%d"), iGold);
		else
			wsprintf(szGoldNum, TEXT("Coin%d"), 20);

		pItem->SetObjKey(szGoldNum);
		pItem->SetPrice(iGold);
	}
}

void CObjMgr::ChangeStage()
{
	for(int i = 0; i < OBJ_END; ++i)
	{
		if(i == OBJ_UI)
		{
			for(list<CObj*>::iterator iter = m_ObjList[OBJ_UI].begin();
				iter != m_ObjList[OBJ_UI].end();)
			{
				if((*iter)->GetObjkey() == TEXT("MiniMap"))
				{
					::Safe_Delete(*iter);
					iter = m_ObjList[OBJ_UI].erase(iter);
				}
				else
					++iter;
			}
		}

		if(i == OBJ_PLAYER || i == OBJ_UI)
			continue;

		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), ::CDeleteObj());
		m_ObjList[i].clear();
	}

	for_each(m_vecFullTile.begin(), m_vecFullTile.end(), ::CDeleteObj());

	m_vecFullTile.clear();
	m_TileList.clear();
	m_CubeList.clear();

	ResetRcTex();
	ResetUIVtx();
	ResetCubeTex();
}

const bool CObjMgr::GetIsLight() const
{
	return m_bLight;
}

void CObjMgr::SetIsLight(const bool bLight)
{
	m_bLight = bLight;
}

void CObjMgr::SetJump(const bool bJump)
{
	m_bJump = bJump;
}

void CObjMgr::SetStageEnd(const bool bStageEnd)
{
	m_bStageEnd = bStageEnd;
	m_pSceneMgr->StopBgm();
}

void CObjMgr::SetStageStart(const bool bStageStart)
{
	m_fLightDis = 0;
	m_bStageStart = bStageStart;
}

void CObjMgr::SetObjectStageEnd(CObj* pObj)
{
	if(m_bStageEnd && CRand::Random(0,40) == 0)
		pObj->SetStageEnd(true);
}

void CObjMgr::TileLightOfSight()
{
	D3DXVECTOR3 vOriRay = D3DXVECTOR3(0.f, 0.f, 5.f*m_fLightDis + m_iTorchDis);
	D3DXVECTOR3 vRay = vOriRay;
	D3DXVECTOR3 vPlayerPos = m_ObjList[OBJ_PLAYER].front()->GetInfo()->vPos;
	vPlayerPos.y = 0.f;
	float fAngle = 0.f;
	float fFlagAngle = 45.f;
	float fM = 0.f; // 기울기
	float fPlus = 1.f;
	int iCamAngle = (int)D3DXToDegree(((CPlayer*)m_ObjList[OBJ_PLAYER].front())->GetCamAngleY());
	int iPlayerAngle = (int)D3DXToDegree(m_ObjList[OBJ_PLAYER].front()->GetAngleY());
	int iPlayerDir = int((iCamAngle+iPlayerAngle)/fFlagAngle);
	int	iCnt = 0;

	if(iPlayerDir < 0)
		iPlayerDir  = 8 + iPlayerDir;
	else if(iPlayerDir > 7)
		iPlayerDir  = iPlayerDir - 8;

	for(int i = 0; i < 72; ++i)
	{
		if(((iPlayerDir == 0 || iPlayerDir == 7) && (fAngle < fFlagAngle*1.f || fAngle > fFlagAngle*7.f)) || 
			((iPlayerDir == 4 || iPlayerDir == 3) && (fAngle > fFlagAngle*3.f && fAngle < fFlagAngle*5.f)) || 
			((iPlayerDir == 2 || iPlayerDir == 1) && (fAngle > fFlagAngle*1.f && fAngle < fFlagAngle*3.f)) || 
			((iPlayerDir == 6 || iPlayerDir == 5) && (fAngle > fFlagAngle*5.f && fAngle < fFlagAngle*7.f)))
			CMathMgr::MyRotationY(vRay, vOriRay+D3DXVECTOR3(0.f, 0.f, 4.f), D3DXToRadian(fAngle));
		else
			CMathMgr::MyRotationY(vRay, vOriRay, D3DXToRadian(fAngle));

		fM = vRay.z/vRay.x;

		if((fAngle < fFlagAngle || fAngle >= fFlagAngle*7.f) || 
			(fAngle >= fFlagAngle*3.f && fAngle < fFlagAngle*5.f))
		{
			if(vRay.z < 0.f)
				fPlus = -1;
			else
				fPlus = 1;

			for(int j = 0; j <= vRay.z*fPlus; j+=2)
			{
				float fX = j/fM*fPlus;

				CTile* pTile = GetTile(GetIndex(vPlayerPos+D3DXVECTOR3(fX, 0.f, j*fPlus)));
				pTile->SetFindWay(true);

				if(!pTile->GetObj() || 
					(pTile->GetTileObjId() != OBJ_CUBE && 
					pTile->GetTileObjKey() != TEXT("Door")))
				{
					if(j+2 > vRay.z*fPlus)
						pTile->SetDarkness(180);
					else
						pTile->SetDarkness(255);
				}
				else if(pTile->GetTileObjId() == OBJ_CUBE || 
					pTile->GetTileObjKey() == TEXT("Door"))
				{
					pTile->SetDarkness(255);

					if(m_iTorchDis)
					{
						j += 2;
						fX = j/fM*fPlus;
						pTile = GetTile(GetIndex(vPlayerPos+D3DXVECTOR3(fX, 0.f, j*fPlus)));
						pTile->SetFindWay(true);
						pTile->SetDarkness(100);
					}
						
					break;
				}
			}
		}
		else if((fAngle >= fFlagAngle && fAngle < fFlagAngle*3.f)
			|| (fAngle >= fFlagAngle*5.f && fAngle < fFlagAngle*7.f))
		{
			if(vRay.x < 0.f)
				fPlus = -1;
			else
				fPlus = 1;

			for(int j = 0; j <= vRay.x*fPlus; j+=2)
			{
				float fZ = fM*j*fPlus;

				CTile* pTile = GetTile(GetIndex(vPlayerPos+D3DXVECTOR3(j*fPlus, 0.f, fZ)));
				pTile->SetFindWay(true);

				if(!pTile->GetObj() || 
					(pTile->GetTileObjId() != OBJ_CUBE && 
					pTile->GetTileObjKey() != TEXT("Door")))
				{
					if(j+2 > vRay.x*fPlus)
						pTile->SetDarkness(180);
					else
						pTile->SetDarkness(255);
				}
				else if(pTile->GetTileObjId() == OBJ_CUBE || 
					pTile->GetTileObjKey() == TEXT("Door"))
				{
					pTile->SetDarkness(255);

					if(m_iTorchDis)
					{
						j += 2;
						fZ = fM*j*fPlus;
						pTile = GetTile(GetIndex(vPlayerPos+D3DXVECTOR3(j*fPlus, 0.f, fZ)));
						pTile->SetFindWay(true);
						pTile->SetDarkness(100);
					}

					break;
				}
			}
		}

		fAngle += 5.f;
	}
}

void CObjMgr::SetTorchDistance(const int iTorchDis)
{
	m_iTorchDis = iTorchDis;
}

bool CObjMgr::CheckOBBCollision(OBB* Box1, OBB* Box2)
{
	double c[3][3];
	double absC[3][3];
	double d[3];
	double r0, r1, r;
	int i;
	const double cutoff = 0.999999;
	bool existsParallelPair = false;
	D3DXVECTOR3 diff = Box1->vCenterPos - Box2->vCenterPos;

	for( i = 0 ; i < 3 ; ++i )
	{
		c[0][i] = D3DXVec3Dot( &Box1->vAxisDir[0], &Box2->vAxisDir[i] );
		absC[0][i] = abs( c[0][i] );
		if( absC[0][i] > cutoff )
			existsParallelPair = true;
	}
	d[0] = D3DXVec3Dot( &diff, &Box1->vAxisDir[0] );
	r = abs( d[0] );
	r0 = Box1->fAxisLen[0];
	r1 = Box2->fAxisLen[0] * absC[0][0] + Box2->fAxisLen[1] * absC[0][1] + Box2->fAxisLen[2] * absC[0][2];
	if( r > r0 + r1 )
		return FALSE;

	for( i = 0 ; i < 3 ; ++i )
	{
		c[1][i] = D3DXVec3Dot( &Box1->vAxisDir[1], &Box2->vAxisDir[i] );
		absC[1][i] = abs( c[1][i] );
		if( absC[1][i] > cutoff )
			existsParallelPair = true;
	}
	d[1] = D3DXVec3Dot( &diff, &Box1->vAxisDir[1] );
	r = abs( d[1] );
	r0 = Box1->fAxisLen[1];
	r1 = Box2->fAxisLen[0] * absC[1][0] + Box2->fAxisLen[1] * absC[1][1] + Box2->fAxisLen[2] * absC[1][2];
	if( r > r0 + r1 )
		return FALSE;

	for( i = 0 ; i < 3 ; ++i )
	{
		c[2][i] = D3DXVec3Dot( &Box1->vAxisDir[2], &Box2->vAxisDir[i] );
		absC[2][i] = abs( c[2][i] );
		if( absC[2][i] > cutoff )
			existsParallelPair = true;
	}
	d[2] = D3DXVec3Dot( &diff, &Box1->vAxisDir[2] );
	r = abs( d[2] );
	r0 = Box1->fAxisLen[2];
	r1 = Box2->fAxisLen[0] * absC[2][0] + Box2->fAxisLen[1] * absC[2][1] + Box2->fAxisLen[2] * absC[2][2];
	if( r > r0 + r1 )
		return FALSE;

	r = abs( D3DXVec3Dot( &diff, &Box2->vAxisDir[0] ) );
	r0 = Box1->fAxisLen[0] * absC[0][0] + Box1->fAxisLen[1] * absC[1][0] + Box1->fAxisLen[2] * absC[2][0];
	r1 = Box2->fAxisLen[0];
	if( r > r0 + r1 )
		return FALSE;

	r = abs( D3DXVec3Dot( &diff, &Box2->vAxisDir[1] ) );
	r0 = Box1->fAxisLen[0] * absC[0][1] + Box1->fAxisLen[1] * absC[1][1] + Box1->fAxisLen[2] * absC[2][1];
	r1 = Box2->fAxisLen[1];
	if( r > r0 + r1 )
		return FALSE;

	r = abs( D3DXVec3Dot( &diff, &Box2->vAxisDir[2] ) );
	r0 = Box1->fAxisLen[0] * absC[0][2] + Box1->fAxisLen[1] * absC[1][2] + Box1->fAxisLen[2] * absC[2][2];
	r1 = Box2->fAxisLen[2];
	if( r > r0 + r1 )
		return FALSE;

	if( existsParallelPair == true )
		return TRUE;

	r = abs( d[2]*c[1][0] - d[1]*c[2][0] );
	r0 = Box1->fAxisLen[1] * absC[2][0] + Box1->fAxisLen[2] * absC[1][0];
	r1 = Box2->fAxisLen[1] * absC[0][2] + Box2->fAxisLen[2] * absC[0][1];
	if( r > r0 + r1 )
		return FALSE;

	r = abs( d[2]*c[1][1] - d[1]*c[2][1] );
	r0 = Box1->fAxisLen[1] * absC[2][1] + Box1->fAxisLen[2] * absC[1][1];
	r1 = Box2->fAxisLen[0] * absC[0][2] + Box2->fAxisLen[2] * absC[0][0];
	if( r > r0 + r1 )
		return FALSE;

	r = abs( d[2]*c[1][2] - d[1]*c[2][2] );
	r0 = Box1->fAxisLen[1] * absC[2][2] + Box1->fAxisLen[2] * absC[1][2];
	r1 = Box2->fAxisLen[0] * absC[0][1] + Box2->fAxisLen[1] * absC[0][0];
	if( r > r0 + r1 )
		return FALSE;

	r = abs( d[0]*c[2][0] - d[2]*c[0][0] );
	r0 = Box1->fAxisLen[0] * absC[2][0] + Box1->fAxisLen[2] * absC[0][0];
	r1 = Box2->fAxisLen[1] * absC[1][2] + Box2->fAxisLen[2] * absC[1][1];
	if( r > r0 + r1 )
		return FALSE;

	r = abs( d[0]*c[2][1] - d[2]*c[0][1] );
	r0 = Box1->fAxisLen[0] * absC[2][1] + Box1->fAxisLen[2] * absC[0][1];
	r1 = Box2->fAxisLen[0] * absC[1][2] + Box2->fAxisLen[2] * absC[1][0];
	if( r > r0 + r1 )
		return FALSE;

	r = abs( d[0]*c[2][2] - d[2]*c[0][2] );
	r0 = Box1->fAxisLen[0] * absC[2][2] + Box1->fAxisLen[2] * absC[0][2];
	r1 = Box2->fAxisLen[0] * absC[1][1] + Box2->fAxisLen[1] * absC[1][0];
	if( r > r0 + r1 )
		return FALSE;

	r = abs( d[1]*c[0][0] - d[0]*c[1][0] );
	r0 = Box1->fAxisLen[0] * absC[1][0] + Box1->fAxisLen[1] * absC[0][0];
	r1 = Box2->fAxisLen[1] * absC[2][2] + Box2->fAxisLen[2] * absC[2][1];
	if( r > r0 + r1 )
		return FALSE;

	r = abs( d[1]*c[0][1] - d[0]*c[1][1] );
	r0 = Box1->fAxisLen[0] * absC[1][1] + Box1->fAxisLen[1] * absC[0][1];
	r1 = Box2->fAxisLen[0] * absC[2][2] + Box2->fAxisLen[2] * absC[2][0];
	if( r > r0 + r1 )
		return FALSE;

	r = abs( d[1]*c[0][2] - d[0]*c[1][2] );
	r0 = Box1->fAxisLen[0] * absC[1][2] + Box1->fAxisLen[1] * absC[0][2];
	r1 = Box2->fAxisLen[0] * absC[2][1] + Box2->fAxisLen[1] * absC[2][0];
	if( r > r0 + r1 )
		return FALSE;

	return TRUE;

}

void CObjMgr::SetActionCam(const bool bActionCam)
{
	m_bActionCam = bActionCam;
}

const bool CObjMgr::GetActionCamSwitch() const
{
	return m_bActionCam;
}

void CObjMgr::SetDarknessSwitch(const bool bDarkness)
{
	m_bDarkness = bDarkness;
}

const bool CObjMgr::GetDarknessSwitch() const
{
	return m_bDarkness;
}