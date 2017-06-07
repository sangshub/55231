#include "stdafx.h"
#include "BufferMgr.h"
#include "RcTex.h"
#include "CubeTex.h"
#include "UITex.h"
#include "RcCol.h"

IMPLEMENT_SINGLETON(CBufferMgr)

CBufferMgr::CBufferMgr()
{
}

CBufferMgr::~CBufferMgr()
{
	Release();
}

void CBufferMgr::Release()
{
	for_each(m_MapBuffer.begin(), m_MapBuffer.end(), ::CDeleteMapObj());
	m_MapBuffer.clear();
}

HRESULT CBufferMgr::GetVtxInfo(const TCHAR* szBufferKey, void* pVertex)
{
	map<const TCHAR*, CVIBuffer*>::const_iterator iter = m_MapBuffer.find(szBufferKey);

	if(iter == m_MapBuffer.end())
		return E_FAIL;

	iter->second->GetVtxInfo(pVertex);

	return S_OK;
}

HRESULT CBufferMgr::GetIndex(const TCHAR* szBufferKey, INDEX* pIndex)
{
	map<const TCHAR*, CVIBuffer*>::const_iterator iter = m_MapBuffer.find(szBufferKey);

	if(iter == m_MapBuffer.end())
		return E_FAIL;

	iter->second->GetIndex(pIndex);

	return S_OK;
}

HRESULT CBufferMgr::SetVtxInfo(const TCHAR* szBufferKey, void* pVertex)
{
	map<const TCHAR*, CVIBuffer*>::const_iterator iter = m_MapBuffer.find(szBufferKey);

	if(iter == m_MapBuffer.end())
		return E_FAIL;

	iter->second->SetVtxInfo(pVertex);

	return S_OK;
}

HRESULT CBufferMgr::SetIdxInfo(const TCHAR* szBufferKey, INDEX* pIndex, const DWORD dwIndexCnt)
{
	map<const TCHAR*, CVIBuffer*>::const_iterator iter = m_MapBuffer.find(szBufferKey);

	if(iter == m_MapBuffer.end())
		return E_FAIL;

	iter->second->SetIdxInfo(pIndex, dwIndexCnt);

	return S_OK;
}

HRESULT CBufferMgr::AddBuffer(const TCHAR* szBufferKey, const BUFFERTYPE& eBufferType)
{
	map<const TCHAR*, CVIBuffer*>::const_iterator iter = m_MapBuffer.find(szBufferKey);

	if(iter != m_MapBuffer.end())
		return E_FAIL;

	CVIBuffer* pBuffer = NULL;

	switch (eBufferType)
	{
	case BUFFERTYPE_RCCOL:
		pBuffer = new CRcCol;
		break;
	case BUFFERTYPE_RCTEX:
		pBuffer = new CRcTex;
		break;
	case BUFFERTYPE_CUBE:
		pBuffer = new CCubeTex;
		break;
	case BUFFERTYPE_UI:
		pBuffer = new CUITex;
		break;
	}

	if(FAILED(pBuffer->CreateBuffer()))
		return E_FAIL;

	m_MapBuffer.insert(map<const TCHAR*, CVIBuffer*>::value_type(szBufferKey, pBuffer));
	
	return S_OK;
}

HRESULT CBufferMgr::RenderBuffer(const TCHAR* szBufferKey)
{
	map<const TCHAR*, CVIBuffer*>::const_iterator iter = m_MapBuffer.find(szBufferKey);

	if(iter == m_MapBuffer.end())
		return E_FAIL;

	iter->second->Render();

	return S_OK;
}