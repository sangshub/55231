#include "stdafx.h"
#include "EnemyAttack.h"

CEnemyAttack::CEnemyAttack()
{
}

CEnemyAttack::~CEnemyAttack()
{
}

HRESULT CEnemyAttack::Initialize()
{
	m_wstrObjKey = TEXT("EnemyAttack");
	m_wstrStateKey = TEXT("Normal");

	EffectInit();

	m_eObjId = OBJ_EFFECT;

	return S_OK;
}

const int CEnemyAttack::Progress()
{
	SetMatrix();
	FrameMove();
	SetFrame();

	if(m_tFrame.fFrame > m_tFrame.fMax - 0.5f)
		return 1;

	return 0;
}

void CEnemyAttack::Render()
{
	EffectRender();
}