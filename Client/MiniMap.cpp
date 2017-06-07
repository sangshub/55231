#include "StdAfx.h"
#include "MiniMap.h"
#include "ObjMgr.h"
#include "Tile.h"
#include "BeatMgr.h"
#include "Rand.h"

CMiniMap::CMiniMap(void)
:m_pObjMgr(CObjMgr::GetInstance())
,m_iRnd(0)
,m_bOnOff(1)
{
}

CMiniMap::~CMiniMap(void)
{
}

HRESULT CMiniMap::Initialize(void)
{
	m_wstrObjKey = TEXT("MiniMap");

	m_vecTile = m_pObjMgr->GetFullTile();
	m_pCubeList = m_pObjMgr->GetObjList(OBJ_CUBE);
	m_pItemList = m_pObjMgr->GetObjList(OBJ_ITEM);
	m_pMonsterList = m_pObjMgr->GetObjList(OBJ_MONSTER);
	m_pEtcList = m_pObjMgr->GetObjList(OBJ_ETC);
	m_pTrapList = m_pObjMgr->GetObjList(OBJ_TRAP);
	m_pPlayer  = m_pObjMgr->GetPlayer();
	
	
	m_dwREF[0] = D3DCOLOR_XRGB(110, 110, 110);
	m_dwREF[1] = D3DCOLOR_XRGB(255, 132, 0);
	m_dwREF[2] = D3DCOLOR_XRGB(203, 31, 213);
	m_dwREF[3] = D3DCOLOR_XRGB(202, 191, 169);
	m_dwREF[4] = D3DCOLOR_XRGB(225, 37, 0);
	m_dwREF[5] = D3DCOLOR_XRGB(255, 213, 0);
	m_dwREF[6] = D3DCOLOR_XRGB(197, 151, 0);
	m_dwREF[7] = D3DCOLOR_XRGB(155, 113, 40);
	m_dwREF[8] = D3DCOLOR_XRGB(122, 78, 37);
	m_dwREF[9] = D3DCOLOR_XRGB(0, 255, 238);
	m_dwREF[10] = D3DCOLOR_XRGB(199, 216, 255);
	m_dwREF[11] = D3DCOLOR_XRGB(255, 0, 0);
	m_dwREF[12] = D3DCOLOR_XRGB(69, 147, 97);
	m_dwREF[13] = D3DCOLOR_XRGB(0, 0, 255);

	m_fMapSizeX = (float)m_pObjMgr->GetTileX();
	m_fMapSizeY = (float)m_pObjMgr->GetTileZ();

	m_tInfo.vPos.x = WINCX - m_fMapSizeX*5-10;
	m_tInfo.vPos.y = 700;


	
	return S_OK;
}

const int CMiniMap::Progress(void)
{
	
	if(m_bIsDelete)
		return 1;


	return 0;
}

