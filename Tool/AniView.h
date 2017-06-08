#pragma once

#include "Include.h"


// CAniView 뷰입니다.

class CAniTool;
class CCube;
class CDevice;
class CCamMgr;
class CTileMap;
class CTile;
class CAniView : public CView
{
	DECLARE_DYNCREATE(CAniView)

protected:
	CAniView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CAniView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	void SetAniTool(CAniTool* _pAniTool);
	void CameraMove(void);
	void SetCam(void);

private:
	CTile*		m_pTile;
	CDevice*	m_pDevice;
	CAniTool*	m_pAniTool;
	CTileMap*	m_pTileMap;
	CCamMgr*	m_pCamMgr;
	vector<CCube*>*		m_pAnimation;

private:
	bool		m_bSwitch;
	float		m_fCamDistance;
	float		m_fCamAngle[ANGLE_END];

	D3DXVECTOR3	m_vCamPos;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};


