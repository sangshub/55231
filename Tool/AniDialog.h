#pragma once

#include "Resource.h"


// CAniDialog 대화 상자입니다.

class CAniView;
class CAniTool;
class CAniDialog : public CDialog
{
	DECLARE_DYNAMIC(CAniDialog)

public:
	CAniDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAniDialog();

// 대화 상자 데이터입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
