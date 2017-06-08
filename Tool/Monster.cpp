#include "StdAfx.h"
#include "Monster.h"

#include "BufferMgr.h"
#include "TextureMgr.h"
#include "MainCamMgr.h"
#include "Device.h"
#include "Pipeline.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CMonster::CMonster(void)
{
}

CMonster::~CMonster(void)
{
}

HRESULT CMonster::Initialize(void)
{
	m_dwVtxCnt = 4;
	m_dwIndexCnt = 2;

	m_pVtxTex = new VTXTEX[m_dwVtxCnt];
	m_pConvertVtxTex = new VTXTEX[m_dwVtxCnt];
	m_pIndex = new INDEX[m_dwIndexCnt];

	m_pBufferMgr->GetVtxInfo(TEXT("UnitTex"), m_pVtxTex);
	m_pBufferMgr->GetIndex(TEXT("UnitTex"), m_pIndex);

	ZeroMemory(m_fAngle, sizeof(float)*3);
	return S_OK;
}

int CMonster::Progress(void)
{
	SetMatrix();

	if(m_bIsDelete)
		return 1;

	return 0;
}

void CMonster::Render(void)
{
	m_pBufferMgr->SetVtxInfo(TEXT("UnitTex"), m_pConvertVtxTex);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(m_wstrObjKey,TEXT("Normal"));

	if(pTexInfo== NULL)
		return;

	BeginRenderState();

	m_pDevice->GetDevice()->SetTexture(0, pTexInfo->pTexture);
	m_pBufferMgr->RenderBuffer(TEXT("UnitTex"));

	EndRenderState();
}

void CMonster::Release(void)
{
	::Safe_Delete_Array(m_pVtxTex);
	::Safe_Delete_Array(m_pConvertVtxTex);
}

void CMonster::SetMatrix()
{
	CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(1.f, 1.f, 1.f), m_fAngle, m_tInfo.vPos);

	D3DXMATRIX matView;
	D3DXMATRIX matProj;

	m_pMainCamMgr->GetMatrix(D3DTS_VIEW, matView);
	m_pMainCamMgr->GetMatrix(D3DTS_PROJECTION, matProj);

	memcpy(m_pConvertVtxTex, m_pVtxTex, sizeof(VTXTEX)*m_dwVtxCnt);

	D3DXMATRIX		matBill;
	D3DXMatrixIdentity(&matBill);

	matBill = matView;
	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));

	D3DXMatrixInverse(&matBill, 0, &matBill);
	m_tInfo.matWorld = matBill * m_tInfo.matWorld;


	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, m_tInfo.matWorld);
		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, matView);

		if(m_pConvertVtxTex[i].vPos.z < 1.f)
			m_pConvertVtxTex[i].vPos.z = 1.f;

		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, matProj);
	}
}