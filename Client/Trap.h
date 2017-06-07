#pragma once

#include "Obj.h"

class CTrap : public CObj
{
public:
	CTrap();
	virtual ~CTrap();

public:
	const int GetNewIndex() const;

public:
	virtual HRESULT Initialize() PURE;
	virtual const int Progress() PURE;
	virtual void Render() PURE;

private:
	virtual void Pattren() PURE;

protected:
	virtual void SetMatrix(const float fScale = 1.f);
	void TrapInit();
	void TrapProgress();
	void TrapRender();
	void CheckObj();

protected:
	int m_iDrawId;
	int m_iIndex;
	int m_iNewIndex;
	float m_fScale;

	CObj* m_pObj;
};