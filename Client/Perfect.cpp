#include "stdafx.h"
#include "Perfect.h"
#include "Device.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "TimeMgr.h"
#include "CamMgr.h"
#include "Pipeline.h"

CPerfect::CPerfect()
: m_fTime(0.f)
{
}

CPerfect::~CPerfect()
{
}

HRESULT CPerfect::Initialize()
{
	m_wstrObjKey = TEXT("Perfect");
	m_wstrStateKey = TEXT("Normal");

	m_pObjMgr->ResetUIVtx();

	m_dwVtxCnt = 4;

	m_pVtxTex = new VTXTEX[m_dwVtxCnt];
	m_pConvertVtxTex = new VTXTEX[m_dwVtxCnt];

	m_pBufferMgr->GetVtxInfo(TEXT("UITex"), m_pVtxTex);

	m_dwIndexCnt = 2;

	m_pIndex = new INDEX[m_dwIndexCnt];

	m_pBufferMgr->GetIndex(TEXT("UITex"), m_pIndex);

	m_tFrame.fFrame = 0.f;

	m_eObjId = OBJ_EFFECT;

	m_tInfo.vPos = D3DXVECTOR3(2.f, 2.f, 0.f);

	return S_OK;
}

const int CPerfect::Progress()
{
	m_fTime += m_pTimeMgr->GetTime();

	SetMatrix(2.f);

	m_tInfo.vPos.y += m_fTime * 0.08f;

	if(m_fTime > 5.f)
		return 1;

	return 0;
}

void CPerfect::Render()
{
	m_pBufferMgr->SetVtxInfo(TEXT("UITex"), m_pConvertVtxTex);

	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(m_wstrObjKey, m_wstrStateKey, TEXT(""), 0);

	if(pTexInfo == NULL)
		return;

	BeginRenderState();

	m_pDevice->GetDevice()->SetTexture(0, pTexInfo->pTexture);

	m_pBufferMgr->RenderBuffer(TEXT("UITex"));

	EndRenderState();
}

void CPerfect::SetMatrix(const float fScale)
{
	CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(fScale, fScale * 0.2f, fScale), m_fAngle, m_tInfo.vPos);

	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	m_pCamMgr->GetMatrix(D3DTS_VIEW, matView);
	m_pCamMgr->GetMatrix(D3DTS_PROJECTION, matProj);
	D3DXMatrixOrthoLH(&matProj, 22.f, 13.f, 0, 1);

	memcpy(m_pConvertVtxTex, m_pVtxTex, sizeof(VTXTEX) * m_dwVtxCnt);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		//m_pConvertVtxTex[i] = m_pVtxTex[i];

		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, m_tInfo.matWorld);
		//CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, matView);

		/*if(m_pConvertVtxTex[i].vPos.z < 1.f)
			m_pConvertVtxTex[i].vPos.z = 1.f;*/

		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, matProj);
	}
}