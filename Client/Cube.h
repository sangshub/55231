#pragma once

#include "Obj.h"

class CCamMgr;

class CCube : public CObj
{
public:
	CCube(void);
	~CCube(void);

public:
	void SetIsMon();

public:
	virtual HRESULT Initialize(void);
	virtual const int Progress(void);
	virtual void Render(void);

public:
	void Release(void);
	void SetCubeInfo(const CUBE& _tCube);

private:
	virtual void SetMatrix(const float fScale = 1.f);

private:
	bool m_bIsMon;
};