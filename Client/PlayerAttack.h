#pragma once

#include "Effect.h"

class CPlayerAttack	: public CEffect
{
public:
	CPlayerAttack(void);
	~CPlayerAttack(void);

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	void SetPlayerMatrix(const D3DXVECTOR3& vScale);
	void SetEffactScale(D3DXVECTOR3& vScale);
};