#include "stdafx.h"
#include "VIBuffer.h"
#include "Device.h"

CVIBuffer::CVIBuffer()
: m_pDevice(CDevice::GetInstance())
, m_pVB(NULL)
, m_pIB(NULL)
, m_fX(0.f)
, m_fY(0.f)
, m_fAngle(0.f)
, m_fSize(1.f)
, m_dwVtxSize(0)
, m_dwVtxCnt(0)
, m_dwVtxFVF(0)
, m_dwTriCnt(0)
, m_dwIdSize(0)
{
}

CVIBuffer::~CVIBuffer()
{
	Release();
}

HRESULT CVIBuffer::CreateBuffer()
{
	if(FAILED(m_pDevice->GetDevice()->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt, 0, 
		m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, NULL)))
	{
		ERR_MSG(TEXT("VertexBuffer Create Fail"));

		return E_FAIL;
	}

	if(FAILED(m_pDevice->GetDevice()->CreateIndexBuffer(m_dwIdSize * m_dwTriCnt, 0, 
		m_IdxFmt, D3DPOOL_MANAGED, &m_pIB, NULL)))
	{
		ERR_MSG(TEXT("IndexBuffer Create Fail"));

		return E_FAIL;
	}

	return S_OK;
}

void CVIBuffer::Render()
{
	m_pDevice->GetDevice()->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pDevice->GetDevice()->SetFVF(m_dwVtxFVF);
	m_pDevice->GetDevice()->SetIndices(m_pIB);
	m_pDevice->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

void CVIBuffer::Release()
{
	::Safe_Release(m_pVB);
	::Safe_Release(m_pIB);
}

void CVIBuffer::GetVtxInfo(void* pVertex)
{
	void* pOriVertex = NULL;

	m_pVB->Lock(0, 0, &pOriVertex, 0);

	memcpy(pVertex, pOriVertex, m_dwVtxCnt * m_dwVtxSize);

	m_pVB->Unlock();
}

void CVIBuffer::GetIndex(INDEX* pIndex)
{
	INDEX* pOriIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pOriIndex, 0);

	memcpy(pIndex, pOriIndex, m_dwTriCnt * m_dwIdSize);

	m_pIB->Unlock();
}

void CVIBuffer::SetVtxInfo(void* pVertex)
{
	void* pOriVertex = NULL;

	m_pVB->Lock(0, 0, &pOriVertex, 0);

	memcpy(pOriVertex, pVertex, m_dwVtxCnt * m_dwVtxSize);

	m_pVB->Unlock();
}

void CVIBuffer::SetIdxInfo(INDEX* pIndex, const DWORD dwIndexCnt)
{
	INDEX* pOriIdx = NULL;

	m_dwTriCnt = dwIndexCnt;

	m_pIB->Lock(0, 0, (void**)&pOriIdx, 0);

	memcpy(pOriIdx, pIndex, m_dwIdSize * m_dwTriCnt);

	m_pIB->Unlock();
}
void CVIBuffer::UpdateBuffer(const VTXTEX* pVertex)
{
	VTXTEX* pVtx = NULL;
	m_pVB->Lock(0, 0, (void**)&pVtx, 0);

	memcpy(pVtx, pVertex, m_dwVtxSize * m_dwVtxCnt);

	m_pVB->Unlock();
}