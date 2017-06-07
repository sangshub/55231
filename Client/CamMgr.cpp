#include "stdafx.h"
#include "CamMgr.h"
#include "Device.h"
#include "TimeMgr.h"
#include "Pipeline.h"
#include "Rand.h"
#include "MathMgr.h"

IMPLEMENT_SINGLETON(CCamMgr)

CCamMgr::CCamMgr()
: m_bShake(false)
, m_fTime(0.f)
, m_fMaxTime(0.2f)
, m_fPow(0.2f)
, m_fSceneTime(0.f)
, m_pDevice(CDevice::GetInstance())
, m_pTimeMgr(CTimeMgr::GetInstance())
{
}

CCamMgr::~CCamMgr()
{
}

void CCamMgr::GetMatrix(const _D3DTRANSFORMSTATETYPE& eType, D3DXMATRIX& matOut)
{
	switch(eType)
	{
	case D3DTS_VIEW:
		matOut = m_matView;
		break;
	case D3DTS_PROJECTION:
		matOut = m_matProj;
		break;
	}
}

void CCamMgr::InitCamera()
{
	m_pDevice->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	//m_pDevice->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	CPipeline::MakeProjectionMatrix(m_matProj, D3DXToRadian(45.f), float(WINCX) / float(WINCY), 1.f, 50.f);
}

void CCamMgr::SetCamPosition(const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt, D3DXVECTOR3& vUp)
{
	CPipeline::MakeViewSpaceMatrix(m_matView, vEye + m_vShake, vAt, vUp);

	m_vShake = D3DXVECTOR3(0.f, 0.f, 0.f);
}

void CCamMgr::Shake(const float fPow)
{
	m_vShake = D3DXVECTOR3(float(CRand::Random(0, 2) - 2) * fPow, 0.f, float(CRand::Random(0, 2) - 2) * fPow);
}

void CCamMgr::Progress()
{
	if(m_bShake)
	{
		m_fTime += m_pTimeMgr->GetTime();

		Shake(m_fPow);

		if(m_fTime > m_fMaxTime)
		{
			m_fTime = 0.f;
			m_bShake = false;
		}
	}
}

void CCamMgr::SetShake(const float fPow, const float fTime)
{
	m_bShake = true;

	m_fPow = fPow;
	m_fMaxTime = fTime;
}

const float CCamMgr::GetShakeY() const
{
	return m_vShake.y;
}

const bool CCamMgr::ActionPlayData(ACTION& Action1, ACTION& Action2)
{
	D3DXVECTOR3 vResult; // 이동값 백터
	D3DXVECTOR3 vResultAt;

	float fTime = Action1.fTime;

	/// 위치값의 선형보간
	vResult.x = Linear(Action1.vEye.x, Action2.vEye.x, fTime, m_fSceneTime);
	vResult.y = Linear(Action1.vEye.y, Action2.vEye.y, fTime, m_fSceneTime);
	vResult.z = Linear(Action1.vEye.z, Action2.vEye.z, fTime, m_fSceneTime);

	vResultAt.x = Linear(Action1.vAt.x, Action2.vAt.x, fTime, m_fSceneTime);
	vResultAt.y = Linear(Action1.vAt.y, Action2.vAt.y, fTime, m_fSceneTime);
	vResultAt.z = Linear(Action1.vAt.z, Action2.vAt.z, fTime, m_fSceneTime);

	SetCamPosition(vResult, vResultAt, D3DXVECTOR3(0.f, 1.f, 0.f));

	m_fSceneTime += GETTIME;

	if(m_fSceneTime >= fTime)
	{
		m_fSceneTime = m_fSceneTime - fTime;

		return true;
	}
	return false;
}

const D3DXVECTOR3 CCamMgr::GetEyePos(const D3DXVECTOR3& vAt, const float& fAngleY, const float& fAngleX, const float& fCamDis)
{
	D3DXVECTOR3 vCamDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vCamPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vRight = D3DXVECTOR3(0.f, 0.f, 0.f);

	CMathMgr::MyRotationY(vCamDir, g_vLook, D3DXToRadian(fAngleY));
	D3DXVec3Normalize(&vCamDir, &vCamDir);

	vCamPos = -vCamDir;
	vCamPos *= fCamDis;
	
	D3DXVec3Cross(&vRight, &D3DXVECTOR3(0.f, 1.f, 0.f), &vCamDir);
	D3DXVec3Normalize(&vRight, &vRight);

	D3DXMATRIX matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(fAngleX));

	D3DXVec3TransformNormal(&vCamPos, &vCamPos, &matRotAxis);

	vCamPos += vAt;

	return vCamPos;
}

const float CCamMgr:: Linear(const float v0, const float v1, const float fTime , const float t)
{
	return v0 * (1.f - (t/fTime)) + v1 * (t/fTime);
}