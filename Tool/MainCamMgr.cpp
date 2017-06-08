#include "stdafx.h"
#include "MainCamMgr.h"
#include "Device.h"
#include "Pipeline.h"

IMPLEMENT_SINGLETON(CMainCamMgr)

CMainCamMgr::CMainCamMgr()
: m_pDevice(CDevice::GetInstance())
{
}

CMainCamMgr::~CMainCamMgr()
{
}

void CMainCamMgr::GetMatrix(const _D3DTRANSFORMSTATETYPE& eType, D3DXMATRIX& matOut)
{
	switch(eType)
	{
	case D3DTS_VIEW:
		matOut = m_matView;
		break;
	case D3DTS_PROJECTION:
		matOut = m_matProj;
		break;
	}
}

void CMainCamMgr::InitCamera()
{
	m_pDevice->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	m_pDevice->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	CPipeline::MakeProjectionMatrix(m_matProj, D3DXToRadian(45.f), float(WINCX) / float(WINCY), 1.f, 50.f);
}

void CMainCamMgr::SetCamPosition(const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt, D3DXVECTOR3& vUp)
{
	CPipeline::MakeViewSpaceMatrix(m_matView, vEye, vAt, vUp);
}