#pragma once
#include "obj.h"

class CCubeEffect :
	public CObj
{
public:
	CCubeEffect(void);
	~CCubeEffect(void);

private:
	float		m_fScale;
	float		m_fElasticity; // ź��
	float		m_fUpPower;
	float		m_fTime;
	float		m_fMaxTime;
	float		m_fGravityTime;
	bool		m_bGravity;
	bool		m_bDown; //UpPower�� ���� �������� ������ Ȯ��
	D3DXVECTOR3 m_vDamageDir;

public:
	virtual HRESULT Initialize(void);
	virtual const int Progress(void);
	virtual void Render(void);

public:
	void CubeSetting(const D3DXVECTOR3& vPos //���� ��ǥ
		, const float fScale //���� ũ��
		, const int	  iSpeed //���� �ӵ�
		, const int	  iMaxSpeed //���� �ִ� �ӵ�
		, const float fElasticity //ź��
		, const float fTime //������ð�
		, const float fUpPower //�ö󰡴� ��
		, const int iDrawId //ť���ؽ���
		, const D3DXVECTOR3& vDamageDir // ������ ���� ����
		, const bool bGravity = FALSE // �߷�����
		, const bool bSizeRnd = FALSE // ũ�ⷣ��
		);

private:
	void SetActive();
	void SetMatrix(const float fScale = 1.f);
};
