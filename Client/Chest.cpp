#include "stdafx.h"
#include "Chest.h"
#include "Item.h"
#include "Tile.h"
#include "Device.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "ItemTableMgr.h"

CChest::CChest()
: m_wstrItemObjKey(TEXT(""))
, m_pItemTable(CItemTableMgr::GetInstance())
{
}

CChest::~CChest()
{
}

HRESULT CChest::Initialize()
{
	EtcInit();

	m_bGetItem = true;

	m_wstrItemObjKey= m_pItemTable->GetRandomItem();

	return S_OK;
}

const int CChest::Progress()
{
	EtcProgress();

	if(m_bIsDelete)
	{
		m_pDevice->SoundPlay(TEXT("Chest_Open"), 0);

		m_pObjMgr->ResetUIVtx();

		m_pObjMgr->GetTile(m_pObjMgr->GetIndex(m_tInfo.vPos))->ResetObj();
		// 여기다 아이템 생성
		CItem* pItem = (CItem*)CObjFactory<CItem>::CreateObj(m_tInfo.vPos);

		pItem->SetObjKey(m_wstrItemObjKey); // 아이템 이름 생성
		pItem->InitSetType();

		m_pObjMgr->AddObj(OBJ_ITEM, pItem);

		return 1;
	}

	return 0;
}

void CChest::Render()
{
	EtcRender();
}

void CChest::Pattren()
{
}

void CChest::SetItemObjKey(const wstring& wstrItemObjKey)
{
	m_wstrItemObjKey = wstrItemObjKey;
}

const wstring CChest::GetItemObjKey(void) const
{
	return m_wstrItemObjKey;
}