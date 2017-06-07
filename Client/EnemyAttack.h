#pragma once

#include "Effect.h"

class CEnemyAttack : public CEffect
{
public:
	CEnemyAttack();
	~CEnemyAttack();

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();
};