void CMiniMap::Render(void)
{
	if(GetAsyncKeyState('Q') & 0x0001)
	{
		if(m_bOnOff == true)
			m_bOnOff = false;
		else
			m_bOnOff = true;
	}


	if(m_pObjMgr->GetActionCamSwitch())
		return;
	unsigned int iSize = m_vecTile.size();
	for(size_t i = 0; i < iSize; i++)
	{
		if(m_bOnOff)
		{
			if (m_pObjMgr->GetTile(i)->GetDrawId() == 16 || m_pObjMgr->GetTile(i)->GetDarkness() == 0 )
				continue;
		}
		DrawPoint2(3, m_vecTile[i]->GetInfo()->vPos.x, m_vecTile[i]->GetInfo()->vPos.z );
	}//타일그리기

	for(list<CObj*>::iterator iter = m_pCubeList->begin(); iter != m_pCubeList->end(); iter++)
	{
		if(m_bOnOff)
		{
		if((*iter)->GetDarkness() == 0)
			continue;
		}

		if( (*iter)->GetCubeInfo().dwType & CUBE_BROK )
			DrawPoint2(7, (*iter)->GetInfo()->vPos.x, (*iter)->GetInfo()->vPos.z);
		else if( (*iter)->GetCubeInfo().dwType & CUBE_NOTB && (*iter)->GetCubeInfo().dwType & CUBE_EVEN1 )
			DrawPoint2(8, (*iter)->GetInfo()->vPos.x, (*iter)->GetInfo()->vPos.z);
		else if( (*iter)->GetCubeInfo().dwType & CUBE_NOTB && !((*iter)->GetCubeInfo().dwType & CUBE_EVEN1) )
			DrawPoint2(0, (*iter)->GetInfo()->vPos.x, (*iter)->GetInfo()->vPos.z);
		else if( (*iter)->GetCubeInfo().dwType & CUBE_BOOM )
			DrawPoint2(6, (*iter)->GetInfo()->vPos.x, (*iter)->GetInfo()->vPos.z);

	}//큐브 그리기

	for(list<CObj*>::iterator iter = m_pMonsterList->begin(); iter != m_pMonsterList->end(); iter++)
	{
		if(m_bOnOff)
		{
		if((*iter)->GetDarkness() == 0)
			continue;
		}
		DrawPoint2(4,(*iter)->GetInfo()->vPos.x, (*iter)->GetInfo()->vPos.z);
	}//몬스터 그리기

	for(list<CObj*>::iterator iter = m_pTrapList->begin(); iter != m_pTrapList->end(); iter++)
	{
		if((*iter)->GetObjkey() == TEXT("Stairs"))
			DrawPoint2(2, (*iter)->GetInfo()->vPos.x, (*iter)->GetInfo()->vPos.z);
		if(m_bOnOff)
		{	
		if((*iter)->GetDarkness() == 0 )
			continue;
		}

		DrawPoint2(10, (*iter)->GetInfo()->vPos.x, (*iter)->GetInfo()->vPos.z);
	}//함정 그리기??

	for(list<CObj*>::iterator iter = m_pEtcList->begin(); iter != m_pEtcList->end(); iter++)
	{

		if((*iter)->GetObjkey() == TEXT("Shopkeeper"))
			DrawPoint2(12, (*iter)->GetInfo()->vPos.x, (*iter)->GetInfo()->vPos.z);
		if(m_bOnOff)
		{
		if((*iter)->GetDarkness() == 0)
			continue;
		}
		DrawPoint2(9, (*iter)->GetInfo()->vPos.x, (*iter)->GetInfo()->vPos.z);
	}//기타 그리기

	for(list<CObj*>::iterator iter = m_pItemList->begin(); iter != m_pItemList->end(); iter++)
	{
		if(m_bOnOff)
		{
		if((*iter)->GetDarkness() == 0)
			continue;
		}
		DrawPoint2(5, (*iter)->GetInfo()->vPos.x, (*iter)->GetInfo()->vPos.z);
	}//아이템 그리기


	if(CBeatMgr::GetInstance()->GetBeatBoomBoom())
		m_iRnd = CRand::Random(0,13);
	DrawPoint2( m_iRnd, m_pPlayer->GetInfo()->vPos.x, m_pPlayer->GetInfo()->vPos.z);//플레이어 그리기
}

void CMiniMap::Release(void)
{
	
}
void CMiniMap::DrawPoint(int iImage, float fX, float fY)
{
	m_pDevice->GetDevice()->BeginScene();
	m_pDevice->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	m_pDevice->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DX_FILTER_NONE);
	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(TEXT("UI"),TEXT("MiniMap"),TEXT(""),iImage);
	if(pTexInfo == NULL)
		return;
	D3DXMATRIX matWorld, matScale, matTrans;
	D3DXMatrixTranslation(&matTrans, (fX*3.f)+810, -(fY*3.f)+720, 0.f);
	D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 0.f);
	matWorld = matScale * matTrans;
	m_pDevice->GetSprite()->SetTransform(&matWorld);
	m_pDevice->GetSprite()->Draw(pTexInfo->pTexture, NULL, 
		&D3DXVECTOR3(float(pTexInfo->ImgInfo.Width>>1), float(pTexInfo->ImgInfo.Height>>1), 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pDevice->GetSprite()->End();
	m_pDevice->GetDevice()->EndScene();

}
void CMiniMap::DrawPoint2(int iImage, float fX, float fY)
{
	m_pDevice->GetLine()->SetWidth(6.f);
	m_pDevice->GetDevice()->BeginScene();
	m_pDevice->GetLine()->Begin();

	D3DXVECTOR2 vPoint[] = { D3DXVECTOR2((fX*2.5f) + m_tInfo.vPos.x, -(fY*2.5f)+m_tInfo.vPos.y),  D3DXVECTOR2((fX*2.5f)+5+m_tInfo.vPos.x, -(fY*2.5f)+m_tInfo.vPos.y)};
	
	m_pDevice->GetLine()->Draw(vPoint, 2, m_dwREF[iImage]);

	m_pDevice->GetLine()->End();
	m_pDevice->GetDevice()->EndScene();

}