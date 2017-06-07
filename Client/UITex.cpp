#include "stdafx.h"
#include "UITex.h"

CUITex::CUITex()
{
}

CUITex::~CUITex()
{
}

HRESULT CUITex::CreateBuffer()
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 4;
	m_dwVtxFVF = VTXFVF_TEX;
	m_dwTriCnt = 2;
	m_dwIdSize = sizeof(INDEX);
	m_IdxFmt = D3DFMT_INDEX32;

	if(FAILED(CVIBuffer::CreateBuffer()))
	{
		ERR_MSG(TEXT("CUITex Buffer Create Fail"));

		return E_FAIL;
	}

	VTXTEX* pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	pVtxTex[0].vPos = D3DXVECTOR3(-1.f, 1.f, 0.f);
	pVtxTex[0].vTex = D3DXVECTOR2(0.f, 0.f);

	pVtxTex[1].vPos = D3DXVECTOR3(1.f, 1.f, 0.f);
	pVtxTex[1].vTex = D3DXVECTOR2(1.f, 0.f);

	pVtxTex[2].vPos = D3DXVECTOR3(1.f, -1.f, 0.f);
	pVtxTex[2].vTex = D3DXVECTOR2(1.f, 1.f);

	pVtxTex[3].vPos = D3DXVECTOR3(-1.f, -1.f, 0.f);
	pVtxTex[3].vTex = D3DXVECTOR2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX* pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}