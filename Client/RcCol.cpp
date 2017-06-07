#include "stdafx.h"
#include "RcCol.h"
#include "Include.h"

CRcCol::CRcCol()
{
}

CRcCol::~CRcCol()
{
}

HRESULT CRcCol::CreateBuffer()
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 4;
	m_dwVtxFVF = VTXFVF_COL;
	m_dwTriCnt = 2;
	m_dwIdSize = sizeof(INDEX);
	m_IdxFmt = D3DFMT_INDEX32;

	if(FAILED(CVIBuffer::CreateBuffer()))
	{
		ERR_MSG(TEXT("CRcCol Buffer Create Fail"));

		return E_FAIL;
	}

	VTXCOL* pVtxCol = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxCol, 0);

	pVtxCol[0].vPos = D3DXVECTOR3(-1.f, 1.f, 0.f);
	pVtxCol[0].dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	pVtxCol[1].vPos = D3DXVECTOR3(1.f, 1.f, 0.f);
	pVtxCol[1].dwColor = D3DCOLOR_ARGB(255, 0, 255, 0);

	pVtxCol[2].vPos = D3DXVECTOR3(1.f, -1.f, 0.f);
	pVtxCol[2].dwColor = D3DCOLOR_ARGB(255, 255, 255, 0);

	pVtxCol[3].vPos = D3DXVECTOR3(-1.f, -1.f, 0.f);
	pVtxCol[3].dwColor = D3DCOLOR_ARGB(255, 255, 255, 0);

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