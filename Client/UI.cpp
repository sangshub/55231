#include "stdafx.h"
#include "UI.h"

CUI::CUI(void)
:m_fSprCX(0)
,m_fSprCY(0)
{
	m_OriVtxCnt = new VTXTEX[4];
	m_pBufferMgr->GetVtxInfo(TEXT("UITex"), m_OriVtxCnt);

}

CUI::~CUI(void)
{
	::Safe_Delete_Array(m_OriVtxCnt);
}

void CUI::SetMatrix(const float fScale)
{
	CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(fScale, fScale, fScale), m_fAngle, m_tInfo.vPos);

	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	m_pCamMgr->GetMatrix(D3DTS_VIEW, matView);
	m_pCamMgr->GetMatrix(D3DTS_PROJECTION, matProj);
	D3DXMatrixOrthoLH(&matProj, m_fSprCX, m_fSprCY, 0, 1);

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
	
}