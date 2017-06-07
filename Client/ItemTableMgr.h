#pragma once

#include "Include.h"

class CSlot;
class CItemTableMgr
{
public:
	DECLARE_SINGLETON(CItemTableMgr)

private:
	CItemTableMgr(void);

public:
	~CItemTableMgr(void);

public:
	void Initialize(void);
	
	void AddItem(const wstring& wstr);
	const wstring GetRandomItem(void);
	const wstring GetRandomItem(const ITEMSLOT& eItemType);
	const wstring GetTableItem(const wstring& wstr);

	void SetSlot(CSlot* pSlot);
	void RefreshItemTable(void);

private:
	void Release(void);

private:
	vector<wstring> m_vecItemTable[ITEMSLOT_END];

	CSlot* m_pSlot;	
};