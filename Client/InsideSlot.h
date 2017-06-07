#pragma once
#include "ui.h"

class CInsideSlot :
	public CUI
{
public:
	CInsideSlot(void);
	virtual ~CInsideSlot(void);

public:
	virtual HRESULT Initialize(void);
	virtual const int Progress(void);
	virtual void Render(void);

private:
	virtual void Release(void);

public:
	void SetItemObjKey(wstring wstrItemObjKey);
	void SetItemStateKey(wstring wstrItemStateKey);
	void SetSlotTypeNum(ITEMSLOT eItemSlot, int iSlotNum);
	void SetSlotPos(void);
	void SetProgress(bool bProgress);
	void SetSpellCool(bool bSpell);
	void SetStartMove(bool bStart);
	void SetBombNum(int iBombNum);

	wstring& GetItemObjKey(void);
	bool	 GetSpellCool(void);
	int		 GetBombNum(void);
	
public:
	void	MoveAnimation(void);
	void	DrawNumber(void);

private:
	wstring		m_wstrItemObjKey;
	wstring		m_wstrItemStateKey;
	wstring		m_wstrRememberStateKey;
	wstring		m_wstrRememberItemObjKey;
	ITEMSLOT	m_eItemSlot;
	D3DXVECTOR3	m_vStartPos;
	bool		m_bMoveStart;

	float		m_fSceneTime;
	
	int			m_iSlotNum;
	int			m_iBombNum;
	bool		m_bSpellCool;

};
