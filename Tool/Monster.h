#pragma once
#include "obj.h"

class CMonster :
	public CObj
{
public:
	CMonster(void);
	virtual ~CMonster(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void);
	virtual void Render(void);

private:
	virtual void Release(void);

public:
	virtual void SetMatrix();


};
