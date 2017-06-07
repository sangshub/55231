#include "StdAfx.h"
#include "Frozen.h"
#include "Pipeline.h"
#include "CamMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "TimeMgr.h"
#include "BeatMgr.h"
#include "MathMgr.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "Device.h"
#include "Monster.h"

CFrozen::CFrozen(void)
: m_pObj(NULL)
, m_fScale(1.f)
{
}

CFrozen::~CFrozen(void)
{
}
HRESULT CFrozen::Initialize()
{
	m_dwVtxCnt = 4;

	m_pVtxTex = new VTXTEX[m_dwVtxCnt];
	m_pConvertVtxTex = new VTXTEX[m_dwVtxCnt];

	m_pBufferMgr->GetVtxInfo(TEXT("UITex"), m_pVtxTex);

	m_dwIndexCnt = 2;

	m_pIndex = new INDEX[m_dwIndexCnt];

	m_pBufferMgr->GetIndex(TEXT("UITex"), m_pIndex);

	m_wstrObjKey = TEXT("Frozen_Medium");
	m_wstrStateKey = TEXT("Normal");

	m_eObjId = OBJ_EFFECT;

	return S_OK;
}

const int CFrozen::Progress()
{
	m_tInfo.vPos = m_pObj->GetInfo()->vPos;

	SetMatrix();
	GetPlayerToDistance();

	if(m_bIsDelete)
		return 1;

	return 0;
}

void CFrozen::Render()
{
	m_pBufferMgr->SetVtxInfo(TEXT("UITex"), m_pConvertVtxTex);

	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(m_wstrObjKey, m_wstrStateKey, TEXT(""), 0);

	if(pTexInfo == NULL)
		return;

	BeginRenderState();
	SetDark(m_iDarkness);

	m_pDevice->GetDevice()->SetTexture(0, pTexInfo->pTexture);

	m_pBufferMgr->RenderBuffer(TEXT("UITex"));

	EndRenderState();
}

void CFrozen::SetMatrix()
{
	CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(m_fScale, m_fScale, m_fScale), m_fAngle, m_tInfo.vPos);

	SetBillBoard();

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

void CFrozen::SetFrozenUnit(CObj* pObj)
{
	m_pObj = pObj;
	m_fScale = ((CMonster*)pObj)->GetScale();
}