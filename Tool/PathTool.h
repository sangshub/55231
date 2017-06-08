#pragma once

#include "Include.h"
#include "afxwin.h"

// CPathTool ��ȭ �����Դϴ�.

class CPathTool : public CDialog
{
	DECLARE_DYNAMIC(CPathTool)

public:
	CPathTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathTool();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PATHTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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