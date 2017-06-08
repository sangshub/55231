#pragma once

#include "Include.h"
#include "afxwin.h"

// CPathTool 대화 상자입니다.

class CPathTool : public CDialog
{
	DECLARE_DYNAMIC(CPathTool)

public:
	CPathTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PATHTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedReset();

private:
	void Release();

private:
	CListBox m_ListBox;
	list<IMGPATH*> m_ImgPathList;
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
};