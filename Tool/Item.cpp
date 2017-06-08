#include "stdafx.h"
#include "Item.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "Pipeline.h"
#include "MainCamMgr.h"
#include "Device.h"

CItem::CItem()
{
}

CItem::~CItem()
{
}

HRESULT CItem::Initialize()
{
	m_dwVtxCnt = 4;

	m_pVtxTex = new VTXTEX[m_dwVtxCnt];
	m_pConvertVtxTex = new VTXTEX[m_dwVtxCnt];

	m_pBufferMgr->GetVtxInfo(TEXT("RcTex"), m_pVtxTex);

	m_dwIndexCnt = 2;

	m_pIndex = new INDEX[m_dwIndexCnt];

	m_pBufferMgr->GetIndex(TEXT("RcTex"), m_pIndex);
	
	m_wstrObjKey = TEXT("Shovel");
	m_wstrStateKey = TEXT("Normal");

	return S_OK;
}

int CItem::Progress()
{
	SetMatrix();

	if(m_bIsDelete)
		return 1;

	return 0;
}

void CItem::Render()
{
	m_pBufferMgr->SetVtxInfo(TEXT("RcTex"), m_pConvertVtxTex);

	const TEXINFO* pTexInfo = NULL;

	pTexInfo = m_pTextureMgr->GetTexture(m_wstrObjKey, m_wstrStateKey, TEXT(""), 0);

	if(pTexInfo == NULL)
		return;

	BeginRenderState();

	m_pDevice->GetDevice()->SetTexture(0, pTexInfo->pTexture);

	m_pDevice->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pDevice->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pBufferMgr->RenderBuffer(TEXT("RcTex"));

	EndRenderState();
}

void CItem::SetMatrix()
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