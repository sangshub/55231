#pragma once

#include "Effect.h"

class CFreeze :	public CEffect
{
public:
	CFreeze(void);
	~CFreeze(void);

private:
	D3DXVECTOR3 m_vPlayerPos;
	D3DXVECTOR3 m_vScale;
	D3DXVECTOR3 m_vDamagePos[9];

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	void SetPlayerMatrix(const D3DXVECTOR3& vScale);

public:
	void SetDamageInfo(const D3DXVECTOR3& vPlayerPos, const float& fAngle);
};