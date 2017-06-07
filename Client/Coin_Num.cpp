#include "StdAfx.h"
#include "Coin_Num.h"

#include "ObjMgr.h"
CCoin_Num::CCoin_Num(void)
:m_iCoinNum(0)
{
}

CCoin_Num::~CCoin_Num(void)
{
}

HRESULT CCoin_Num::Initialize(void)
{

	
	return S_OK;
}

const int CCoin_Num::Progress(void)
{
	m_iCoinNum = m_pObjMgr->GetPlayer()->GetUnitInfo()->iMoney;
	return 0;
}

void CCoin_Num::Render(void)
{

	m_pDevice->GetDevice()->BeginScene();
	m_pDevice->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	m_pDevice->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DX_FILTER_NONE);

	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(TEXT("Text"),TEXT("AlphaBet"),TEXT(""),10);

	if(pTexInfo == NULL)
		return;

	D3DXMATRIX matWorld, matScale, matTrans;

	D3DXMatrixTranslation(&matTrans, 950.f, 45.f, 0.f);
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);

	matWorld = matScale * matTrans;

	m_pDevice->GetSprite()->SetTransform(&matWorld);
	m_pDevice->GetSprite()->Draw(pTexInfo->pTexture, NULL, 
		&D3DXVECTOR3(float(pTexInfo->ImgInfo.Width>>1), float(pTexInfo->ImgInfo.Height>>1), 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	
	switch (NumberCheck(m_iCoinNum))
	{
	case 1:
		DrawNumber(m_iCoinNum, 0);
		break;
	case 2:
		DrawNumber(m_iCoinNum % 10 , 1);
		DrawNumber(m_iCoinNum / 10, 0);
		break;
	case 3:
		DrawNumber( (m_iCoinNum % 100) % 10 , 2);
		DrawNumber( (m_iCoinNum % 100) / 10, 1);
		DrawNumber(m_iCoinNum / 100, 0);
		break;
	}

	m_pDevice->GetSprite()->End();
	m_pDevice->GetDevice()->EndScene();
//	m_pDevice->GetDevice()->Present(NULL, NULL, NULL, NULL);
}

void CCoin_Num::Release(void)
{
	
}

void CCoin_Num::DrawNumber(int iNum, int iDigit)
{
	if( (iNum > 9 && iNum <0) || iDigit> 2 )
		return;
	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(TEXT("Number"),TEXT("Normal"),TEXT(""),iNum);

	if(pTexInfo == NULL)
		return;

	D3DXMATRIX matWorld, matTrans, matScale;

	if(iDigit == 0 )
		D3DXMatrixTranslation(&matTrans, 964.f, 45.f, 0.f);//맨앞에
	else if(iDigit == 1 )
		D3DXMatrixTranslation(&matTrans, 978.f, 45.f, 0.f);//두번째
	else if(iDigit == 2 )
		D3DXMatrixTranslation(&matTrans, 992.f, 45.f, 0.f);//세번째

	D3DXMatrixScaling(&matScale, 3.5f, 3.5f, 0.f);

	matWorld = matScale * matTrans;

	m_pDevice->GetSprite()->SetTransform(&matWorld);
	m_pDevice->GetSprite()->Draw(pTexInfo->pTexture, NULL, 
		&D3DXVECTOR3(float(pTexInfo->ImgInfo.Width>>1), float(pTexInfo->ImgInfo.Height>>1), 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	

}

int CCoin_Num::NumberCheck(int iNum)
{
	CString temp = TEXT("");

	temp.Format(TEXT("%d"),iNum);

	int len = temp.GetLength();

	return len;
}
void CCoin_Num::SetCoin_Num(int iNum)
{
	m_iCoinNum = iNum;

}