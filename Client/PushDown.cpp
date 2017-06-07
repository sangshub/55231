#include "stdafx.h"
#include "PushDown.h"
#include "Tile.h"
#include "Monster.h"
#include "ObjMgr.h"
#include "Player.h"
#include "Device.h"

CPushDown::CPushDown()
: m_iNextIndex(0)
{
}

CPushDown::~CPushDown()
{
}

HRESULT CPushDown::Initialize()
{
	TrapInit();

	m_wstrObjKey = TEXT("Push_Down");

	return S_OK;
}

const int CPushDown::Progress()
{
	TrapProgress();

	Pattren();

	if(m_bIsDelete)
		return 1;

	return 0;
}

void CPushDown::Render()
{
	TrapRender();
}

void CPushDown::Pattren()
{
	m_iNextIndex = m_iIndex - 1;

	if(m_pObj != NULL)
	{
		if(m_pObjMgr->GetTile(m_iNextIndex)->GetObj() != NULL)
			return;

		if(m_pObj->GetObjId() == OBJ_MONSTER && !((CMonster*)m_pObj)->GetIsFly())
		{
			((CMonster*)m_pObj)->SetTrapMove(m_pObjMgr->GetTile(m_iNextIndex)->GetInfo()->vPos);
			m_pObjMgr->GetTile(m_iNextIndex)->SetObj(m_pObj);
			m_pObjMgr->GetTile(m_iIndex)->ResetObj();
			((CMonster*)m_pObj)->SetOldIndex(m_iNextIndex);
		}
		else if(m_pObj->GetObjId() == OBJ_PLAYER)
			((CPlayer*)m_pObj)->Moving(m_pObjMgr->GetTile(m_iNextIndex)->GetInfo()->vPos);
		
		m_pObj = NULL;

		if(!m_pDevice->SoundPlaying(TEXT("Trap_Move")))
			m_pDevice->SoundPlay(TEXT("Trap_Move"), 0);
	}
}