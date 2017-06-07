#pragma once

#include "Monster.h"

class CYellowSkeleton : public CMonster
{
public:
	CYellowSkeleton();
	~CYellowSkeleton();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	virtual void Pattren();

private:
	bool m_bChange;
	int m_iRunaway;
};