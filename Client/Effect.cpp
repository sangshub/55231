#include "stdafx.h"
#include "Effect.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "Pipeline.h"
#include "Device.h"
#include "CamMgr.h"
#include "ObjMgr.h"

CEffect::CEffect()
{
}

CEffect::~CEffect()
{
}

void CEffect::SetMatrix(const float fScale)
{
	CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(fScale, fScale, fScale), m_fAngle, m_tInfo.vPos);

	SetBillBoard();

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

void CEffect::EffectInit()
{
	m_pObjMgr->ResetUIVtx();

	m_dwVtxCnt = 4;

	m_pVtxTex = new VTXTEX[m_dwVtxCnt];
	m_pConvertVtxTex = new VTXTEX[m_dwVtxCnt];

	m_pBufferMgr->GetVtxInfo(TEXT("UITex"), m_pVtxTex);

	m_dwIndexCnt = 2;

	m_pIndex = new INDEX[m_dwIndexCnt];

	m_pBufferMgr->GetIndex(TEXT("UITex"), m_pIndex);

	m_tFrame.fFrame = 0.f;
}

void CEffect::EffectRender(const BYTE byAlpha)
{
	m_pBufferMgr->SetVtxInfo(TEXT("UITex"), m_pConvertVtxTex);

	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(m_wstrObjKey, m_wstrStateKey, TEXT(""), (int)m_tFrame.fFrame);

	if(pTexInfo == NULL)
		return;

	BeginRenderState(byAlpha);

	m_pDevice->GetDevice()->SetTexture(0, pTexInfo->pTexture);

	//m_pDevice->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pDevice->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pBufferMgr->RenderBuffer(TEXT("UITex"));

	EndRenderState();
}

void CEffect::SetUIMatrix(const float fScale)
{
	CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(fScale, fScale, fScale), m_fAngle, m_tInfo.vPos);

	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	m_pCamMgr->GetMatrix(D3DTS_VIEW, matView);
	m_pCamMgr->GetMatrix(D3DTS_PROJECTION, matProj);
	D3DXMatrixOrthoLH(&matProj, 1, 1, 0, 1);

	memcpy(m_pConvertVtxTex, m_pVtxTex, sizeof(VTXTEX) * m_dwVtxCnt);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		//m_pConvertVtxTex[i] = m_pVtxTex[i];

		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, m_tInfo.matWorld);
		//CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, matView);

		//if(m_pConvertVtxTex[i].vPos.z < 1.f)
		//	m_pConvertVtxTex[i].vPos.z = 1.f;

		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, matProj);
	}
}