#include "StdAfx.h"
#include "InsideSlot.h"

CInsideSlot::CInsideSlot(void)
:m_iSlotNum(0)
,m_bSpellCool(0)
,m_vStartPos(D3DXVECTOR3(WINCX/2.f, WINCY/2.f, 0.f))
,m_bMoveStart(false)
,m_fSceneTime(0)
,m_iBombNum(0)
{
}

CInsideSlot::~CInsideSlot(void)
{
}

HRESULT CInsideSlot::Initialize(void)
{
	m_wstrRememberStateKey = TEXT("");
	m_wstrRememberItemObjKey = TEXT("");
	m_tFrame.fCnt = 0.f;
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax= 0.f;


	return S_OK;
}

const int CInsideSlot::Progress(void)
{
	if(m_bSpellCool)
	{

		m_tFrame.fCnt = 2.f;
		//m_tFrame.fFrame = 0.f;
		m_tFrame.fMax= 8.f;
		FrameMove(0);

		if (m_tFrame.fFrame > 7.5)
		{
			m_wstrStateKey = m_wstrRememberStateKey;
			m_wstrItemObjKey = m_wstrRememberItemObjKey;
			m_bSpellCool = 0;
		}

	}
	else
	{
		m_tFrame.fCnt = 0.f;
		m_tFrame.fFrame = 0.f;
		m_tFrame.fMax= 0.f;
		m_wstrRememberStateKey  = m_wstrStateKey;
		m_wstrRememberItemObjKey  = m_wstrItemObjKey;
	

	}
	
	if(m_bMoveStart)
	{
		m_fSceneTime += GETTIME;
		MoveAnimation();
	}
	return 0;
}

void CInsideSlot::Render(void)
{
	m_pDevice->GetDevice()->BeginScene();
	m_pDevice->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	m_pDevice->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DX_FILTER_NONE);

	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(m_wstrObjKey,m_wstrStateKey,TEXT(""),0);

	if(pTexInfo == NULL)
		return;

	D3DXMATRIX matWorld, matTrans, matScale;
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);

	matWorld = matScale * matTrans;

	m_pDevice->GetSprite()->SetTransform(&matWorld);
	m_pDevice->GetSprite()->Draw(pTexInfo->pTexture, NULL, 
		&D3DXVECTOR3(float(pTexInfo->ImgInfo.Width>>1), float(pTexInfo->ImgInfo.Height>>1), 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));//슬롯출력

	if(m_bSpellCool)
		pTexInfo = m_pTextureMgr->GetTexture(m_wstrItemObjKey,m_wstrItemStateKey,TEXT(""),(int)m_tFrame.fFrame );
	else
		pTexInfo = m_pTextureMgr->GetTexture(m_wstrItemObjKey,m_wstrItemStateKey,TEXT(""),0);

	if(pTexInfo == NULL)
		return;

	if(m_bMoveStart)
	{
		if(m_eItemSlot == ITEMSLOT_EQUIP)
			D3DXMatrixTranslation(&matTrans, m_vStartPos.x, m_vStartPos.y+5.f, 0.f);
		else if(m_eItemSlot == ITEMSLOT_ITEM)
			D3DXMatrixTranslation(&matTrans, m_vStartPos.x, m_vStartPos.y-5.f, 0.f);
		else
			D3DXMatrixTranslation(&matTrans, m_vStartPos.x, m_vStartPos.y-5.f, 0.f);

	}
	else
	{
		if(m_eItemSlot == ITEMSLOT_EQUIP)
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y+5.f, 0.f);
		else if(m_eItemSlot == ITEMSLOT_ITEM)
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y-5.f, 0.f);
		else
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y-5.f, 0.f);
	}
	D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 0.f);

	matWorld = matScale * matTrans;

	m_pDevice->GetSprite()->SetTransform(&matWorld);
	m_pDevice->GetSprite()->Draw(pTexInfo->pTexture, NULL, 
		&D3DXVECTOR3(float(pTexInfo->ImgInfo.Width>>1), float(pTexInfo->ImgInfo.Height>>1), 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));//아이템 출력

	if(m_wstrItemObjKey == TEXT("Bomb"))
	{
		DrawNumber();
	}
	


	m_pDevice->GetSprite()->End();
	m_pDevice->GetDevice()->EndScene();
	
}

void CInsideSlot::Release(void)
{
	
}

void CInsideSlot::SetItemStateKey(wstring wstrItemStateKey)
{
	m_wstrItemStateKey= wstrItemStateKey;

}

void CInsideSlot::SetItemObjKey(wstring wstrItemObjKey)
{
	m_wstrItemObjKey = wstrItemObjKey;

}

