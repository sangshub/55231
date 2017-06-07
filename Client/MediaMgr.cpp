#include "StdAfx.h"
#include "MediaMgr.h"
#include "MediaObj.h"
#include "RcTex.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CMediaMgr)

CMediaMgr::CMediaMgr(void)
: m_pMediaObj(NULL)
, m_pRcTexBuffer(NULL)
, m_pVtxPos(NULL)
, m_pVtxConPos(NULL)
, m_pDevice(CDevice::GetInstance()->GetDevice())
, m_fHalfWidth(0.f)
, m_fHalfHeight(0.f)
, m_vVanPoint(D3DXVECTOR3(0.f, 0.f, 0.f))
, m_vPos(D3DXVECTOR3(0.f, 0.f, 0.f))
{
}

CMediaMgr::~CMediaMgr(void)
{
	Release();
}

HRESULT CMediaMgr::VideoPlay(WCHAR* wFileName)
{
	Initialize();

	m_pMediaObj = new CMediaObj;
	m_pMediaObj->Initialize(wFileName);

	return S_OK;
}

void CMediaMgr::Initialize(void)
{
	CoInitialize(NULL);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	InitMoviePos();

	m_pRcTexBuffer = new CRcTex;
	m_pRcTexBuffer->CreateBuffer();
}

void CMediaMgr::Progress(void)
{
	VertexRenewal();
}

void CMediaMgr::Render(void)
{
	m_pMediaObj->SetTexMovie(0);
	m_pRcTexBuffer->UpdateBuffer(m_pVtxConPos);
	m_pRcTexBuffer->Render();
}

void CMediaMgr::Release(void)
{
	Safe_Delete(m_pRcTexBuffer);
	Safe_Delete(m_pMediaObj);
	Safe_Delete(m_pVtxConPos);
	Safe_Delete(m_pVtxPos);
	CoUninitialize();
}

void CMediaMgr::InitMoviePos(void)
{
	m_vVanPoint = D3DXVECTOR3(float(WINCX>>1), 100.f, 500.f);

	m_vPos = D3DXVECTOR3(float(WINCX>>1), float(WINCY>>1), 0.f);
	m_vPos.z = m_vVanPoint.z;
	m_fHalfWidth = float(WINCX>>1);
	m_fHalfHeight = float(WINCY>>1);

	m_pVtxPos = new VTXTEX[4];
	m_pVtxConPos = new VTXTEX[4];

	for(int i = 0; i < 4; ++i)
	{
		m_pVtxPos[i].vPos = D3DXVECTOR3(m_vPos.x + (i % 3 == 0 ? -m_fHalfWidth : m_fHalfWidth), 
			m_vPos.y + (i < 2 ? -m_fHalfHeight : m_fHalfHeight), 
			m_vPos.z);
		m_pVtxPos[i].vTex = D3DXVECTOR2((i % 3 == 0 ? 0.f : 1.f), 
			(i < 2 ? 0.f : 1.f));
	}
}

void CMediaMgr::VertexRenewal(void)
{
	if(m_pVtxPos == NULL || m_pVtxConPos == NULL)
		return;

	for(int i = 0; i < 4; ++i)
	{
		m_pVtxConPos[i] = m_pVtxPos[i];

		ProjVector(m_pVtxConPos[i].vPos, m_vVanPoint);

		ConvertVector(m_pVtxConPos[i].vPos, WINCX, WINCY);

		m_pVtxConPos[i].vPos.z = 0.f;
	}
}

void CMediaMgr::ProjVector(D3DXVECTOR3& Vector, const D3DXVECTOR3& VenPoint)
{
	Vector.x = VenPoint.x + (Vector.x - VenPoint.x) * (VenPoint.z / Vector.z);
	Vector.y = VenPoint.y + (Vector.y - VenPoint.y) * (VenPoint.z / Vector.z);
}

void CMediaMgr::ConvertVector(D3DXVECTOR3& Vector, const int iWidth, const int iHeight)
{
	Vector.x = (Vector.x * 2.f / iWidth) - 1.f;
	Vector.y = -((Vector.y * 2.f / iHeight) - 1.f);
}