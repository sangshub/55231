#include "stdafx.h"
#include "Tile.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "Pipeline.h"
#include "MainCamMgr.h"
#include "Device.h"
#include "CamMgr.h"

CTile::CTile()
: m_iOption(0)
, m_iDrawId(0)
, m_iCubeCnt(0)
, m_iObjCnt(0)
, m_pObj(NULL)
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

	return S_OK;
}

int CTile::Progress()
{
	SetMatrix();

	return 0;
}

void CTile::Render()
{
	m_pBufferMgr->SetVtxInfo(TEXT("RcTex"), m_pConvertVtxTex);

	const TEXINFO* pTexInfo = NULL;

	pTexInfo = m_pTextureMgr->GetTexture(TEXT("Tile"), m_wstrStateKey, TEXT(""), m_iDrawId);

	if(pTexInfo == NULL)
		return;

	m_pDevice->GetDevice()->SetTexture(0, pTexInfo->pTexture);

	m_pDevice->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pBufferMgr->RenderBuffer(TEXT("RcTex"));
}

const int CTile::GetOption() const
{
	return m_iOption;
}

const int CTile::GetDrawId() const
{
	return m_iDrawId;
}

const int CTile::GetCubeCnt() const
{
	return m_iCubeCnt;
}

const int CTile::GetObjCnt() const
{
	return m_iObjCnt;
}

CObj* CTile::GetTileObj() const
{
	return m_pObj;
}

void CTile::SetDrawId(const int iDrawId)
{
	m_iDrawId = iDrawId;
}

void CTile::SetCubeCnt(const int iCubeCnt)
{
	m_iCubeCnt = iCubeCnt;
}

void CTile::SetObjCnt(const int iObjCnt)
{
	m_iObjCnt = iObjCnt;
}

void CTile::SetStateKey(const wstring& wstrStateKey)
{
	m_wstrStateKey = wstrStateKey;
}

void CTile::SetTileObj(CObj* pObj)
{
	m_pObj = pObj;
}

void CTile::SetMatrix()
{
	CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(1.f, 1.f, 1.f), m_fAngle, m_tInfo.vPos);

	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	m_pMainCamMgr->GetMatrix(D3DTS_VIEW, matView);
	m_pMainCamMgr->GetMatrix(D3DTS_PROJECTION, matProj);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVtxTex[i] = m_pVtxTex[i];

		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, m_tInfo.matWorld);
		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, matView);

		if(m_pConvertVtxTex[i].vPos.z < 1.f)
			m_pConvertVtxTex[i].vPos.z = 1.f;

		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, matProj);
	}
}

void CTile::SetAniMatrix()
{
	CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(1.f, 1.f, 1.f), m_fAngle, m_tInfo.vPos);

	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	m_pCamMgr->GetAniMatrix(D3DTS_VIEW, matView);
	m_pCamMgr->GetAniMatrix(D3DTS_PROJECTION, matProj);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVtxTex[i] = m_pVtxTex[i];

		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, m_tInfo.matWorld);
		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, matView);

		if(m_pConvertVtxTex[i].vPos.z < 1.f)
			m_pConvertVtxTex[i].vPos.z = 1.f;

		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, matProj);
	}
}