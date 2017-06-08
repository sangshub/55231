#pragma once
#include "afxwin.h"

#include "Include.h"

class CTileMap;

// CObjTool 대화 상자입니다.

class CObjTool : public CDialog
{
	DECLARE_DYNAMIC(CObjTool)

public:
	CObjTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CObjTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_OBJTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_Listbox;

	map<CString, CImage*>		m_MapImage;
	list<SLIME*>				m_MonsterData;

	list<IMGPATH*>		m_ImgPathlist;
	wstring				m_wstrCurSelName;
	VTXTEX*				m_pVtxTex;
	CTileMap*			m_pTileMap;
	CStatic				m_Picture;


public:
	wstring GetCurSelectName();
	void CreateObj(D3DXVECTOR3 vPos);

	void SaveToMyForm(const CString& strPath);
	void LoadToMyForm(const CString& strPath);

	void DeleteObjData(D3DXVECTOR3 vPos);

public:
	void Release();

public:
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonsave1();
	afx_msg void OnBnClickedButtonload1();
};
