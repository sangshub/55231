#include "stdafx.h"
#include "Tile.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "Pipeline.h"
#include "Device.h"
#include "CamMgr.h"
#include "ObjMgr.h"
#include "SceneMgr.h"
#include "Stage.h"
#include "BeatMgr.h"

CTile::CTile()
: m_bFindWay(false)
, m_iOption(0)
, m_iDrawId(0)
, m_fDarkLv(1.f)
, m_pTileObj(NULL)
, m_pItemObj(NULL)
{
}

CTile::~CTile()
{
}

HRESULT CTile::Initialize()
{
	m_dwVtxCnt = 4;

	m_pVtxTex = new VTXTEX[m_dwVtxCnt];
	m_pConvertVtxTex = new VTXTEX[m_dwVtxCnt];

	m_pBufferMgr->GetVtxInfo(TEXT("RcTex"), m_pVtxTex);

	m_dwIndexCnt = 2;

	m_pIndex = new INDEX[m_dwIndexCnt];

	m_pBufferMgr->GetIndex(TEXT("RcTex"), m_pIndex);

	m_eObjId = OBJ_TERRAIN;

	return S_OK;
}

const int CTile::Progress()
{
	SetDarknessValue();

	if(!m_bStageEnd)
		SetMatrix();
	
	StageEndScene(); //¿£µù

	/*if(m_pTileObj != NULL)
	{
		m_iDrawId = 10;
		if(m_pTileObj->GetObjId() == OBJ_PLAYER)
			m_iDrawId = 5;
	}
	else
		m_iDrawId = 0;*/

	if(m_bIsDelete)
		return 1;

	return 0;
}

void CTile::Render()
{
	m_pBufferMgr->SetVtxInfo(TEXT("RcTex"), m_pConvertVtxTex);

	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(TEXT("Tile"), m_wstrStateKey, TEXT(""), m_iDrawId);

	if(pTexInfo == NULL)
		return;

	BeginRenderState();
	SetDark(m_iDarkness);

	m_pDevice->GetDevice()->SetTexture(0, pTexInfo->pTexture);

	//m_pDevice->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pDevice->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pBufferMgr->RenderBuffer(TEXT("RcTex"));

	EndRenderState();
}

const int CTile::GetOption() const
{
	return m_iOption;
}

const int CTile::GetDrawId() const
{
	return m_iDrawId;
}

CObj* CTile::GetObj() const
{
	return m_pTileObj;
}

CItem* CTile::GetItem() const
{
	return m_pItemObj;
}

const OBJID CTile::GetTileObjId() const
{
	if(m_pTileObj == NULL)
		return OBJ_END;

	return m_pTileObj->GetObjId();
}

void CTile::SetDrawId(const int iDrawId)
{
	m_iDrawId = iDrawId;
}

void CTile::SetObj(CObj* pObj)
{
	m_pTileObj = pObj;
}

void CTile::SetItem(CItem* pItem)
{
	m_pItemObj = pItem;
}

void CTile::ResetObj()
{
	m_pTileObj = NULL;
}

void CTile::ResetItem()
{
	m_pItemObj = NULL;
}

void CTile::SetMatrix(const float fScale)
{
	CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(fScale, fScale, fScale), m_fAngle, m_tInfo.vPos);

	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	m_pCamMgr->GetMatrix(D3DTS_VIEW, matView);
	m_pCamMgr->GetMatrix(D3DTS_PROJECTION, matProj);

	memcpy(m_pConvertVtxTex, m_pVtxTex, sizeof(VTXTEX) * m_dwVtxCnt);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		//m_pConvertVtxTex[i] = m_pVtxTex[i];

		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, m_tInfo.matWorld);
		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, matView);

		if(m_pConvertVtxTex[i].vPos.z < 1.f)
			m_pConvertVtxTex[i].vPos.z = 1.f;

		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, matProj);
	}
}

void CTile::SetFindWay(const bool bFindWay)
{
	m_bFindWay = bFindWay;
}

const wstring CTile::GetTileObjKey() const
{
	return m_pTileObj->GetObjkey();
}

void CTile::SetDarknessValue()
{
	if(m_bFindWay)
		m_iDarkness = 50;
	else
		m_iDarkness = 0;

	if(((CStage*)m_pSceneMgr->GetSceneObj())->GetBossStage()
		|| m_pObjMgr->GetActionCamSwitch())
	{
		if(m_pObjMgr->GetDarknessSwitch()
			&& m_pBeatMgr->GetBeatBoomBoom())
		{
			m_fDarkLv -= 0.33f;
			if(m_fDarkLv <= 0.f)
			{
				m_pObjMgr->SetDarknessSwitch(false);
				m_pObjMgr->SetActionCam(false);
				m_fDarkLv = 0.f;
			}
		}
		m_iDarkness = int(255 * m_fDarkLv);
	}
	else
		m_fDarkLv = 1.f;
}