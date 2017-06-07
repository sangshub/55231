#pragma once

#include "Obj.h"

class CObjMgr;

class CParticleMgr
{
public:
	DECLARE_SINGLETON(CParticleMgr)

private:
	CParticleMgr(void);

public:
	~CParticleMgr(void);

public:
	void CtreateParticle(const D3DXVECTOR3& vPos //���� ��ǥ
		, const int& iCreateCnt //������ ����
		, const wstring& strBuffer //������ ����
		, const float fScale //���� ũ��
		, const int iSpeed //���� �ӵ�
		, const int iMaxSpeed //�ִ� �ӵ�
		, const float fElasticity //ź��
		, const float fTime //������ð�
		, const float fUpPower //�ö󰡴� ��
		, const DWORD dwDrawId //ť���ؽ���
		, const D3DXVECTOR3& vDamageDir // ������ ���� ����
		, const bool bGravity = false // �߷�����
		, const bool bSizeRnd = false // ũ�ⷣ��
		);

public:
	HRESULT Initialize(void);

private:
	CObjMgr*	m_pObjMgr;
};