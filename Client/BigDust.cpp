#include "stdafx.h"
#include "BigDust.h"
#include "Device.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "TimeMgr.h"
#include "CamMgr.h"
#include "Pipeline.h"

CBigDust::CBigDust()
{
}

CBigDust::~CBigDust()
{
}

HRESULT CBigDust::Initialize()
{
	m_wstrObjKey = TEXT("BigDust");
	m_wstrStateKey = TEXT("Normal");

	m_pObjMgr->ResetRcTex();

	m_dwVtxCnt = 4;

	m_pVtxTex = new VTXTEX[m_dwVtxCnt];
	m_pConvertVtxTex = new VTXTEX[m_dwVtxCnt];

	m_pBufferMgr->GetVtxInfo(TEXT("RcTex"), m_pVtxTex);

	m_dwIndexCnt = 2;

	m_pIndex = new INDEX[m_dwIndexCnt];

	m_pBufferMgr->GetIndex(TEXT("RcTex"), m_pIndex);

	m_tFrame.fFrame = 0.f;

	m_eObjId = OBJ_EFFECT;

	return S_OK;
}

const int CBigDust::Progress()
{
	SetMatrix();
	FrameMove();
	SetFrame();

	m_tInfo.vPos.y = 0.3f;

	if(m_tFrame.fFrame > m_tFrame.fMax - 0.5f)
		return 1;

	return 0;
}

void CBigDust::Render()
{
	m_pBufferMgr->SetVtxInfo(TEXT("RcTex"), m_pConvertVtxTex);

	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(m_wstrObjKey, m_wstrStateKey, TEXT(""), (int)m_tFrame.fFrame);

	if(pTexInfo == NULL)
		return;

	BeginRenderState();

	m_pDevice->GetDevice()->SetTexture(0, pTexInfo->pTexture);

	m_pBufferMgr->RenderBuffer(TEXT("RcTex"));

	EndRenderState();
}

void CBigDust::SetMatrix(const float fScale)
{
	CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(fScale, fScale, fScale * 1.2f), m_fAngle, m_tInfo.vPos);

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