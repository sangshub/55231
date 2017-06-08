#pragma once

#include "Include.h"

class CMainCamMgr;
class CObj;

class CMouse
{
public:
	DECLARE_SINGLETON(CMouse)

private:
	CMouse();

public:
	~CMouse();

public:
	bool PickObject(const OBJID& eObjId, CObj* pObj);

private:
	const D3DXVECTOR3 GetMouse() const;
	void TranslationViewSpace(const D3DXVECTOR3& vPoint);
	void TranslationLocal(const D3DXMATRIX& matWorld);

private:
	CMainCamMgr* m_pCamMgr;

	D3DXVECTOR3 m_vPivotPos;
	D3DXVECTOR3 m_vRayDir;
};