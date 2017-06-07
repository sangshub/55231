#include "StdAfx.h"
#include "Diamond_Num.h"
#include "ObjMgr.h"
CDiamond_Num::CDiamond_Num(void)
:m_iDiaNum(0)
{
}

CDiamond_Num::~CDiamond_Num(void)
{
}

HRESULT CDiamond_Num::Initialize(void)
{
	m_iDiaNum = 0;
	return S_OK;
}

const int CDiamond_Num::Progress(void)
{
	m_iDiaNum  = m_pObjMgr->GetPlayer()->GetUnitInfo()->iDiamond;
	return 0;
}

void CDiamond_Num::Render(void)
{
	m_pDevice->GetDevice()->BeginScene();
	m_pDevice->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	m_pDevice->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DX_FILTER_NONE);

	const TEXINFO*	pTexInfo = m_pTextureMgr->GetTexture(TEXT("Text"),TEXT("AlphaBet"),TEXT(""),10);

	if(pTexInfo == NULL)
		return;
	D3DXMATRIX matWorld, matScale, matTrans;
	D3DXMatrixTranslation(&matTrans, 950.f, 105.f, 0.f);
	D3DXMatrixScaling(&matScale, 2.0f, 2.0f, 0.f);

	matWorld = matScale * matTrans;

	m_pDevice->GetSprite()->SetTransform(&matWorld);
	m_pDevice->GetSprite()->Draw(pTexInfo->pTexture, NULL, 
		&D3DXVECTOR3(float(pTexInfo->ImgInfo.Width>>1), float(pTexInfo->ImgInfo.Height>>1), 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));



	switch (NumberCheck(m_iDiaNum))
	{
	case 1:
		DrawNumber(m_iDiaNum, 0);
		break;
	case 2:
		DrawNumber(m_iDiaNum % 10 , 1);
		DrawNumber(m_iDiaNum / 10, 0);
		break;
	case 3:
		DrawNumber( (m_iDiaNum % 100) % 10 , 2);
		DrawNumber( (m_iDiaNum % 100) / 10, 1);
		DrawNumber(m_iDiaNum / 100, 0);
		break;
	}


	m_pDevice->GetSprite()->End();
	m_pDevice->GetDevice()->EndScene();
	//	m_pDevice->GetDevice()->Present(NULL, NULL, NULL, NULL);
}

void CDiamond_Num::Release(void)
{
}

void CDiamond_Num::DrawNumber(int iNum, int iDigit)
{

	if( (iNum > 9 && iNum <0) || iDigit> 2 )
		return;
	const TEXINFO* pTexInfo = m_pTextureMgr->GetTexture(TEXT("Number"),TEXT("Normal"),TEXT(""),iNum);

	if(pTexInfo == NULL)
		return;

	D3DXMATRIX matWorld, matTrans, matScale;

	if(iDigit == 0 )
		D3DXMatrixTranslation(&matTrans, 964.f, 105.f, 0.f);//맨앞에
	else if(iDigit == 1 )
		D3DXMatrixTranslation(&matTrans, 978.f, 105.f, 0.f);//두번째
	else if(iDigit == 2 )
		D3DXMatrixTranslation(&matTrans, 992.f, 105.f, 0.f);//세번째


	D3DXMatrixScaling(&matScale, 3.5f, 3.5f, 0.f);

	matWorld = matScale * matTrans;

	m_pDevice->GetSprite()->SetTransform(&matWorld);
	m_pDevice->GetSprite()->Draw(pTexInfo->pTexture, NULL, 
		&D3DXVECTOR3(float(pTexInfo->ImgInfo.Width>>1), float(pTexInfo->ImgInfo.Height>>1), 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

}

int CDiamond_Num::NumberCheck(int iNum)
{

	CString temp = TEXT("");

	temp.Format(TEXT("%d"),iNum);

	int len = temp.GetLength();

	return len;

}

void CDiamond_Num::SetDiamond_Num(int iNum)
{
	m_iDiaNum = iNum;

}


