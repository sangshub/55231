#pragma once

#include "Include.h"

class CVIBuffer;

class CBufferMgr
{
public:
	DECLARE_SINGLETON(CBufferMgr)

private:
	CBufferMgr();

public:
	~CBufferMgr();

public:
	HRESULT GetVtxInfo(const TCHAR* szBufferKey, void* pVertex);
	HRESULT GetIndex(const TCHAR* szBufferKey, INDEX* pIndex);
	HRESULT SetVtxInfo(const TCHAR* szBufferKey, void* pVertex);

public:
	HRESULT AddBuffer(const TCHAR* szBufferKey, const BUFFERTYPE& eBufferType);
	HRESULT RenderBuffer(const TCHAR* szBufferKey);

private:
	void Release();

private:
	map<const TCHAR*, CVIBuffer*> m_MapBuffer;
};