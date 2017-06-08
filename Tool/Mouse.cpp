#include "stdafx.h"
#include "Mouse.h"
#include "MainCamMgr.h"
#include "Tile.h"

IMPLEMENT_SINGLETON(CMouse)

CMouse::CMouse()
: m_pCamMgr(CMainCamMgr::GetInstance())
{
}

CMouse::~CMouse()
{
}

const D3DXVECTOR3 CMouse::GetMouse() const
{
	POINT tPoint;

	GetCursorPos(&tPoint);
	ScreenToClient(g_hWnd, &tPoint);

	return D3DXVECTOR3(float(tPoint.x), float(tPoint.y), 0.f);
}

void CMouse::TranslationViewSpace(const D3DXVECTOR3& vPoint)
{
	D3DXMATRIX matProj;
	m_pCamMgr->GetMatrix(D3DTS_PROJECTION, matProj);

	m_vRayDir.x = (float(vPoint.x) / float(WINCX>>1) - 1.f) / matProj._11;
	m_vRayDir.y = (float(-vPoint.y) / float(WINCY>>1) + 1.f) / matProj._22;
	m_vRayDir.z = 1.f;

	m_vPivotPos = D3DXVECTOR3(0.f, 0.f, 0.f);
}

void CMouse::TranslationLocal(const D3DXMATRIX& matWorld)
{
	D3DXMATRIX matView;

	m_pCamMgr->GetMatrix(D3DTS_VIEW, matView);

	D3DXMatrixInverse(&matView, 0, &matView);
	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matView);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matView);

	D3DXMATRIX matWorldInverse;
	D3DXMatrixInverse(&matWorldInverse, 0, &matWorld);
	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matWorldInverse);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matWorldInverse);

	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
}

bool CMouse::PickObject(const OBJID& eObjId, CObj* pObj)
{
	D3DXVECTOR3 vMouse = GetMouse();
	TranslationViewSpace(vMouse);

	TranslationLocal(pObj->GetInfo()->matWorld);

	float fU = 0.f;
	float fV = 0.f;
	float fDistance = 0.f;

	const INDEX* pIndex = pObj->GetIndex();

	int iMax = pObj->GetIndexCnt();

	if(eObjId == OBJ_TERRAIN)
	{
		VTXTEX* pVtxTex = (VTXTEX*)pObj->GetVtxPointer();

		for(int i = 0; i < iMax; ++i)
		{
			if(D3DXIntersectTri(&pVtxTex[pIndex[i]._1].vPos, &pVtxTex[pIndex[i]._2].vPos, &pVtxTex[pIndex[i]._3].vPos, 
				&m_vPivotPos, &m_vRayDir, &fU, &fV, &fDistance))
				return true;
		}
	}
	return false;
}