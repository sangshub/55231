#pragma once

#include "Include.h"

class CDevice;

class CMainCamMgr
{
public:
	DECLARE_SINGLETON(CMainCamMgr)

private:
	CMainCamMgr();

public:
	~CMainCamMgr();

public:
	void GetMatrix(const _D3DTRANSFORMSTATETYPE& eType, D3DXMATRIX& matOut);

public:
	void InitCamera();
	void SetCamPosition(const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt, D3DXVECTOR3& vUp);

private:
	CDevice* m_pDevice;

	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
};