#pragma once

#include "Resource.h"


// CAniDialog ��ȭ �����Դϴ�.

class CAniView;
class CAniTool;
class CAniDialog : public CDialog
{
	DECLARE_DYNAMIC(CAniDialog)

public:
	CAniDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAniDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ANIDIALOG };

public:
	void SetAniTool(CAniTool* _pAniTool);

public:
	CCreateContext	m_pContext;
	CAniView*		m_pAniView;
	CAniTool*		m_pAniTool;

public:
	void SetWindowSize(RECT _rc);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
