#pragma once

#include "Obj.h"

class CTile : public CObj
{
public:
	CTile();
	~CTile();

public:
	const int GetOption() const;
	const int GetDrawId() const;
	const int GetCubeCnt() const;
	const int GetObjCnt() const;
	CObj* GetTileObj() const;
	void SetDrawId(const int iDrawId);
	void SetCubeCnt(const int iCubeCnt);
	void SetObjCnt(const int iObjCnt);
	void SetStateKey(const wstring& wstrStateKey);
	void SetTileObj(CObj* pObj);

public:
	virtual HRESULT Initialize();
	virtual int Progress();
	virtual void Render();

private:
	virtual void SetMatrix();

public:
	void SetAniMatrix();

private:
	int m_iOption;
	int m_iDrawId;
	int m_iCubeCnt;
	int m_iObjCnt;

	CObj* m_pObj;
};