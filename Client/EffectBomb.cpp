#include "stdafx.h"
#include "EffectBomb.h"

CEffectBomb::CEffectBomb(void)
{
}

CEffectBomb::~CEffectBomb(void)
{
}

HRESULT CEffectBomb::Initialize()
{
	m_wstrObjKey = TEXT("Effect");
	m_wstrStateKey = TEXT("KaBoom");

	EffectInit();

	m_eObjId = OBJ_EFFECT;

	return S_OK;
}

const int CEffectBomb::Progress()
{
	SetMatrix();
	SetFrame(1.f);
	FrameMove();

	if(m_tFrame.fFrame > m_tFrame.fMax - 0.5f)

		return 1;

	return 0;
}

void CEffectBomb::Render()
{
	EffectRender();	
}