#include "StdAfx.h"
#include "ItemTableMgr.h"
#include "Slot.h"
#include "Rand.h"

IMPLEMENT_SINGLETON(CItemTableMgr)

CItemTableMgr::CItemTableMgr(void)
: m_pSlot(NULL)
{
}

CItemTableMgr::~CItemTableMgr(void)
{
	Release();
}

void CItemTableMgr::Initialize()
{
	m_vecItemTable[ITEMSLOT_EQUIP].reserve(24);
	m_vecItemTable[ITEMSLOT_ITEM].reserve(12);
	m_vecItemTable[ITEMSLOT_SPELL].reserve(2);

	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Blunderbuss"));
	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Dagger"));
	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Shovel_Basic"));
	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Broad_Sword"));
	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Spear"));
	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Whip"));
	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Leather_Armor"));

	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Blood_Armor"));
	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Chain_Armor"));
	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Golden_Armor"));
	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Grass_Armor"));
	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Ki_Armor"));
	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Plate_Armor"));

	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Plate_Boots"));
	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Red_Boots"));

	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Circlet"));
	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Crown"));
	m_vecItemTable[ITEMSLOT_ITEM].push_back(TEXT("Food_Cheese"));
	m_vecItemTable[ITEMSLOT_ITEM].push_back(TEXT("Food_Chicken"));
	m_vecItemTable[ITEMSLOT_ITEM].push_back(TEXT("Food_Meat"));
	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Ring_Charisma"));
	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Ring_Mighty"));
	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Ring_Gold"));

	m_vecItemTable[ITEMSLOT_ITEM].push_back(TEXT("Bomb"));
	m_vecItemTable[ITEMSLOT_ITEM].push_back(TEXT("Bomb3"));
	m_vecItemTable[ITEMSLOT_ITEM].push_back(TEXT("Bomb"));
	m_vecItemTable[ITEMSLOT_ITEM].push_back(TEXT("Bomb3"));
	m_vecItemTable[ITEMSLOT_ITEM].push_back(TEXT("Bomb"));
	m_vecItemTable[ITEMSLOT_ITEM].push_back(TEXT("Bomb3"));
	m_vecItemTable[ITEMSLOT_ITEM].push_back(TEXT("Bomb"));
	m_vecItemTable[ITEMSLOT_ITEM].push_back(TEXT("Bomb3"));
	m_vecItemTable[ITEMSLOT_ITEM].push_back(TEXT("Food_Apple"));
	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Boots_Explorers"));
	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Helmet"));
	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Holster"));

	m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Ring_Luck"));
	m_vecItemTable[ITEMSLOT_SPELL].push_back(TEXT("Fireball"));
	m_vecItemTable[ITEMSLOT_SPELL].push_back(TEXT("Freeze"));
	//m_vecItemTable[ITEMSLOT_EQUIP].push_back(TEXT("Torch_1"));

}
const wstring CItemTableMgr::GetRandomItem()
{
	int i = 0;
	i = CRand::Random(0, 2);

	while(!m_vecItemTable[i].size())
	{
		i = CRand::Random(0, 2);
	}
	int j = CRand::Random(0, m_vecItemTable[i].size() - 1);

	wstring wstr = m_vecItemTable[i][j];
	for(vector<wstring>::iterator iter = m_vecItemTable[i].begin();
		iter != m_vecItemTable[i].end();)
	{
		if((*iter) == wstr)
		{
			iter = m_vecItemTable[i].erase(iter);

			return wstr;
		}
		else 
			++iter;
	}
	return NULL;
}
const wstring CItemTableMgr::GetRandomItem(const ITEMSLOT& eItemType)
{
	int j = CRand::Random(0, m_vecItemTable[eItemType].size()- 1);

	wstring wstr = m_vecItemTable[eItemType][j];

	for(vector<wstring>::iterator iter = m_vecItemTable[eItemType].begin();
		iter != m_vecItemTable[eItemType].end();)
	{
		if((*iter) == wstr)
		{
			iter = m_vecItemTable[eItemType].erase(iter);

			return wstr;
		}
		else 
			++iter;
	}
	return NULL;
}
const wstring CItemTableMgr::GetTableItem(const wstring& wstr)
{
	if(wstr == TEXT(""))
		return TEXT("");

	for(size_t i = 0; i < ITEMSLOT_END ; i++)
	{
		for(vector<wstring>::iterator iter = m_vecItemTable[i].begin();
			iter != m_vecItemTable[i].end(); )
		{
			if((*iter) == wstr)
			{
				iter = m_vecItemTable[i].erase(iter);

				return wstr;
			}
			else 
				++iter;
		}
	}
	
	return TEXT("");
}

