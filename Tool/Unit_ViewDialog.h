#pragma once
#include "Resource.h"


// CUnit_ViewDialog 대화 상자입니다.
class CUnitView;
class CUnit_ViewDialog : public CDialog
{
	DECLARE_DYNAMIC(CUnit_ViewDialog)

public:
	CUnit_ViewDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnit_ViewDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_UNIT_VIEWDIALOG };


public:
	CCreateContext	m_pContext;
	CUnitView*		m_pUnitView;

public:
	void SetWindowSize(RECT _rc);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
