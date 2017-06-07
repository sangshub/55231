#include "stdafx.h"
#include "Etc.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ObjMgr.h"
#include "Pipeline.h"
#include "CamMgr.h"

CEtc::CEtc(void)
: m_bSprite(false)
, m_bGetItem(false)
, m_fScale(1.f)
, m_fFrameSpeed(1.f)
{
}

CEtc::~CEtc(void)
{
}

void CEtc::ToShadow()
{
	D3DXVECTOR3 vDist = m_pObjMgr->GetPlayer()->GetInfo()->vPos - m_tInfo.vPos;
	float fDistance = D3DXVec3Length(&vDist);

	if(fDistance > 14.f)
		m_wstrStateKey = TEXT("Shadow");
	else
		m_wstrStateKey = TEXT("Normal");
}

void CEtc::SetMatrix(const float fScale)
{
	CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(fScale, fScale, fScale), m_fAngle, m_tInfo.vPos);

	if(m_wstrObjKey != TEXT("Door"))
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

void CEtc::EtcInit()
{
	m_dwVtxCnt = 4;

	m_pVtxTex = new VTXTEX[m_dwVtxCnt];
	m_pConvertVtxTex = new VTXTEX[m_dwVtxCnt];

	m_pBufferMgr->GetVtxInfo(TEXT("UITex"), m_pVtxTex);

	m_dwIndexCnt = 2;

	m_pIndex = new INDEX[m_dwIndexCnt];

	m_pBufferMgr->GetIndex(TEXT("UITex"), m_pIndex);

	m_wstrStateKey = TEXT("Normal");

	m_tFrame.fFrame = 0.f;

	m_eObjId = OBJ_ETC;
}

void CEtc::EtcProgress()
{
	SetMatrix(m_fScale);
	GetPlayerToDistance();
	StageEndScene();

	if(m_bSprite)
	{
		FrameMove();
		SetFrame(m_fFrameSpeed);
	}
}

void CEtc::EtcRender()
{
	m_pBufferMgr->SetVtxInfo(TEXT("UITex"), m_pConvertVtxTex);

	const TEXINFO* pTexInfo = NULL;

	if(m_bSprite)
		pTexInfo = m_pTextureMgr->GetTexture(m_wstrObjKey, m_wstrStateKey, TEXT(""), (int)m_tFrame.fFrame);
	else
		pTexInfo = m_pTextureMgr->GetTexture(m_wstrObjKey, m_wstrStateKey, TEXT(""), 0);

	if(pTexInfo == NULL)
		return;

	BeginRenderState();

	SetDark(m_iDarkness);

	ToShadow();

	m_pDevice->GetDevice()->SetTexture(0, pTexInfo->pTexture);

	m_pBufferMgr->RenderBuffer(TEXT("UITex"));

	EndRenderState();
}

const bool CEtc::GetIsItem() const
{
	return m_bGetItem;
}