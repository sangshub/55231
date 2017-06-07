#include "stdafx.h"
#include "Logo.h"
#include "SceneMgr.h"
#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"

CLogo::CLogo()
{
}

CLogo::~CLogo()
{
	Release();
}

HRESULT CLogo::Initialize()
{
	m_tFrame.fCnt = 2;
	m_tFrame.fMax = 5;
	m_tFrame.fFrame = 0;

	m_pDevice->SoundPlay(TEXT("Menu"), 1);

	return S_OK;
}

void CLogo::Progress()
{
	FrameMove();

	if(GetAsyncKeyState(VK_RETURN) & 0x0001)
		NextStage();
	if(GetAsyncKeyState('G') & 0x0001)
		ReStage();
}

void CLogo::Render()
{
	m_pDevice->GetDevice()->Clear(0, NULL
		, D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET
		, D3DCOLOR_ARGB(255, 0, 0, 0), 1.f, 6);

	m_pDevice->GetDevice()->BeginScene();

	m_pDevice->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	m_pDevice->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DX_FILTER_NONE);
	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(TEXT("Loading"), TEXT("Normal"), TEXT(""), (int)m_tFrame.fFrame);

	if(pTexInfo == NULL)
		return;

	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, (float)WINCX * 0.5f, (float)WINCY * 0.5f, 0.f);

	m_pDevice->GetSprite()->SetTransform(&matTrans);
	m_pDevice->GetSprite()->Draw(pTexInfo->pTexture, NULL, 
		&D3DXVECTOR3(float(pTexInfo->ImgInfo.Width>>1), float(pTexInfo->ImgInfo.Height>>1), 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
	m_pDevice->GetSprite()->SetTransform(&matTrans);

	m_pDevice->GetSprite()->End();
	m_pDevice->GetDevice()->EndScene();
	m_pDevice->GetDevice()->Present(NULL, NULL, NULL, NULL);
}

void CLogo::Release()
{
	m_pDevice->SoundStop(TEXT("Menu"));
}

void CLogo::FrameMove(void)
{
	m_tFrame.fFrame += m_tFrame.fCnt * GETTIME;

	if(m_tFrame.fFrame > m_tFrame.fMax)
		m_tFrame.fFrame = 0.f;
}

void CLogo::NextStage()
{
	m_pSceneMgr->SetSceneChange(SCENE_TUTORIAL);
}

void CLogo::ReStage()
{
	m_pSceneMgr->SetSceneChange(SCENE_LOGO);
}

void CLogo::StopBgm()
{	
}