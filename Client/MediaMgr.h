#pragma once

#include "Include.h"

class CMediaObj;
class CRcTex;

class CMediaMgr
{
public:
	DECLARE_SINGLETON(CMediaMgr)

private:
	CMediaMgr(void);

public:
	~CMediaMgr(void);

public:
	void Initialize(void);
	void Progress(void);
	void Render(void);

private:
	void Release(void);

public:
	HRESULT VideoPlay(WCHAR* wFileName);

private:
	void InitMoviePos(void);
	void ProjVector(D3DXVECTOR3& Vector, const D3DXVECTOR3& VenPoint);
	void ConvertVector(D3DXVECTOR3& Vector, const int iWidth, const int iHeight);
	void VertexRenewal(void);

private:
	CMediaObj*		m_pMediaObj;
	LPDIRECT3DDEVICE9		m_pDevice;

private:
	VTXTEX* m_pVtxPos;
	VTXTEX* m_pVtxConPos;
	D3DXVECTOR3 m_vVanPoint;
	D3DXVECTOR3 m_vPos;
	float		m_fHalfWidth;
	float		m_fHalfHeight;
	CRcTex*		m_pRcTexBuffer;
};
