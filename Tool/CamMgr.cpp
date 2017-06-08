#include "StdAfx.h"
#include "CamMgr.h"
#include "Pipeline.h"

#include "Device.h"

IMPLEMENT_SINGLETON(CCamMgr)

CCamMgr::CCamMgr(void)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

CCamMgr::~CCamMgr(void)
{
}

void CCamMgr::SetCamPosition(const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt, const D3DXVECTOR3& vUp)
{
	D3DXMATRIX matView;

	CPipeline::MakeViewSpaceMatrix(matView, vEye, vAt, vUp);

	m_matView = matView;
}

void CCamMgr::SetProjection(const float fFovY, const float fAspect, const float fNear, const float fFar)
{
	D3DXMATRIX matProj;

	CPipeline::MakeProjectionMatrix(matProj, fFovY, fAspect, fNear, fFar);

	m_matProj = matProj;
}

void CCamMgr::GetMatrix(_D3DTRANSFORMSTATETYPE eType, D3DXMATRIX& pOut)
{
	switch(eType)
	{
	case D3DTS_VIEW:
		pOut = m_matView;
		break;

	case D3DTS_PROJECTION:
		pOut = m_matProj;
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////////
void CCamMgr::SetAniCamPosition(const D3DXVECTOR3& vEye , const D3DXVECTOR3& vAt , const D3DXVECTOR3& vUp)
{
	D3DXMATRIX matView;

	CPipeline::MakeViewSpaceMatrix(matView, vEye, vAt, vUp);

	m_matAniView = matView;
}

void CCamMgr::SetAniProjection(const float fFovY , const float fAspect , const float fNear , const float fFar)
{
	D3DXMATRIX matProj;

	CPipeline::MakeProjectionMatrix(matProj, fFovY, fAspect, fNear, fFar);

	m_matAniProj = matProj;
}

void CCamMgr::GetAniMatrix(_D3DTRANSFORMSTATETYPE eType, D3DXMATRIX& pOut)
{
	switch(eType)
	{
	case D3DTS_VIEW:
		pOut = m_matAniView;
		break;

	case D3DTS_PROJECTION:
		pOut = m_matAniProj;
		break;
	}
}
