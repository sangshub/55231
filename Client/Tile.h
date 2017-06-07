#pragma once

#include "Obj.h"

class CItem;

class CTile : public CObj
{
public:
	CTile();
	~CTile();

public:
	const int GetOption() const;
	const int GetDrawId() const;
	CObj* GetObj() const;
	CItem* GetItem() const;
	const OBJID GetTileObjId() const;
	const wstring GetTileObjKey()const;
	void SetDrawId(const int iDrawId);
	void SetObj(CObj* pObj);
	void SetItem(CItem* pItem);
	void SetFindWay(const bool bFindWay);
	void ResetObj();
	void ResetItem();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void SetMatrix(const float fScale = 1.f);

private:
	void SetDarknessValue();

private:
	bool	m_bFindWay;
	int		m_iOption;
	int		m_iDrawId;
	float	m_fDarkLv;

	CObj* m_pTileObj;
	CItem* m_pItemObj;
};