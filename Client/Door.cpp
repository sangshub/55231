#include "stdafx.h"
#include "Door.h"
#include "Tile.h"
#include "Device.h"
#include "ObjMgr.h"

CDoor::CDoor()
{
}

CDoor::~CDoor()
{
}

HRESULT CDoor::Initialize()
{
	EtcInit();

	m_bGetItem = true;

	return S_OK;
}

const int CDoor::Progress()
{
	m_wstrObjKey = TEXT("Door");

	EtcProgress();

	if(m_bIsDelete)
	{
		m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->ResetObj();

		m_pDevice->SoundPlay(TEXT("Door"), 0);

		return 1;
	}

	return 0;
}

void CDoor::Render()
{
	EtcRender();
}

void CDoor::Pattren()
{
}