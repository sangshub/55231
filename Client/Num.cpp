#include "StdAfx.h"
#include "Num.h"
#include "Device.h"
#include "BufferMgr.h"
#include "TextureMgr.h"

CNum::CNum(void)
{
}

CNum::~CNum(void)
{
}

HRESULT CNum::Initialize()
{
	m_wstrObjKey = TEXT("Text");
	m_wstrStateKey = TEXT("AlphaBet");

	/*m_tFrame.fCnt = 0;
	m_tFrame.fFrame = 0;
	m_tFrame.fMax= 0;*/
	m_tFrame.fFrame = 0;

	EffectInit();

	m_eObjId = OBJ_EFFECT;

	return S_OK;
}

const int CNum::Progress()
{
	SetMatrix(0.2f);

	return 0;
}

void CNum::Render()
{
	m_pBufferMgr->SetVtxInfo(TEXT("UITex"), m_pConvertVtxTex);

	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(m_wstrObjKey, m_wstrStateKey, TEXT(""), (int)m_tFrame.fFrame);

	if(pTexInfo == NULL)
		return;

	BeginRenderState();
	SetDark(m_iDarkness);

	m_pDevice->GetDevice()->SetTexture(0, pTexInfo->pTexture);

	//m_pDevice->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pDevice->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pBufferMgr->RenderBuffer(TEXT("UITex"));

	EndRenderState();
}