void CInsideSlot::SetSlotTypeNum(ITEMSLOT eItemSlot, int iSlotNum)
{
	m_eItemSlot = eItemSlot;
	m_iSlotNum = iSlotNum;

}
void CInsideSlot::SetSlotPos(void)
{
	if(m_iSlotNum == 0)
		return;

	switch(m_eItemSlot)
	{
	case ITEMSLOT_EQUIP:
		if(m_iSlotNum == 1)
			m_tInfo.vPos = D3DXVECTOR3(40.f, 43.f, 0.f); 
		else if(m_iSlotNum == 2)
			m_tInfo.vPos = D3DXVECTOR3(105.f, 43.f, 0.f); 
		else if(m_iSlotNum == 3)
			m_tInfo.vPos = D3DXVECTOR3(170.f, 43.f, 0.f); 
		else if(m_iSlotNum == 4)
			m_tInfo.vPos = D3DXVECTOR3(235.f, 43.f, 0.f); 
		else if(m_iSlotNum == 5)
			m_tInfo.vPos = D3DXVECTOR3(300.f, 43.f, 0.f); 
		else if(m_iSlotNum == 6)
			m_tInfo.vPos = D3DXVECTOR3(365.f, 43.f, 0.f); 
		else if(m_iSlotNum == 7)
			m_tInfo.vPos = D3DXVECTOR3(430.f, 43.f, 0.f); 
		else if(m_iSlotNum == 8)
			m_tInfo.vPos = D3DXVECTOR3(495.f, 43.f, 0.f); 

		break;
	case ITEMSLOT_ITEM:
		if(m_iSlotNum == 1)
			m_tInfo.vPos = D3DXVECTOR3(40.f, 125.f, 0.f); 
		else if(m_iSlotNum == 2)
			m_tInfo.vPos = D3DXVECTOR3(40.f, 220.f, 0.f); 
		else if(m_iSlotNum == 3)
			m_tInfo.vPos = D3DXVECTOR3(40.f, 315.f, 0.f); 
		else if(m_iSlotNum == 4)
			m_tInfo.vPos = D3DXVECTOR3(40.f, 410.f, 0.f);
		break;
	case ITEMSLOT_SPELL:
		if(m_iSlotNum == 1)
			m_tInfo.vPos = D3DXVECTOR3(975.f, 200.f, 0.f); 
		else if(m_iSlotNum == 2)
			m_tInfo.vPos = D3DXVECTOR3(975.f, 295.f, 0.f); 
		break;
	}
}

wstring& CInsideSlot::GetItemObjKey(void)
{
	return m_wstrItemObjKey;

}

void CInsideSlot::SetSpellCool(bool bSpell)
{
	m_bSpellCool = bSpell;
}

bool CInsideSlot::GetSpellCool(void)
{
	return m_bSpellCool;
}

void CInsideSlot::MoveAnimation(void)
{

	if(m_fSceneTime < 0.3f)
	{
		m_vStartPos.x = Linear(WINCX/2.f, m_tInfo.vPos.x, 0.3f,m_fSceneTime );
		m_vStartPos.y = Linear(WINCY/2.f, m_tInfo.vPos.y, 0.3f, m_fSceneTime);
		m_vStartPos.z = Linear(0, m_tInfo.vPos.z, 0.3f, m_fSceneTime);
	}
	else
	{
		m_bMoveStart = false;
		m_vStartPos = m_tInfo.vPos;
		m_fSceneTime = 0.f;
	}


}

void CInsideSlot::SetStartMove(bool bStart)
{
	m_bMoveStart = bStart;
}

void CInsideSlot::SetBombNum(int iBombNum)
{
	m_iBombNum = iBombNum;

}
int CInsideSlot::GetBombNum(void)
{
	return m_iBombNum;
}

void CInsideSlot::DrawNumber(void)
{
	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(TEXT("Text"),TEXT("AlphaBet"),TEXT(""),10);

	if(pTexInfo == NULL)
		return;

	D3DXMATRIX matWorld, matScale, matTrans;
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x+6.f, m_tInfo.vPos.y+5.f, 0.f);
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);

	matWorld = matScale * matTrans;

	m_pDevice->GetSprite()->SetTransform(&matWorld);
	m_pDevice->GetSprite()->Draw(pTexInfo->pTexture, NULL, 
		&D3DXVECTOR3(float(pTexInfo->ImgInfo.Width>>1), float(pTexInfo->ImgInfo.Height>>1), 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));


	pTexInfo = m_pTextureMgr->GetTexture(TEXT("Text"),TEXT("AlphaBet"),TEXT(""),m_iBombNum);

	if(pTexInfo == NULL)
		return;

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 10.f+6.f, m_tInfo.vPos.y+5.f,0.f);
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);

	matWorld = matScale * matTrans;

	m_pDevice->GetSprite()->SetTransform(&matWorld);
	m_pDevice->GetSprite()->Draw(pTexInfo->pTexture, NULL, 
		&D3DXVECTOR3(float(pTexInfo->ImgInfo.Width>>1), float(pTexInfo->ImgInfo.Height>>1), 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

}

