#pragma once

#include "Include.h"

class CCamMgr
{
public:
	DECLARE_SINGLETON(CCamMgr)

private:
	CCamMgr(void);

public:
	~CCamMgr(void);

public:
	void SetCamPosition(const D3DXVECTOR3& vEye
		, const D3DXVECTOR3& vAt
		, const D3DXVECTOR3& vUp);

	void SetProjection(const float fFovY
		, const float fAspect
		, const float fNear
		, const float fFar);

	void GetMatrix(_D3DTRANSFORMSTATETYPE eType, D3DXMATRIX& pOut);

private:
	D3DXMATRIX	m_matView;
	D3DXMATRIX	m_matProj;

/////////////////////////////////////////////////////////////////////

public:
	void SetAniCamPosition(const D3DXVECTOR3& vEye
		, const D3DXVECTOR3& vAt
		, const D3DXVECTOR3& vUp);

	void SetAniProjection(const float fFovY
		, const float fAspect
		, const float fNear
		, const float fFar);

	void GetAniMatrix(_D3DTRANSFORMSTATETYPE eType, D3DXMATRIX& pOut);

private:
	D3DXMATRIX	m_matAniView;
	D3DXMATRIX	m_matAniProj;
};