#pragma once

#include "Include.h"

class CDevice;
class CTimeMgr;

class CCamMgr
{
public:
	DECLARE_SINGLETON(CCamMgr)

private:
	CCamMgr(void);

public:
	~CCamMgr(void);

public:
	void GetMatrix(const _D3DTRANSFORMSTATETYPE& eType, D3DXMATRIX& matOut);

public:
	void InitCamera();
	void SetCamPosition(const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt, D3DXVECTOR3& vUp);
	void SetShake(const float fPow = 0.2f, const float fTime = 0.2f);
	const float GetShakeY() const;

public:
	void Progress();

private:
	void Shake(const float fPow);

public: // 액션카메라를 위한 함수들
	const bool ActionPlayData(ACTION& Action1, ACTION& Action2);
	const float Linear(const float v0, const float v1, const float fTime, const float t);
	const D3DXVECTOR3 GetEyePos(const D3DXVECTOR3& vAt, const float& fAngleY, const float& fAngleX, const float& fCamDis);

private:
	bool m_bShake;
	float m_fTime;
	float m_fMaxTime;
	float m_fSceneTime; // 씬의 진행 시간
	float m_fPow;

	CDevice* m_pDevice;
	CTimeMgr* m_pTimeMgr;

	D3DXVECTOR3 m_vShake;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
};