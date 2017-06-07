#pragma once

#include "Obj.h"

class CItem : public CObj
{
public:
	CItem();
	~CItem();

public:
	void InitSetType();
	void SetType(const DWORD dwType);
	const DWORD GetType() const;

public:
	virtual HRESULT Initialize();
	virtual const int Progress();
	virtual void Render();

private:
	void Release();
	virtual void SetMatrix(const float fScale = 1.f);

private:
	void Shake();

public:
	void SetPrice(const int iPrice, const bool bDiamond = 0);
	void SetPrice(void);
	void SetSelling(const bool bSelling);
	void SetScale(const float fScale);
	void SetSellPrice();

	const bool GetisSelling() const;

	const int GetPrice() const;

private:
	bool  m_bMoving;
	bool  m_bSelling;
	int	  m_iPrice;
	int	  m_iOriPrice;
	int	  m_iRePrice;
	float m_fTime;
	float m_fCount;
	float m_fScale;
	DWORD m_dwType;

	D3DXVECTOR3 vPlayerPos;
	D3DXVECTOR3 vSlotPos;

	vector<CObj*>	m_vecPrice;
};