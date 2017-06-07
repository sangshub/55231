#pragma once
#include "ui.h"

class CSlot :
	public CUI
{
public:
	CSlot(void);
	virtual ~CSlot(void);

public:
	virtual HRESULT Initialize(void);
	virtual const int Progress(void);
	virtual void Render(void);

private:
	virtual void Release(void);

public:
	void AddSlot(DWORD dwSlotType, wstring wstrItemObjKey, int iSpell = 0, int iAni = 0); //슬롯 타입 비트값, 아이템스테이트키
	void PopSlot(DWORD dwSlotType);
	ITEMSLOT CheckSlot(DWORD dwSlotType);
	void ChooseSlotType(DWORD dwSlotType);
	void ChangeSlotItem(wstring wstrItemObjKey, DWORD dwSlotType, int Spell = 0, int iAni = 0);
public:
	const wstring GetSlotItemKey(DWORD dwSlotType, int Spell = 0);
	DWORD GetSlotType(void);
	void	SetBombNum(int iBombNum);
	int		GetBombNum(void);
private:
	vector<CObj*>		m_vecEquipSlot;
	vector<CObj*>		m_vecItemSlot;
	vector<CObj*>		m_vecSpellSlot;

	DWORD				m_dwSlotType;

	int					m_iBombNum;
	

};
