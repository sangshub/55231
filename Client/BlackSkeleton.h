#pragma once

#include "Monster.h"

class CBlackSkeleton : public CMonster
{
public:
	CBlackSkeleton();
	~CBlackSkeleton();

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