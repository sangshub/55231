#pragma once

#include "Include.h"

class CDevice;
class CTimeMgr;

class CVIBuffer
{
public:
	CVIBuffer();
	virtual ~CVIBuffer();

public:
	void GetVtxInfo(void* pVertex);
	void GetIndex(INDEX* pIndex);
	void SetVtxInfo(void* pVertex);

public:
	virtual HRESULT CreateBuffer();
	virtual void Render();

private:
	void Release();

protected:
	float m_fX;
	float m_fY;
	float m_fAngle;
	float m_fSize;

	DWORD m_dwVtxSize;
	DWORD m_dwVtxCnt;
	DWORD m_dwVtxFVF;
	DWORD m_dwTriCnt;
	DWORD m_dwIdSize;

	D3DFORMAT m_IdxFmt;

	CDevice* m_pDevice;
	CTimeMgr* m_pTimeMgr;

	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;
};