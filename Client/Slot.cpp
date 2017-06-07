#include "StdAfx.h"
#include "Slot.h"

#include "ObjFactory.h"
#include "InsideSlot.h"
#include "ItemTableMgr.h"


CSlot::CSlot(void)
:m_iBombNum(1)
{
	m_dwSlotType = 0x00000000;
}

CSlot::~CSlot(void)
{
	Release();
}

HRESULT CSlot::Initialize(void)
{
	return S_OK;
}
const int CSlot::Progress(void)
{
	size_t iSize = m_vecEquipSlot.size();
	for(size_t i = 0; i < iSize; i++)
	{
		m_vecEquipSlot[i]->Progress();
	}

	iSize = m_vecItemSlot.size();
	for(size_t i = 0; i < iSize; i++)
	{
		m_vecItemSlot[i]->Progress();
	}

	iSize = m_vecSpellSlot.size();
	for(size_t i = 0; i < iSize; i++)
	{
		m_vecSpellSlot[i]->Progress();
	}
	return 0;
}
void CSlot::Render(void)
{
	size_t iSize = m_vecEquipSlot.size();
	for(size_t i = 0; i < iSize; i++)
	{
		m_vecEquipSlot[i]->Render();
	}

	iSize = m_vecItemSlot.size();
	for(size_t i = 0; i < iSize; i++)
	{
		m_vecItemSlot[i]->Render();
	}

	iSize = m_vecSpellSlot.size();
	for(size_t i = 0; i < iSize; i++)
	{
		m_vecSpellSlot[i]->Render();
	}
}
void CSlot::Release(void)
{
	for_each(m_vecEquipSlot.begin(), m_vecEquipSlot.end(), CDeleteObj());
	for_each(m_vecItemSlot.begin(), m_vecItemSlot.end(), CDeleteObj());
	for_each(m_vecSpellSlot.begin(), m_vecSpellSlot.end(), CDeleteObj());
}
void CSlot::AddSlot(DWORD dwSlotType, wstring wstrItemObjKey, int iSpell /*= 0*/,  int iAni /*= 0*/)//슬롯 타입 비트값, 아이템스테이트키
{
	if(dwSlotType  & ITEM_ITEM || dwSlotType  & ITEM_BOMB
		|| dwSlotType  & ITEM_SPELL || dwSlotType  & ITEM_HOLSTER 
		||  dwSlotType  & ITEM_PACK )
		m_pDevice->SoundPlay(TEXT("Player_Food_Pick"), 0);

	if(dwSlotType  & ITEM_ATTACK || dwSlotType  & ITEM_KEY 
		|| dwSlotType  & ITEM_MISC || dwSlotType & ITEM_HEAD) 
		m_pDevice->SoundPlay(TEXT("Player_Weapon_Pick"), 0);

	if(dwSlotType & ITEM_BODY || dwSlotType & ITEM_SHOVEL
		|| dwSlotType & ITEM_BODY || dwSlotType & ITEM_FEET)
		m_pDevice->SoundPlay(TEXT("Player_Armor"), 0);

	if ((dwSlotType & ITEM_SPELL) && ((iSpell ==1 || iSpell ==2)))
	{
		if(((CInsideSlot*)m_vecSpellSlot[iSpell-1])->GetSpellCool())
			return;

		ChangeSlotItem(wstrItemObjKey, dwSlotType, iSpell, iAni);
		return;
	}
	else if(m_dwSlotType & dwSlotType  && !(dwSlotType & ITEM_SPELL)   )
	{

		ChangeSlotItem(wstrItemObjKey, dwSlotType, iSpell, iAni);
		return;
	}
	else
	{
		m_dwSlotType |= dwSlotType;
		CObj* pObj = CObjFactory<CInsideSlot>::CreateObj();

		ChooseSlotType(dwSlotType);
		pObj->SetObjKey(TEXT("Slot"));
		pObj->SetStateKey(m_wstrStateKey);		//슬롯 타입 결정

		ITEMSLOT eItemSlot = CheckSlot(dwSlotType);

		((CInsideSlot*)pObj)->SetItemObjKey(wstrItemObjKey);
		((CInsideSlot*)pObj)->SetItemStateKey(TEXT("Normal"));	//슬롯에 들어갈 아이템
		switch(eItemSlot)
		{
		case ITEMSLOT_EQUIP:
			((CInsideSlot*)pObj)->SetSlotTypeNum(eItemSlot, m_vecEquipSlot.size() + 1);
			((CInsideSlot*)pObj)->SetSlotPos();
			if(!iAni)
				((CInsideSlot*)pObj)->SetStartMove(true);
			m_vecEquipSlot.push_back(pObj);
			break;
		case ITEMSLOT_ITEM:
			((CInsideSlot*)pObj)->SetSlotTypeNum(eItemSlot, m_vecItemSlot.size() + 1);
			((CInsideSlot*)pObj)->SetSlotPos();
			if(!iAni)
				((CInsideSlot*)pObj)->SetStartMove(true);
			if(wstrItemObjKey == TEXT("Bomb"))
				((CInsideSlot*)pObj)->SetBombNum(1);
			if(wstrItemObjKey == TEXT("Bomb3"))
			{
				((CInsideSlot*)pObj)->SetBombNum(3);
				((CInsideSlot*)pObj)->SetItemObjKey(TEXT("Bomb"));
			}
			m_vecItemSlot.push_back(pObj);
			break;
		case ITEMSLOT_SPELL:
			((CInsideSlot*)pObj)->SetSlotTypeNum(eItemSlot, m_vecSpellSlot.size() + 1);
			((CInsideSlot*)pObj)->SetSlotPos();
			m_vecSpellSlot.push_back(pObj);
			break;
		}

	}

	
}

