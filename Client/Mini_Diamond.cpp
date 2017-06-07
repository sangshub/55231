#include "stdafx.h"
#include "Mini_Diamond.h"

CMini_Diamond::CMini_Diamond(void)
{
}

CMini_Diamond::~CMini_Diamond(void)
{
}

HRESULT CMini_Diamond::Initialize()
{
	m_wstrObjKey = TEXT("Effect");
	m_wstrStateKey = TEXT("Mini_Diamond");

	/*m_tFrame.fCnt = 0;
	m_tFrame.fFrame = 0;
	m_tFrame.fMax= 0;*/
	m_tFrame.fFrame = 0;

	EffectInit();

	m_eObjId = OBJ_EFFECT;

	return S_OK;
}

const int CMini_Diamond::Progress()
{
	SetMatrix(0.2f);
	//SetFrame(0.5f);
	//FrameMove();

	return 0;
}

void CMini_Diamond::Render()
{
	EffectRender();
}