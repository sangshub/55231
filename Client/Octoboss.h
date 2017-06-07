#pragma once

#include "Obj.h"

class CTentacle;
class CBossHp;

class COctoboss : public CObj
{
public:
	COctoboss(void);
	~COctoboss(void);

public:
	const int GetBossDamage() const;
	void SetBossDamage();

public:
	virtual HRESULT Initialize(void);
	virtual const int Progress(void);
	virtual void Render(void);

private:
	void Release();

private:
	virtual void SetMatrix(const float fScale = 1.f);
	void InitTentacle();
	void BeatClock();
	void Shake();
	void Hide();
	void Appear();
	void TrapRelease();

	void TentacleSmash();
	void ChaseTentacle();
	void DDR();

private:
	bool m_bInit;
	int m_iCount;
	int m_iTileSize;
	int m_iRand[4];
	int m_iPattrenCount;
	int m_iPattrenRand;
	int m_iAppearType;
	int m_iDamage;
	float m_fShakeTime;

	DWORD m_dwBossState;
	DWORD m_dwWant;

	vector<CTentacle*> m_vecTentacle;
	vector<CObj*> m_vecFullTile;
	vector<CObj*> m_vecTrap;
	vector<CBossHp*> m_vecHp;

	D3DXVECTOR3 m_vecOldPlayerPos;
};