void CSlot::ChangeSlotItem(wstring wstrItemObjKey, DWORD dwSlotType, int iSpell /*= 0*/, int iAni /*= 0*/)
{
	ITEMSLOT eItemSlot = CheckSlot(dwSlotType);
	ChooseSlotType(dwSlotType);

	switch(eItemSlot)
	{
	case ITEMSLOT_EQUIP:
		for(vector<CObj*>::iterator iter = m_vecEquipSlot.begin();
			iter != m_vecEquipSlot.end(); iter++)
		{
			if( (*iter)->GetStatekey() == m_wstrStateKey )
			{
				((CInsideSlot*)(*iter))->SetItemObjKey(wstrItemObjKey);
				if(!iAni)
					((CInsideSlot*)(*iter))->SetStartMove(true);
			}
		}
		break;
	case ITEMSLOT_ITEM:
		for(vector<CObj*>::iterator iter = m_vecItemSlot.begin();
			iter != m_vecItemSlot.end(); iter++)
		{
			if( (*iter)->GetStatekey() == m_wstrStateKey )
			{
				((CInsideSlot*)(*iter))->SetItemObjKey(wstrItemObjKey);
				if(!iAni)
					((CInsideSlot*)(*iter))->SetStartMove(true);
				if(wstrItemObjKey == TEXT("Bomb"))
					((CInsideSlot*)(*iter))->SetBombNum( ((CInsideSlot*)(*iter))->GetBombNum() + 1);
				if(wstrItemObjKey == TEXT("Bomb3"))
				{
					((CInsideSlot*)(*iter))->SetBombNum( ((CInsideSlot*)(*iter))->GetBombNum() + 3);
					((CInsideSlot*)(*iter))->SetItemObjKey(TEXT("Bomb"));
				}
				
			}
		}
		break;
	case ITEMSLOT_SPELL:
		((CInsideSlot*)m_vecSpellSlot[iSpell-1])->SetItemObjKey(wstrItemObjKey);
		if(wstrItemObjKey == TEXT("SpellCool"))
		{
			((CInsideSlot*)m_vecSpellSlot[iSpell-1])->SetSpellCool(true);
		}
		else
		{
			((CInsideSlot*)m_vecSpellSlot[iSpell-1])->SetStartMove(true);
		}
		break;
	}


}
void CSlot::PopSlot(DWORD dwSlotType)
{
	ITEMSLOT eItemSlot = CheckSlot(dwSlotType);	
	ChooseSlotType(dwSlotType);


	switch(eItemSlot)
	{
	case ITEMSLOT_EQUIP:
		for(vector<CObj*>::iterator iter = m_vecEquipSlot.begin();
			iter != m_vecEquipSlot.end(); )
		{
			if( (*iter)->GetStatekey() == m_wstrStateKey )
			{
				::Safe_Delete(*iter);
				iter = m_vecEquipSlot.erase(iter);
			}
			else
				iter++;
		}
		for (size_t i = 0; i < m_vecEquipSlot.size(); i++)
		{
			((CInsideSlot*)m_vecEquipSlot[i])->SetSlotTypeNum(eItemSlot, i + 1);
			((CInsideSlot*)m_vecEquipSlot[i])->SetSlotPos();
		}
		break;
	case ITEMSLOT_ITEM:
		for(vector<CObj*>::iterator iter = m_vecItemSlot.begin();
			iter != m_vecItemSlot.end(); )
		{
			if( (*iter)->GetStatekey() == m_wstrStateKey )
			{
				if(m_wstrStateKey == TEXT("Bomb"))
				{
					((CInsideSlot*)(*iter))->SetBombNum( ((CInsideSlot*)(*iter))->GetBombNum() - 1);
					if(( (CInsideSlot*)(*iter))->GetBombNum() != 0 )
						return;
				}
				::Safe_Delete(*iter);
				iter = m_vecItemSlot.erase(iter);
			}
				
			else
				iter++;
		}
		for (size_t i = 0; i < m_vecItemSlot.size(); i++)
		{
			((CInsideSlot*)m_vecItemSlot[i])->SetSlotTypeNum(eItemSlot, i + 1);
			((CInsideSlot*)m_vecItemSlot[i])->SetSlotPos();
		}
		break;
	case ITEMSLOT_SPELL:
		for(vector<CObj*>::iterator iter = m_vecSpellSlot.begin();
			iter != m_vecSpellSlot.end(); )
		{
			if( (*iter)->GetStatekey() == m_wstrStateKey )
			{
				::Safe_Delete(*iter);
				iter = m_vecSpellSlot.erase(iter);
			}
			else
				iter++;
		}
		for (size_t i = 0; i < m_vecSpellSlot.size(); i++)
		{
			((CInsideSlot*)m_vecSpellSlot[i])->SetSlotTypeNum(eItemSlot, i + 1);
			((CInsideSlot*)m_vecSpellSlot[i])->SetSlotPos();

				
				
		}
		break;
	}

	m_dwSlotType &= ~dwSlotType;



}
ITEMSLOT CSlot::CheckSlot(DWORD dwSlotType)
{
	ITEMSLOT eItemSlot;

	if( (dwSlotType & ITEM_ATTACK) || (dwSlotType & ITEM_SHOVEL) || (dwSlotType & ITEM_BODY)
		|| (dwSlotType & ITEM_FEET) || (dwSlotType & ITEM_HEAD) || (dwSlotType & ITEM_KEY)
		|| (dwSlotType & ITEM_TORCH) || (dwSlotType & ITEM_MISC))
		eItemSlot = ITEMSLOT_EQUIP;
	else if((dwSlotType & ITEM_BOMB) || (dwSlotType & ITEM_HOLSTER) || (dwSlotType & ITEM_ITEM)
		|| (dwSlotType & ITEM_PACK) )
		eItemSlot = ITEMSLOT_ITEM;
	else
		eItemSlot = ITEMSLOT_SPELL;


	return eItemSlot;
}
void CSlot::ChooseSlotType(DWORD dwSlotType)//슬롯의 타입을 DWORD에서 StateKey로 변경해줌
{
	if (dwSlotType & ITEM_SHOVEL)
		m_wstrStateKey = TEXT("Shovel");
	else if (dwSlotType & ITEM_ATTACK)
	{
		m_wstrStateKey = TEXT("Attack");
	}
	else if (dwSlotType & ITEM_BODY)
		m_wstrStateKey = TEXT("Body");
	else if (dwSlotType & ITEM_BOMB)
		m_wstrStateKey = TEXT("Bomb");
	else if (dwSlotType & ITEM_FEET)
		m_wstrStateKey = TEXT("Feet");
	else if (dwSlotType & ITEM_HEAD)
		m_wstrStateKey = TEXT("Head");
	else if (dwSlotType & ITEM_HOLSTER)
		m_wstrStateKey = TEXT("Holster");
	else if (dwSlotType & ITEM_ITEM)
		m_wstrStateKey = TEXT("Item");
	else if (dwSlotType & ITEM_KEY)
		m_wstrStateKey = TEXT("Key");
	else if (dwSlotType & ITEM_MISC)
		m_wstrStateKey = TEXT("Misc");
	else if (dwSlotType & ITEM_PACK)
		m_wstrStateKey = TEXT("Pack");
	else if (dwSlotType & ITEM_REROAD)
		m_wstrStateKey = TEXT("Reroad");
	else if (dwSlotType & ITEM_TORCH)
		m_wstrStateKey = TEXT("Torch");
	else if (dwSlotType & ITEM_MISC)
		m_wstrStateKey = TEXT("Misc");
	else if (dwSlotType & ITEM_SPELL)
	{
		if(m_vecSpellSlot.size() == 0)
			m_wstrStateKey = TEXT("SpellA");
		else
			m_wstrStateKey = TEXT("SpellB");
	}
	else
		return;



}
const wstring CSlot::GetSlotItemKey(DWORD dwSlotType, int Spell /*= 0*/)
{
	wstring wstrItemObjKey = TEXT("");
	ITEMSLOT eItemSlot = CheckSlot(dwSlotType);	
	ChooseSlotType(dwSlotType);

	switch(eItemSlot)
	{
	case ITEMSLOT_EQUIP:
		for(vector<CObj*>::iterator iter = m_vecEquipSlot.begin();
			iter != m_vecEquipSlot.end();	iter++ )
		{
			if( (*iter)->GetStatekey() == m_wstrStateKey )
			{	
				wstrItemObjKey = ((CInsideSlot*)(*iter))->GetItemObjKey();
			}
		}
		break;
	case ITEMSLOT_ITEM:
		for(vector<CObj*>::iterator iter = m_vecItemSlot.begin();
			iter != m_vecItemSlot.end();iter++ )
		{
			if( (*iter)->GetStatekey() == m_wstrStateKey )
			{
				wstrItemObjKey = ((CInsideSlot*)(*iter))->GetItemObjKey();
			}
		}
		break;
	case ITEMSLOT_SPELL:
		if(m_vecSpellSlot.size()==1 &&Spell ==2)
			return wstrItemObjKey;
		wstrItemObjKey = ((CInsideSlot*)m_vecSpellSlot[Spell-1])->GetItemObjKey();
		break;
	}

	return wstrItemObjKey;
}

DWORD CSlot::GetSlotType(void)
{
	return m_dwSlotType;
}

void CSlot::SetBombNum(int iBombNum)
{
	m_iBombNum = iBombNum;
}

int CSlot::GetBombNum(void)
{
	return m_iBombNum;

}
