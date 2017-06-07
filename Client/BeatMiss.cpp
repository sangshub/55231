#include "stdafx.h"
#include "BeatMiss.h"

CBeatMiss::CBeatMiss(void)
: m_byAlpha(255)
{
}

CBeatMiss::~CBeatMiss(void)
{
}

HRESULT CBeatMiss::Initialize()
{
	m_wstrObjKey = TEXT("Text");
	m_wstrStateKey = TEXT("Missed");

	EffectInit();

	m_eObjId = OBJ_EFFECT;

	m_tInfo.vPos = D3DXVECTOR3(0.f, -0.3f, 0.f);

	return S_OK;
}

const int CBeatMiss::Progress()
{
	SetUIMatrix(0.02f);

	if(m_byAlpha > 50)
	{
		m_byAlpha -= 3;
		m_tInfo.vPos.y += 0.002f;
	}
	else
		m_bIsDelete = 1;

	if(m_bIsDelete)
		return 1;

	return 0;
}

void CBeatMiss::Render()
{
	EffectRender(m_byAlpha);
}