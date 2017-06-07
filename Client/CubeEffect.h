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
	float		m_fElasticity; // 탄성
	float		m_fUpPower;
	float		m_fTime;
	float		m_fMaxTime;
	float		m_fGravityTime;
	bool		m_bGravity;
	bool		m_bDown; //UpPower를 사용시 떨어지는 중인지 확인
	D3DXVECTOR3 m_vDamageDir;

public:
	virtual HRESULT Initialize(void);
	virtual const int Progress(void);
	virtual void Render(void);

public:
	void CubeSetting(const D3DXVECTOR3& vPos //시작 좌표
		, const float fScale //입자 크기
		, const int	  iSpeed //입자 속도
		, const int	  iMaxSpeed //입자 최대 속도
		, const float fElasticity //탄성
		, const float fTime //사라질시간
		, const float fUpPower //올라가는 힘
		, const int iDrawId //큐브텍스쳐
		, const D3DXVECTOR3& vDamageDir // 데미지 받은 방향
		, const bool bGravity = FALSE // 중력적용
		, const bool bSizeRnd = FALSE // 크기랜덤
		);

private:
	void SetActive();
	void SetMatrix(const float fScale = 1.f);
};
