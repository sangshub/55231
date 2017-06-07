#include "StdAfx.h"
#include "TextMgr.h"
#include "Device.h"
#include "TextureMgr.h"

IMPLEMENT_SINGLETON(CTextMgr)

CTextMgr::CTextMgr(void)
: m_pDevice(CDevice::GetInstance())
, m_pTextureMgr(CTextureMgr::GetInstance())
{
}

CTextMgr::~CTextMgr(void)
{
}

void CTextMgr::TextDraw(float fX, float fY, wstring wstrStateKey,float fScaleX /*= 1.f*/, float fScaleY /*= 1.f*/)
{
	m_pDevice->GetDevice()->EndScene();
	m_pDevice->GetDevice()->BeginScene();
	m_pDevice->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	m_pDevice->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DX_FILTER_NONE);

	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(TEXT("Text"),wstrStateKey,TEXT(""),0);

	if(pTexInfo == NULL)
		return;

	D3DXMATRIX matWorld, matTrans, matScale;

	D3DXMatrixTranslation(&matTrans, fX, fY, 0.f);

	D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 0.f);

	matWorld = matScale * matTrans;

	m_pDevice->GetSprite()->SetTransform(&matWorld);
	m_pDevice->GetSprite()->Draw(pTexInfo->pTexture, NULL, 
		&D3DXVECTOR3(float(pTexInfo->ImgInfo.Width>>1), float(pTexInfo->ImgInfo.Height>>1), 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pDevice->GetSprite()->End();
	m_pDevice->GetDevice()->EndScene();
	m_pDevice->GetDevice()->BeginScene();
}