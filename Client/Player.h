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
	void SetWeaponPos(); // ���� ���� ��ǥ ����
	bool BeatKeyCheck();
private:
	int FindMovePoint(); // ����Ű ��ġ�� ���⿡ ���� �˻�
	bool AttackPoint(); // ���⿡ ���� Ÿ�� �˻��Լ�
	void MonsterDamage(); // ���� ������ �ֱ� �Լ�
	void CreateAttackEffect(WEAPONID eWeaponId, int iNum = 0); // ���⿡ ���� ����Ʈ ����
	void SpellCasting(int Spell);//���� ���
	void UsingItem(void);//������ ���
	void UsingPack(void);//�� ���
	void SwitchAttack(void);//������ ���
	void UsingBomb(void); //��ź ���
	void RootingItem(CTile* pTile);
	bool ItemAbility(wstring wstr);



public:
	void MapJump(); //�� ���� ���� ī�޶� �ޱ�
	void Moving(const D3DXVECTOR3 vTarget); //���� Ʈ�� ����
	void SetCubeInfo(const CUBE	_tCube);
	void SetPlayerIndex(D3DXVECTOR3& vPos); // �ε��� ����
	void SetIsMonkey(CMonkey* pMonkey); // ������ �޶���� �Լ�
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
	DWORD		m_TempCommend; // �������� ó���ϱ� ���� ����
	float		m_fCamDistance;
	float		m_fCamAngle[ANGLE_END];
	D3DXVECTOR3	m_vCamDir;
	D3DXVECTOR3	m_vCamDis;
	D3DXVECTOR3	m_vCamMovingPos[2]; // ���� ȿ��

private: // �ൿ�� �ʿ��� ������
	float		m_fMoveDis;
	float		m_fRotAngle;
	float		m_fRotPlayer;
	float		m_fTotalAngle;
	float		m_fMJTime; // ������ �ð�
	D3DXVECTOR3	m_vMovingPos;

private: // ����Ű ��ġ�� �ִ� Ÿ���� ����
	CTile*			m_pTargetTile;
	OBJID			m_eTargetObjId;
	D3DXVECTOR3		m_vTargetTile;
	vector<D3DXVECTOR3>	m_vecWeapon[WP_END]; 
	vector<CObj*>	m_vecMonster; //���� ���ݹ����� �� ����

	CFrustum*	m_pFrustum;
	CSlot*		m_pSlot;
	CHpbar*		m_pHpBar;

	CMonkey*	m_pMonkey; // �޶���� ��Ű

public:
	CPlayer();
	~CPlayer();
};