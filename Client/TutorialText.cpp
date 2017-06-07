#include "stdafx.h"
#include "TutorialText.h"
#include "Player.h"
#include "Device.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "Pipeline.h"
#include "CamMgr.h"

CTutorialText::CTutorialText(void)
: m_iImageNum(0)
, m_fSizeX(0.f)
, m_fSizeY(0.f)
{
}

CTutorialText::~CTutorialText(void)
{
}

HRESULT CTutorialText::Initialize()
{
	m_wstrObjKey = TEXT("Text");
	m_wstrStateKey = TEXT("Tutorial");

	EffectInit();

	m_eObjId = OBJ_EFFECT;

	return S_OK;
}

const int CTutorialText::Progress()
{
	float f = D3DXToDegree( ((CPlayer*)m_pObjMgr->GetPlayer())->GetCamAngleY());
	if((D3DXToDegree( ((CPlayer*)m_pObjMgr->GetPlayer())->GetCamAngleY()) > 260.f &&
		D3DXToDegree( ((CPlayer*)m_pObjMgr->GetPlayer())->GetCamAngleY()) < 280.f) ||
		(D3DXToDegree(((CPlayer*)m_pObjMgr->GetPlayer())->GetCamAngleY()) > 80.f &&
		D3DXToDegree(((CPlayer*)m_pObjMgr->GetPlayer())->GetCamAngleY()) < 100.f) ||
		(D3DXToDegree( ((CPlayer*)m_pObjMgr->GetPlayer())->GetCamAngleY()) < -260.f &&
		D3DXToDegree( ((CPlayer*)m_pObjMgr->GetPlayer())->GetCamAngleY()) > -280.f) ||
		(D3DXToDegree(((CPlayer*)m_pObjMgr->GetPlayer())->GetCamAngleY()) < -80.f &&
		D3DXToDegree(((CPlayer*)m_pObjMgr->GetPlayer())->GetCamAngleY()) > -100.f)
		)
		TextSetMatrix(m_fSizeY, m_fSizeX);
	else
		TextSetMatrix(m_fSizeX, m_fSizeY);

	GetPlayerToDistance();

	return 0;
}

void CTutorialText::Render()
{
	if(!m_bStageEnd)
		TextRender(m_iImageNum);
}

void CTutorialText::TextRender(const int iImageNum)
{
	m_pBufferMgr->SetVtxInfo(TEXT("UITex"), m_pConvertVtxTex);

	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(m_wstrObjKey, m_wstrStateKey, TEXT(""), iImageNum);

	if(pTexInfo == NULL)
		return;
	BeginRenderState();
	SetDark(m_iDarkness);
	m_pDevice->GetDevice()->SetTexture(0, pTexInfo->pTexture);
	m_pBufferMgr->RenderBuffer(TEXT("UITex"));
	EndRenderState();
}

void CTutorialText::TextSetMatrix(const float fX, const float fZ)
{
	m_tInfo.vPos.y = 0.3f;
	CPipeline::MakeWorldMatrix(m_tInfo.matWorld, D3DXVECTOR3(fX, 0.f, fZ), m_fAngle, m_tInfo.vPos);
	SetBillBoard();
	D3DXMATRIX matView, matProj, matRot;

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

void CTutorialText::SetTextImageNumSize(const int iImageNum, const float fSizeX, const float fSizeY)
{
	m_iImageNum = iImageNum;
	m_fSizeX = fSizeX;
	m_fSizeY = fSizeY;
}