#include "stdafx.h"
#include "Frustum.h"
#include "Obj.h"
#include "MainCamMgr.h"
#include "ObjMgr.h"

IMPLEMENT_SINGLETON(CFrustum)

CFrustum::CFrustum()
: m_fDistance(0.f)
, m_fMinX(0.f)
, m_fMaxX(0.f)
, m_fMinZ(0.f)
, m_fMaxZ(0.f)
, m_pCaMgr(CMainCamMgr::GetInstance())
{
	ZeroMemory(m_vPoint, sizeof(D3DXVECTOR3) * 8);
	ZeroMemory(m_Plane, sizeof(D3DXPLANE) * 6);
}

CFrustum::~CFrustum()
{
}

const float CFrustum::GetMinX() const
{
	return m_fMinX;
}

const float CFrustum::GetMaxX() const
{
	return m_fMaxX;
}

const float CFrustum::GetMinZ() const
{
	return m_fMinZ;
}

const float CFrustum::GetMaxZ() const
{
	return m_fMaxZ;
}

void CFrustum::SetFrustum()
{
	m_vPoint[0] = D3DXVECTOR3(-1.f, 1.f, 0.f);
	m_vPoint[1] = D3DXVECTOR3(1.f, 1.f, 0.f);
	m_vPoint[2] = D3DXVECTOR3(1.f, -1.f, 0.f);
	m_vPoint[3] = D3DXVECTOR3(-1.f, -1.f, 0.f);

	m_vPoint[4] = D3DXVECTOR3(-1.f, 1.f, 1.f);
	m_vPoint[5] = D3DXVECTOR3(1.f, 1.f, 1.f);
	m_vPoint[6] = D3DXVECTOR3(1.f, -1.f, 1.f);
	m_vPoint[7] = D3DXVECTOR3(-1.f, -1.f, 1.f);

	D3DXMATRIX matView;
	D3DXMATRIX matProj;

	m_pCaMgr->GetMatrix(D3DTS_VIEW, matView);
	m_pCaMgr->GetMatrix(D3DTS_PROJECTION, matProj);

	D3DXMATRIX matInverseView;
	D3DXMATRIX matInverseProj;

	D3DXMatrixInverse(&matInverseView, 0, &matView);
	D3DXMatrixInverse(&matInverseProj, 0, &matProj);

	float fArrayX[8] = {0.f};
	float fArrayZ[8] = {0.f};

	for(int i = 0; i < 8; ++i)
	{
		// 투영 -> 뷰스페이스
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matInverseProj);

		// 뷰스페이스 -> 월드
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matInverseView);

		fArrayX[i] = m_vPoint[i].x;
		fArrayZ[i] = m_vPoint[i].z;
	}

	m_fMinX = GetMinArray(fArrayX, 8);
	m_fMaxX = GetMaxArray(fArrayX, 8);

	m_fMinZ = GetMinArray(fArrayZ, 8);
	m_fMaxZ = GetMaxArray(fArrayZ, 8);

	// +x
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);
	// -x
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);
	// +y
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);
	// -y
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);
	// +z
	D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);
	// -z
	D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);
}

const bool CFrustum::FrustumCulling(const D3DXVECTOR3& vPos) const
{
	if(Check(vPos))
		return true;

	return false;
}

const bool CFrustum::Check(const D3DXVECTOR3& vPos) const
{
	if(MyPlandDotCoord(m_Plane[0], vPos) > m_fDistance)
		return false;
	else if(MyPlandDotCoord(m_Plane[1], vPos) > m_fDistance)
		return false;
	else if(MyPlandDotCoord(m_Plane[2], vPos) > m_fDistance)
		return false;
	else if(MyPlandDotCoord(m_Plane[3], vPos) > m_fDistance)
		return false;
	else if(MyPlandDotCoord(m_Plane[4], vPos) > m_fDistance)
		return false;
	else if(MyPlandDotCoord(m_Plane[5], vPos) > m_fDistance)
		return false;
	else
		return true;
}

const float CFrustum::MyPlandDotCoord(const D3DXPLANE& Plane, const D3DXVECTOR3& vPos) const
{
	return Plane.a * vPos.x + Plane.b * vPos.y + Plane.c * vPos.z + Plane.d;
}

const float CFrustum::GetMinArray(float* fNum, int iSize)
{
	float fMin = fNum[0];

	for(int i = 0; i < iSize; ++i)
	{
		if(fNum[i] < fMin)
			fMin = fNum[i];
	}

	if(fMin < 0.f)
		fMin = 0.f;

	return fMin;
}

const float CFrustum::GetMaxArray(float* fNum, int iSize)
{
	float fMax = fNum[0];

	for(int i = 0; i < iSize; ++i)
	{
		if(fNum[i] > fMax)
			fMax = fNum[i];
	}

	return fMax;
}