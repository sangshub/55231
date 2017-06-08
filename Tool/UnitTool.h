#pragma once

#include "Unit_ViewDialog.h"
#include "afxwin.h"

#include "Include.h"

// CUnitTool 대화 상자입니다.
class CUnitView;
class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnitTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_UNITTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CUnitView*	m_pUnitView; // 뷰창 포인터

private:
	CUnit_ViewDialog	m_ViewDialog;

public:
	CListBox m_ListBox;

	list<IMGPATH*>		m_ImgPathlist;
	wstring				m_wstrCurSelName;
	VTXTEX*				m_pVtxTex;
	

public:
	void SetUnitView(CUnitView*	_pUnitView);
	void SetVtxTex(VTXTEX* pVtxTex);
	VTXTEX* GetVtxTex();
	wstring GetCurSelectName();

public:
	void Release(void);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLbnSelchangeList1();
};
