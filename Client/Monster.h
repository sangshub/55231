#pragma once

#include "Obj.h"

class CSmallHeart;
class CComboMgr;

class CMonster : public CObj
{
public:
	CMonster();
	virtual ~CMonster();

public:
	const int GetNewIndex() const;
	const float GetScale() const;
	const DWORD GetColor() const;
	const bool GetHide() const;
	const bool GetIsFly() const;
	void SetOldIndex(const int iIndex);
	void SetHitSound();
	void SetPosMon(const D3DXVECTOR3& vPos);
	void SetTrapMove(const D3DXVECTOR3& vPos);
	void SetDamageDirection(const D3DXVECTOR3& vDamageDir);

public:
	virtual HRESULT Initialize() PURE;
	virtual const int Progress() PURE;
	virtual void Render() PURE;

private:
	void Release();

	virtual void Pattren() PURE;
	void ToShadow();
	void SetDir(const float fScale = 1.f);

protected:
	virtual void SetMatrix(const float fScale = 1.f);
	void MonsterInit();
	void MonsterProgress();
	void MonsterRender();
	void BeatClock();

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	void MoveChase();

	void Jump();
	void Move(const D3DXVECTOR3& vPos);
	void TrapMove(const D3DXVECTOR3& vPos);
	void Attack();

	void HeartRender();
	void SetScreenMode();

protected:
	bool m_bInit;
	bool m_bBeat;
	bool m_bLeft;
	bool m_bCubeBreak;
	bool m_bHide;
	bool m_bTrapMove;
	bool m_bIsFly;
	bool m_bJump;
	int m_iCount;
	int m_iOldIndex;
	int m_iNewIndex;
	int m_iAlpha;
	int m_iSceenMode;
	float m_fBaseY;
	float m_fJumpTime;
	float m_fJumpPow;
	float m_fScale;
	float m_fWidth;
	float m_fFrameSpeed;
	DWORD m_dwColor;

	CString m_strAttackSound;
	CString m_strHitSound;
	CString m_strDeathSound;
	CString m_strMoveSound;
	CString m_strInitSound;

	D3DXVECTOR3 m_vDamageDir;
	D3DXVECTOR3 m_vTarget;
	D3DXVECTOR3 m_vTrapMove;

	vector<CSmallHeart*> m_vecHp;
};