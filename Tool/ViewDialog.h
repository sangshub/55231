#pragma once
#include "Resource.h"


// CViewDialog ��ȭ �����Դϴ�.

class CCubeView;
class CViewDialog : public CDialog
{
	DECLARE_DYNAMIC(CViewDialog)

public:
	CViewDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CViewDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VIEWDIALOG };

public:
	CCreateContext	m_pContext;
	CCubeView*	m_pCubeView;

public:
	void SetWindowSize(RECT _rc);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};
