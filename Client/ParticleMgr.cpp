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

void CParticleMgr::CtreateParticle(const D3DXVECTOR3& vPos //���� ��ǥ
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
								   , const bool bGravity// �߷�����
								   , const bool bSizeRnd // ũ�ⷣ��
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
