#pragma once

#include "Obj.h"

#include "Pipeline.h"
#include "CamMgr.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"


class CUI :	public CObj
{
public:
	CUI(void);
	virtual ~CUI(void);

public:
	virtual HRESULT Initialize(void)PURE;
	virtual const int Progress(void)PURE;
	virtual void Render(void)PURE;

private:
	virtual void Release(void)PURE;

protected:
	virtual void SetMatrix(const float fScale = 1.f);

protected:
	float m_fSprCX;
	float m_fSprCY;

	VTXTEX*			m_OriVtxCnt;

	


};