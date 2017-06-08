#pragma once

#include "Include.h"

// CUnitView ���Դϴ�.
class CUnitTool;
class CDevice;
class CCamMgr;
class CMonster;
class CObj;
class CUnitView : public CView
{
	DECLARE_DYNCREATE(CUnitView)

protected:
	CUnitView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CUnitView();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif


private:
	CDevice*	m_pDevice;
	CUnitTool*	m_pUnitTool;
	//CObj*		m_pMonster;

private:
	wstring		m_wstrObjkey;

protected:
	DECLARE_MESSAGE_MAP()
public:
	void	SetObjkey(CString strObjkey);
public:
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
};


