#pragma once

#include "Include.h"

class CObj;
class CMainCamMgr;

class CFrustum
{
public:
	DECLARE_SINGLETON(CFrustum)

private:
	CFrustum();

public:
	~CFrustum();

public:
	const float GetMinX() const;
	const float GetMaxX() const;
	const float GetMinZ() const;
	const float GetMaxZ() const;

public:
	void SetFrustum();
	const bool FrustumCulling(const D3DXVECTOR3& vPos) const;

private:
	const bool Check(const D3DXVECTOR3& vPos) const;
	const float MyPlandDotCoord(const D3DXPLANE& Plane, const D3DXVECTOR3& vPos) const;
	const float GetMinArray(float* fNum, int iSize);
	const float GetMaxArray(float* fNum, int iSize);

private:
	float m_fDistance;
	float m_fMinX;
	float m_fMaxX;
	float m_fMinZ;
	float m_fMaxZ;
	CMainCamMgr* m_pCaMgr;

	D3DXVECTOR3 m_vPoint[8]; // 절두체의 정점
	D3DXPLANE m_Plane[6]; // 절두체의 면
};