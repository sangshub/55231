#include "stdafx.h"
#include "TerrainCol.h"
#include "Include.h"

CTerrainCol::CTerrainCol()
{
}

CTerrainCol::~CTerrainCol()
{
}

HRESULT CTerrainCol::CreateBuffer()
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = VTXCNTX * VTXCNTZ;
	m_dwVtxFVF = VTXFVF_COL;
	m_dwTriCnt = ((VTXCNTX - 1) * (VTXCNTZ - 1))<<1;
	m_dwIdSize = sizeof(INDEX);
	m_IdxFmt = D3DFMT_INDEX32;

	if(FAILED(CVIBuffer::CreateBuffer()))
	{
		ERR_MSG(TEXT("CTerrainCol Buffer Create Fail"));

		return E_FAIL;
	}

	VTXCOL* pVtxCol = NULL;
	
	int iIndex = 0;

	m_pVB->Lock(0, 0, (void**)&pVtxCol, 0);

	for(int z = 0; z < VTXCNTZ; ++z)
	{
		for(int x = 0; x < VTXCNTX; ++x)
		{
			iIndex = z * VTXCNTX + x;

			pVtxCol[iIndex].vPos = D3DXVECTOR3(float(x * VTXITV), 0.f, float(z * VTXITV));
			pVtxCol[iIndex].dwColor = D3DCOLOR_ARGB(255, 0, 255, 0);
		}
	}
	m_pVB->Unlock();

	INDEX* pIndex = NULL;

	int iTriCnt = 0;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for(int z = 0; z < VTXCNTZ - 1; ++z)
	{
		for(int x = 0; x < VTXCNTX - 1; ++x)
		{
			iIndex = z * VTXCNTX + x;

			pIndex[iTriCnt]._1 = iIndex + VTXCNTX;
			pIndex[iTriCnt]._2 = iIndex + VTXCNTX + 1;
			pIndex[iTriCnt]._3 = iIndex + 1;
			++iTriCnt;

			pIndex[iTriCnt]._1 = iIndex + VTXCNTX;
			pIndex[iTriCnt]._2 = iIndex + 1;
			pIndex[iTriCnt]._3 = iIndex;
			++iTriCnt;

		}
	}
	m_pIB->Unlock();

	return S_OK;
}