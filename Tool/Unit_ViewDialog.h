#pragma once
#include "Resource.h"


// CUnit_ViewDialog ��ȭ �����Դϴ�.
class CUnitView;
class CUnit_ViewDialog : public CDialog
{
	DECLARE_DYNAMIC(CUnit_ViewDialog)

public:
	CUnit_ViewDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUnit_ViewDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_UNIT_VIEWDIALOG };


public:
	CCreateContext	m_pContext;
	CUnitView*		m_pUnitView;

public:
	void SetWindowSize(RECT _rc);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
