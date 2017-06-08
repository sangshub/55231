#pragma once

#include "Obj.h"

class CFrustum;
class CToolView;

class CMainCamObj : public CObj
{
public:
	CMainCamObj();
	~CMainCamObj();

public:
	virtual HRESULT Initialize();
	virtual int Progress();
	virtual void Render();

private:
	virtual void SetMatrix();

private:
	void SetCamera();
	void KeyInput();

private:
	float m_fCamDistance;
	float m_fCamAngle;

	CFrustum* m_pFrustum;
	CToolView*	m_pMainView;
};