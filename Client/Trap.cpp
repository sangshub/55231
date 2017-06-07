#include "stdafx.h"
#include "Trap.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "Pipeline.h"
#include "Device.h"
#include "CamMgr.h"
#include "ObjMgr.h"

CTrap::CTrap()
: m_iDrawId(1)
, m_iIndex(0)
, m_iNewIndex(0)
, m_fScale(1.f)
, m_pObj(NULL)
{
}

CTrap::~CTrap()
{
}

void CTrap::SetMatrix(const float fScale)
{
	m_tInfo.vPos.y = 0.2f;

	CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(fScale, fScale, fScale), m_fAngle, m_tInfo.vPos);

	m_iIndex = m_pObjMgr->GetIndex(m_tInfo.vPos);

	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	m_pCamMgr->GetMatrix(D3DTS_VIEW, matView);
	m_pCamMgr->GetMatrix(D3DTS_PROJECTION, matProj);

	memcpy(m_pConvertVtxTex, m_pVtxTex, sizeof(VTXTEX) * m_dwVtxCnt);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		//m_pConvertVtxTex[i] = m_pVtxTex[i];

		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, m_tInfo.matWorld);
		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, matView);

		if(m_pConvertVtxTex[i].vPos.z < 1.f)
			m_pConvertVtxTex[i].vPos.z = 1.f;

		CPipeline::MyTransformCoord(m_pConvertVtxTex[i].vPos, m_pConvertVtxTex[i].vPos, matProj);
	}
}

void CTrap::TrapInit()
{
	m_dwVtxCnt = 4;

	m_pVtxTex = new VTXTEX[m_dwVtxCnt];
	m_pConvertVtxTex = new VTXTEX[m_dwVtxCnt];

	m_pBufferMgr->GetVtxInfo(TEXT("RcTex"), m_pVtxTex);

	m_dwIndexCnt = 2;

	m_pIndex = new INDEX[m_dwIndexCnt];

	m_pBufferMgr->GetIndex(TEXT("RcTex"), m_pIndex);

	m_wstrStateKey = TEXT("Normal");

	m_tFrame.fFrame = 0.f;

	m_eObjId = OBJ_TRAP;
}

void CTrap::TrapProgress()
{
	SetMatrix(m_fScale);
	GetPlayerToDistance();

	CheckObj();
	StageEndScene();
}

void CTrap::TrapRender()
{
	m_pBufferMgr->SetVtxInfo(TEXT("RcTex"), m_pConvertVtxTex);

	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(m_wstrObjKey, m_wstrStateKey, TEXT(""), m_iDrawId);

	if(pTexInfo == NULL)
		return;

	BeginRenderState();

	SetDark(m_iDarkness);

	m_pDevice->GetDevice()->SetTexture(0, pTexInfo->pTexture);

	//m_pDevice->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pDevice->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pBufferMgr->RenderBuffer(TEXT("RcTex"));

	EndRenderState();
}

const int CTrap::GetNewIndex() const
{
	return m_iNewIndex;
}

void CTrap::CheckObj()
{
	for(list<CObj*>::const_iterator iter = m_pObjMgr->GetObjList(OBJ_MONSTER)->begin();
		iter != m_pObjMgr->GetObjList(OBJ_MONSTER)->end(); ++iter)
	{
		if((*iter)->GetInfo()->vPos.x == m_tInfo.vPos.x
			&& (*iter)->GetInfo()->vPos.z == m_tInfo.vPos.z)
		{
			m_pObj = (*iter);

			return;
		}
		else
			m_pObj = NULL;
	}

	if(m_pObjMgr->GetPlayer()->GetInfo()->vPos.x == m_tInfo.vPos.x
		&& m_pObjMgr->GetPlayer()->GetInfo()->vPos.z == m_tInfo.vPos.z)
	{
		m_pObj = m_pObjMgr->GetPlayer();

		return;
	}
	else
		m_pObj = NULL;
}