void CItemTableMgr::AddItem(const wstring& wstr)
{
	ITEMSLOT eSlot;
	if(wstr == TEXT("Blunderbuss")
		|| wstr == TEXT("Broad_Sword")
		|| wstr == TEXT("Dagger")
		|| wstr == TEXT("Spear")
		|| wstr == TEXT("Whip")
		|| wstr == TEXT("Leather_Armor")
		|| wstr == TEXT("Boots_Explorers")
		|| wstr == TEXT("Helmet")
		|| wstr == TEXT("Ring_Luck")
		|| wstr == TEXT("Leather_Armor")
		|| wstr == TEXT("Circlet")
		|| wstr == TEXT("Blood_Armor")
		|| wstr == TEXT("Grass_Armor")
		|| wstr == TEXT("Chain_Armor")
		|| wstr == TEXT("Golden_Armor")
		|| wstr == TEXT("Torch_1")
		|| wstr == TEXT("Ki_Armor")
		|| wstr == TEXT("Plate_Armor")
		|| wstr == TEXT("Red_Boots")
		|| wstr == TEXT("Plate_Boots")
		|| wstr == TEXT("Crown")
		|| wstr == TEXT("Ring_Charisma")
		|| wstr == TEXT("Ring_Mighty")
		|| wstr == TEXT("Ring_Gold"))
		eSlot = ITEMSLOT_EQUIP;
	else if(wstr == TEXT("Bomb")
		|| wstr == TEXT("Bomb3")
		|| wstr == TEXT("Holster")
		|| wstr == TEXT("Food_Apple")
		|| wstr == TEXT("Food_Cheese")
		|| wstr == TEXT("Food_Chicken")
		|| wstr == TEXT("Food_Meat")
		|| wstr == TEXT("Pack"))
		eSlot = ITEMSLOT_ITEM;
	else if(wstr == TEXT("Fireball")
		|| wstr == TEXT("Freeze"))
		eSlot = ITEMSLOT_SPELL;

	m_vecItemTable[eSlot].push_back(wstr);
}
void CItemTableMgr::RefreshItemTable(void)
{
	DWORD dwSlotType =  m_pSlot->GetSlotType();
	Release();
	Initialize();

	if(dwSlotType & ITEM_ATTACK)
		GetTableItem(m_pSlot->GetSlotItemKey(ITEM_ATTACK));
	if(dwSlotType & ITEM_BODY)
		GetTableItem(m_pSlot->GetSlotItemKey(ITEM_BODY));
	if(dwSlotType & ITEM_FEET)
		GetTableItem(m_pSlot->GetSlotItemKey(ITEM_FEET));
	if(dwSlotType & ITEM_HEAD)
		GetTableItem(m_pSlot->GetSlotItemKey(ITEM_HEAD));
	if(dwSlotType & ITEM_HOLSTER)
		GetTableItem(m_pSlot->GetSlotItemKey(ITEM_HOLSTER));
	if(dwSlotType & ITEM_ITEM)
		GetTableItem(m_pSlot->GetSlotItemKey(ITEM_ITEM));
	if(dwSlotType & ITEM_MISC)
		GetTableItem(m_pSlot->GetSlotItemKey(ITEM_MISC));
	if(dwSlotType & ITEM_PACK)
		GetTableItem(m_pSlot->GetSlotItemKey(ITEM_PACK));
	if(dwSlotType & ITEM_SPELL)
		GetTableItem(m_pSlot->GetSlotItemKey(ITEM_SPELL, 1));
	if(dwSlotType & ITEM_SPELL)
		GetTableItem(m_pSlot->GetSlotItemKey(ITEM_SPELL, 2));
}

void CItemTableMgr::SetSlot(CSlot* pSlot)
{
	m_pSlot = pSlot;
}

void CItemTableMgr::Release(void)
{
	for(int i = 0; i < ITEMSLOT_END; ++i)
		vector<wstring>().swap(m_vecItemTable[i]);
}