#include "stdafx.h"
#include "Stairs.h"
#include "ObjMgr.h"

CStairs::CStairs()
{
}

CStairs::~CStairs()
{
}

HRESULT CStairs::Initialize()
{
	TrapInit();

	m_iDrawId = 0;

	return S_OK;
}

const int CStairs::Progress()
{
	TrapProgress();

	if(m_pObj && m_pObj->GetObjId() == OBJ_PLAYER)
		m_pObjMgr->SetStageEnd(true);

	if(m_bIsDelete)
		return 1;

	return 0;
}

void CStairs::Render()
{
	TrapRender();
}

void CStairs::Pattren()
{
}