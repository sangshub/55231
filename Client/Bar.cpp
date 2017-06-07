#include "stdafx.h"
#include "Bar.h"
#include "BeatMgr.h"

CBar::CBar(void)
:m_iProgress(0)
, m_bRender(false)
{
}

CBar::~CBar(void)
{
	Release();
}

HRESULT CBar::Initialize(void)
{
	m_dwVtxCnt = 4;
	//m_pVtxTex = new VTXTEX[m_dwVtxCnt];
	m_pConvertVtxTex = new VTXTEX[m_dwVtxCnt];
	//m_pBufferMgr->GetVtxInfo(TEXT("UITex"), m_pVtxTex);

	m_dwIndexCnt = 2;
	m_pIndex = new INDEX[m_dwIndexCnt];
	m_pBufferMgr->GetIndex(TEXT("UITex"), m_pIndex);

	m_wstrObjKey = TEXT("Bar");
	m_wstrStateKey = TEXT("Normal");		//변경 레드바로 변경


	if(m_eBeatDir == BEAT_LEFT)
	{
		m_tInfo.vPos.x = -65.f;
		m_tInfo.vPos.y = -7.3f;
	}
	else
	{
		m_tInfo.vPos.x = 65.f;
		m_tInfo.vPos.y = -7.3f;

	}
	

	m_tInfo.vPos = D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_fSprCX = 115;	m_fSprCY = 19;

	m_tFrame.fCnt = 5.f;
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax= 0.f;

	m_iAlpha = 100;

	return S_OK;
}

const int CBar::Progress(void)
{
	SetMatrix();
	MoveDir();
	return EraseBar();
}

void CBar::Render(void)
{
	if(m_bRender)
		return;
	m_pBufferMgr->SetVtxInfo(TEXT("UITex"), m_pConvertVtxTex);

	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(m_wstrObjKey,m_wstrStateKey,TEXT(""),(int)m_tFrame.fFrame);

	if(pTexInfo == NULL)
		return;

	BeginRenderState(m_iAlpha);
	
	m_pDevice->GetDevice()->SetTexture(0, pTexInfo->pTexture);
	
	//m_pDevice->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pDevice->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pBufferMgr->RenderBuffer(TEXT("UITex"));

	EndRenderState();
}
void CBar::Release(void)
{
	
}

void CBar::MoveDir(void)
{
	float fBeatSpeed = 32.5f*GETTIME;

	if (m_iProgress)
		return;

	switch(m_eBeatDir)
	{
	case BEAT_LEFT:
		m_tInfo.vPos.x += fBeatSpeed;

		if(m_tInfo.vPos.x > 5.f)
		{	
			m_iProgress = 1;
		}
		else if (m_tInfo.vPos.x > -2.f)
		{
			m_bRender = 1;	//m_iAlpha = 100; 
		}
		else if(m_tInfo.vPos.x > -37.f)
		{
			if(m_iAlpha < 255)
				m_iAlpha+=4;
			if(m_iAlpha >= 255)
				m_iAlpha = 255;
		}

		break;
	case BEAT_RIGHT:
		m_tInfo.vPos.x -= fBeatSpeed;
		if(m_tInfo.vPos.x < -5.f)
		{
			m_iProgress = 1;
		}
		else if (m_tInfo.vPos.x < 2.f)
		{
			m_bRender = 1;  //m_iAlpha = 100;
		}
		else if(m_tInfo.vPos.x < 37.f)
		{
			if(m_iAlpha < 255)
				m_iAlpha+=4;
			if(m_iAlpha >= 255)
				m_iAlpha = 255;
		}
		break;
	}

	
}

void CBar::SetBeatDir(BEATDIR _eBeatDir)
{
	m_eBeatDir = _eBeatDir;
}

void CBar::SetVtxTex(VTXTEX* pVtxTex)
{
	m_pVtxTex = new VTXTEX[4];
	memcpy(m_pVtxTex, pVtxTex, sizeof(VTXTEX)*4);

}

void CBar::SetProgress(int iProgress)
{
	m_iProgress = iProgress;

}

int CBar::EraseBar()
{
	if(m_iProgress)
	{
		m_iAlpha-= 3;
	}
	if(m_iAlpha <100)
		return 1;

	return 0;
}

int CBar::GetProgress(void)
{
	return m_iProgress;
}

BEATDIR CBar::GetBeatDir(void)
{
	return m_eBeatDir;

}
