#include "StdAfx.h"
#include "ParticleMgr.h"
#include "Rand.h"
#include "ObjFactory.h"
#include "CubeEffect.h"
#include "RectEffect.h"
#include "ObjMgr.h"

IMPLEMENT_SINGLETON(CParticleMgr)

CParticleMgr::CParticleMgr(void)
{
}

CParticleMgr::~CParticleMgr(void)
{
}

HRESULT CParticleMgr::Initialize(void)
{
	m_pObjMgr = CObjMgr::GetInstance();

	return S_OK;
}

void CParticleMgr::CtreateParticle(const D3DXVECTOR3& vPos //시작 좌표
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
								   , const bool bGravity// 중력적용
								   , const bool bSizeRnd // 크기랜덤
								   )
{
	CObj* pEffect = NULL;

	if(strBuffer == TEXT("CubeTex"))
	{
		for(int i = 0; i < iCreateCnt; ++i)
		{
			pEffect = CObjFactory<CCubeEffect>::CreateObj();
			((CCubeEffect*)pEffect)->CubeSetting(vPos, fScale, iSpeed, iMaxSpeed, fElasticity, fTime, fUpPower, dwDrawId, vDamageDir, bGravity, bSizeRnd);
			m_pObjMgr->AddObj(OBJ_ETC, pEffect);
		}
	}
	else if(strBuffer == TEXT("RcCol"))
	{
		for(int i = 0; i < iCreateCnt; ++i)
		{
			pEffect = CObjFactory<CRectEffect>::CreateObj();
			((CRectEffect*)pEffect)->RectSetting(vPos, fScale, iSpeed, iMaxSpeed, fElasticity, fTime, fUpPower, dwDrawId, vDamageDir, bGravity, bSizeRnd);
			m_pObjMgr->AddObj(OBJ_ETC, pEffect);
		}
	}
}
