#pragma once

#include "Obj.h"

class CMonkey;
class CFrustum;
class CSlot;
class CTile;
class CHpbar;


class CPlayer : public CObj
{
public:
	virtual HRESULT Initialize(void);
	virtual const int Progress(void);
	virtual void Render(void);

private:
	virtual void SetMatrix(const float fScale = 1.f);

private:
	void Release();
	void KeyInput();
	void PlayerState();
	void SetCamera();
	void SetPlayerDirection();
	void SetWeaponPos(); // 무기 범위 좌표 셋팅
	bool BeatKeyCheck();
private:
	int FindMovePoint(); // 방향키 위치의 무기에 따른 검색
	bool AttackPoint(); // 무기에 따른 타일 검색함수
	void MonsterDamage(); // 몬스터 데미지 주기 함수
	void CreateAttackEffect(WEAPONID eWeaponId, int iNum = 0); // 무기에 따른 이펙트 생성
	void SpellCasting(int Spell);//스펠 사용
	void UsingItem(void);//아이템 사용
	void UsingPack(void);//팩 사용
	void SwitchAttack(void);//무기집 사용
	void UsingBomb(void); //폭탄 사용
	void RootingItem(CTile* pTile);
	bool ItemAbility(wstring wstr);



public:
	void MapJump(); //맵 점프 관련 카메라 앵글
	void Moving(const D3DXVECTOR3 vTarget); //방향 트랩 무빙
	void SetCubeInfo(const CUBE	_tCube);
	void SetPlayerIndex(D3DXVECTOR3& vPos); // 인덱스 수정
	void SetIsMonkey(CMonkey* pMonkey); // 원숭이 달라붙은 함수
	void SetCommand(const DWORD dwCommand);
	void SetActionCamSwitch(bool bActionCam);
	CMonkey* GetIsMonkey();
	const float GetCamAngleY() const;
	D3DXVECTOR3* GetCamDirection();
	const DWORD GetCommand() const;
	const DWORD GetState() const;

private:
	bool		m_bActionCam;
	bool		m_bKeyTiming;
	bool		m_bJumpSetting;
	bool		m_bJumpSwap;

	int			m_iIndex;
	int			m_iAttCombo;
	DWORD		m_Commend;
	DWORD		m_TempCommend; // 보스에서 처리하기 위한 변수
	float		m_fCamDistance;
	float		m_fCamAngle[ANGLE_END];
	D3DXVECTOR3	m_vCamDir;
	D3DXVECTOR3	m_vCamDis;
	D3DXVECTOR3	m_vCamMovingPos[2]; // 점프 효과

private: // 행동에 필요한 변수들
	float		m_fMoveDis;
	float		m_fRotAngle;
	float		m_fRotPlayer;
	float		m_fTotalAngle;
	float		m_fMJTime; // 맵점프 시간
	D3DXVECTOR3	m_vMovingPos;

private: // 방향키 위치에 있는 타일의 정보
	CTile*			m_pTargetTile;
	OBJID			m_eTargetObjId;
	D3DXVECTOR3		m_vTargetTile;
	vector<D3DXVECTOR3>	m_vecWeapon[WP_END]; 
	vector<CObj*>	m_vecMonster; //무기 공격범위에 들어간 몬스터

	CFrustum*	m_pFrustum;
	CSlot*		m_pSlot;
	CHpbar*		m_pHpBar;

	CMonkey*	m_pMonkey; // 달라붙은 몽키

public:
	CPlayer();
	~CPlayer();
};