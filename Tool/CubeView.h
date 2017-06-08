#pragma once

#include "Include.h"

// CCubeView 뷰입니다.

class CCubeTool;
class CDevice;
class CCamMgr;
class CCube;
class CCubeView : public CView
{
	DECLARE_DYNCREATE(CCubeView)

protected:
	CCubeView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CCubeView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	void SetCam(void);
	void SetCubeData(const CUBE& tCube);

private:
	CDevice*	m_pDevice;
	CCamMgr*	m_pCamMgr;
	CCubeTool*	m_pCubeTool;
	CCube*		m_pCube;

private:
	bool		m_bSwitch;
	float		m_fCamDistance;
	float		m_fCamAngle[ANGLE_END];

	D3DXVECTOR3	m_vCamPos;

public:
	void CameraMove(void);

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};


