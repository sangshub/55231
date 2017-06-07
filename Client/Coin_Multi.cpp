#include "StdAfx.h"
#include "Coin_Multi.h"
#include "Pipeline.h"
#include "CamMgr.h"
#include "ComboMgr.h"

CCoin_Multi::CCoin_Multi(void)
{
}

CCoin_Multi::~CCoin_Multi(void)
{
}

HRESULT CCoin_Multi::Initialize()
{
	m_wstrObjKey = TEXT("Text");
	m_wstrStateKey = TEXT("Coin_Multi");

	EffectInit();

	m_eObjId = OBJ_EFFECT;

	m_tInfo.vPos = D3DXVECTOR3(0.f, -0.48f, 0.f);

	return S_OK;
}

const int CCoin_Multi::Progress()
{
	CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(0.11f, 0.013f, 1.f), m_fAngle, m_tInfo.vPos);

	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	m_pCamMgr->GetMatrix(D3DTS_VIEW, matView);
	m_pCamMgr->GetMatrix(D3DTS_PROJECTION, matProj);
	D3DXMatrixOrthoLH(&matProj, 1, 1, 0, 1);

	memcpy(m_pConvertVtxTex, m_pVtxTex, sizeof(VTXTEX) * m_dwVtxCnt);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		//m_pConvertVtxTex[i] = m_pVtxTex[i];

		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, m_tInfo.matWorld);
		//CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, matView);

		//if(m_pConvertVtxTex[i].vPos.z < 1.f)
		//	m_pConvertVtxTex[i].vPos.z = 1.f;

		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, matProj);
	}

	m_tFrame.fFrame = (float)m_pComboMgr->GetCombo();

	if(m_bIsDelete)
		return 1;

	return 0;
}

void CCoin_Multi::Render()
{
	EffectRender();
}