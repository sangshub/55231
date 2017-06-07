#include "StdAfx.h"
#include "Diamond.h"

CDiamond::CDiamond(void)
{
}

CDiamond::~CDiamond(void)
{
}

HRESULT CDiamond::Initialize(void)
{
	m_dwVtxCnt = 4;
	m_pVtxTex = new VTXTEX[m_dwVtxCnt];
	m_pConvertVtxTex = new VTXTEX[m_dwVtxCnt];
	m_pBufferMgr->GetVtxInfo(TEXT("UITex"), m_pVtxTex);

	m_dwIndexCnt = 2;
	m_pIndex = new INDEX[m_dwIndexCnt];
	m_pBufferMgr->GetIndex(TEXT("UITex"), m_pIndex);

	m_wstrObjKey = TEXT("Diamond");
	m_wstrStateKey = TEXT("Normal");

	m_tInfo.vPos = D3DXVECTOR3(15.f, 10.0f, 0.f);

	m_fSprCX = 38;	m_fSprCY = 27;



	return S_OK;
}

const int CDiamond::Progress(void)
{
	SetMatrix();

	return 0;
}

void CDiamond::Render(void)
{
	m_pBufferMgr->SetVtxInfo(TEXT("UITex"), m_pConvertVtxTex);

	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(m_wstrObjKey,m_wstrStateKey,TEXT(""),0);

	if(pTexInfo == NULL)
		return;

	BeginRenderState();

	m_pDevice->GetDevice()->SetTexture(0, pTexInfo->pTexture);

	//m_pDevice->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pDevice->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pBufferMgr->RenderBuffer(TEXT("UITex"));

	EndRenderState();
}

void CDiamond::Release(void)
{
	
}
