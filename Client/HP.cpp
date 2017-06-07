#include "StdAfx.h"
#include "HP.h"

CHP::CHP(void)
:m_iHpNum(0)
,m_iMaxHpNum(0)
,m_iCount(0)
,m_iPlayerHp(0)
{
}

CHP::~CHP(void)
{
	Release();
}

HRESULT CHP::Initialize(void)
{
	m_dwVtxCnt = 4;
	
	m_pConvertVtxTex = new VTXTEX[m_dwVtxCnt];
	//m_pBufferMgr->GetVtxInfo(TEXT("UITex"), m_pVtxTex);

	m_dwIndexCnt = 2;
	m_pIndex = new INDEX[m_dwIndexCnt];
	m_pBufferMgr->GetIndex(TEXT("UITex"), m_pIndex);

	m_wstrObjKey = TEXT("HP");
	m_wstrStateKey = TEXT("Normal");

	if(m_iHpNum ==1)
	{
		m_tInfo.vPos = D3DXVECTOR3(14.5f, 15.5f, 0.f);
		m_fSprCX = 43;	m_fSprCY = 35;
	}
	else if (m_iHpNum == 2)
	{
		m_tInfo.vPos = D3DXVECTOR3(12.2f, 15.5f, 0.f);
		m_fSprCX = 43;	m_fSprCY = 35;
	}
	else if (m_iHpNum == 3)
	{
		m_tInfo.vPos = D3DXVECTOR3(9.9f, 15.5f, 0.f);
		m_fSprCX = 43;	m_fSprCY = 35;
	}
	else if (m_iHpNum == 4)
	{
		m_tInfo.vPos = D3DXVECTOR3(7.6f, 15.5f, 0.f);
		m_fSprCX = 43;	m_fSprCY = 35;
	}
	else if (m_iHpNum == 5)
	{
		m_tInfo.vPos = D3DXVECTOR3(5.3f, 15.5f, 0.f);
			m_fSprCX = 43;	m_fSprCY = 35;
	}

	m_tFrame.fCnt = 5.f;
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax= 0.f;

	m_iFrame = 0;
	return S_OK;
}

const int CHP::Progress(void)
{
	if((int)m_tFrame.fFrame == m_iHpNum-1 && m_iFrame != 1)
	{
		SetMatrix(1.2f);
	}
	else
		SetMatrix(1.0f);
	m_iFrame = ChooseHpState();
	SetFrame();
	FrameMove();
	return 0;
}

void CHP::Render(void)
{
	//m_pDevice->SetRenderState(D3DRS_ZENABLE, false);
	m_pBufferMgr->SetVtxInfo(TEXT("UITex"), m_pConvertVtxTex);

	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(m_wstrObjKey,m_wstrStateKey,TEXT(""),(int)m_iFrame);

	if(pTexInfo == NULL)
		return;

	BeginRenderState();

	m_pDevice->GetDevice()->SetTexture(0, pTexInfo->pTexture);

	//m_pDevice->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pDevice->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pBufferMgr->RenderBuffer(TEXT("UITex"));
	//m_pDevice->SetRenderState(D3DRS_ZENABLE, true);

	EndRenderState();
}

void CHP::Release(void)
{

}

void CHP::SetHpNumber(int hpNum)
{
	m_iHpNum = hpNum;

}

void CHP::SetVtxTex(VTXTEX* pVtxTex)
{
	m_pVtxTex = new VTXTEX[4];
	memcpy(m_pVtxTex, pVtxTex, sizeof(VTXTEX)*4);

}

void CHP::SetMaxHpNum(int iMaxHpNum)
{
	m_iMaxHpNum = iMaxHpNum;

}

void CHP::SetPlayerHp(int iPlayerHp)
{
	m_iPlayerHp = iPlayerHp;

}
int CHP::ChooseHpState(void)
{

 	if(m_iPlayerHp >= m_iHpNum * 2)
		return 0;
	else if( ((m_iHpNum * 2) - m_iPlayerHp) == 1)
		return 2;
	else
		return 1;
}
