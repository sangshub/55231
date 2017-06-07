#pragma once

#include "Etc.h"

class CItemTableMgr;

class CChest : public CEtc
{
public:
	CChest();
	~CChest();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();

public:
	void SetItemObjKey(const wstring& wstrItemObjKey);
	const wstring GetItemObjKey(void) const;

private:
	CItemTableMgr* m_pItemTable;
	wstring	m_wstrItemObjKey;
};