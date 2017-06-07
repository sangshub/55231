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
	void CtreateParticle(const D3DXVECTOR3& vPos //시작 좌표
		, const int& iCreateCnt //생성할 개수
		, const wstring& strBuffer //버퍼의 종류
		, const float fScale //입자 크기
		, const int iSpeed //입자 속도
		, const int iMaxSpeed //최대 속도
		, const float fElasticity //탄성
		, const float fTime //사라질시간
		, const float fUpPower //올라가는 힘
		, const DWORD dwDrawId //큐브텍스쳐
		, const D3DXVECTOR3& vDamageDir // 데미지 받은 방향
		, const bool bGravity = false // 중력적용
		, const bool bSizeRnd = false // 크기랜덤
		);

public:
	HRESULT Initialize(void);

private:
	CObjMgr*	m_pObjMgr;
};