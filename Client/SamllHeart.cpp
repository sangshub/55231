#include "stdafx.h"
#include "SmallHeart.h"

CSmallHeart::CSmallHeart()
{
}

CSmallHeart::~CSmallHeart()
{
}

HRESULT CSmallHeart::Initialize()
{
	m_wstrObjKey = TEXT("HP");
	m_wstrStateKey = TEXT("Normal");

	EffectInit();

	m_eObjId = OBJ_EFFECT;

	return S_OK;
}

const int CSmallHeart::Progress()
{
	SetMatrix(0.5f);

	if(m_bIsDelete)
		return 1;

	return 0;
}

void CSmallHeart::Render()
{
	EffectRender();
}

void CSmallHeart::SetDamage()
{
	m_tFrame.fFrame = 1.f;
}