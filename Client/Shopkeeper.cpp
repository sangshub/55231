#include "stdafx.h"
#include "Shopkeeper.h"

CShopkeeper::CShopkeeper()
{
}

CShopkeeper::~CShopkeeper()
{
}

HRESULT CShopkeeper::Initialize()
{
	EtcInit();

	m_bSprite = true;

	m_fScale = 1.58f;
	m_fFrameSpeed = 0.5f;

	return S_OK;
}

const int CShopkeeper::Progress()
{
	EtcProgress();

	return 0;
}

void CShopkeeper::Render()
{
	EtcRender();
}

void CShopkeeper::Pattren()
{
}