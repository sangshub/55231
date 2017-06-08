#pragma once
#include "Resource.h"


// CViewDialog 대화 상자입니다.

class CCubeView;
class CViewDialog : public CDialog
{
	DECLARE_DYNAMIC(CViewDialog)

public:
	CViewDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CViewDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VIEWDIALOG };

public:
	CCreateContext	m_pContext;
	CCubeView*	m_pCubeView;

public:
	void SetWindowSize(RECT _rc);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};
