#include "stdafx.h"
#include "CubeTex.h"
#include "Include.h"

CCubeTex::CCubeTex()
{
}

CCubeTex::~CCubeTex()
{
}

HRESULT CCubeTex::CreateBuffer()
{
	m_dwVtxSize = sizeof(VTXCUBE);
	m_dwVtxCnt = 8;
	m_dwVtxFVF = VTXFVF_CUBE;
	m_dwTriCnt = 12;
	m_dwIdSize = sizeof(INDEX);
	m_IdxFmt = D3DFMT_INDEX32;

	if(FAILED(CVIBuffer::CreateBuffer()))
	{
		ERR_MSG(TEXT("CubeTex Buffer Create Fail"));

		return E_FAIL;
	}

	VTXCUBE* pCubeTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pCubeTex, 0);

	pCubeTex[0].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pCubeTex[0].vTex = pCubeTex[0].vPos;

	pCubeTex[1].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pCubeTex[1].vTex = pCubeTex[1].vPos;

	pCubeTex[2].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pCubeTex[2].vTex = pCubeTex[2].vPos;

	pCubeTex[3].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pCubeTex[3].vTex = pCubeTex[3].vPos;

	pCubeTex[4].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pCubeTex[4].vTex = pCubeTex[4].vPos;

	pCubeTex[5].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pCubeTex[5].vTex = pCubeTex[5].vPos;

	pCubeTex[6].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pCubeTex[6].vTex = pCubeTex[6].vPos;

	pCubeTex[7].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pCubeTex[7].vTex = pCubeTex[7].vPos;

	m_pVB->Unlock();

	INDEX* pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 1;
	pIndex[0]._2 = 5;
	pIndex[0]._3 = 6;
	pIndex[1]._1 = 1;
	pIndex[1]._2 = 6;
	pIndex[1]._3 = 2;

	pIndex[2]._1 = 4;
	pIndex[2]._2 = 0;
	pIndex[2]._3 = 3;
	pIndex[3]._1 = 4;
	pIndex[3]._2 = 3;
	pIndex[3]._3 = 7;

	pIndex[4]._1 = 4;
	pIndex[4]._2 = 5;
	pIndex[4]._3 = 1;
	pIndex[5]._1 = 4;
	pIndex[5]._2 = 1;
	pIndex[5]._3 = 0;

	pIndex[6]._1 = 3;
	pIndex[6]._2 = 2;
	pIndex[6]._3 = 6;
	pIndex[7]._1 = 3;
	pIndex[7]._2 = 6;
	pIndex[7]._3 = 7;

	pIndex[8]._1 = 7;
	pIndex[8]._2 = 6;
	pIndex[8]._3 = 5;
	pIndex[9]._1 = 7;
	pIndex[9]._2 = 5;
	pIndex[9]._3 = 4;

	pIndex[10]._1 = 0;
	pIndex[10]._2 = 1;
	pIndex[10]._3 = 2;
	pIndex[11]._1 = 0;
	pIndex[11]._2 = 2;
	pIndex[11]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}