#pragma once

#include "afxwin.h"
#include "Include.h"

// CItemTool 대화 상자입니다.

class CObjMgr;
class CTextureMgr;
class CBufferMgr;
class CTileMap;

class CItemTool : public CDialog
{
	DECLARE_DYNAMIC(CItemTool)

public:
	CItemTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CItemTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ITEMTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void CreateItem(const D3DXVECTOR3& vPos);
	void SaveToMyForm(const CString& strPath);
	void LoadToMyForm(const CString& strPath);

	void DeleteItemData(D3DXVECTOR3 vPos);

private:
	void DataLoad();
	void Release();
	void SetItem();

private:
	CObjMgr* m_pObjMgr;
	CTextureMgr* m_pTextureMgr;
	CBufferMgr* m_pBufferMgr;
	CTileMap* m_pTileMap;

	CString m_strObjKey;
	CStatic m_Picture;
	CListBox m_ListBox;
	CButton m_Radio[10];
	CButton m_CheckSell;

	vector<ITEMDATA*> m_vecItem;
	list<wstring> m_ImgPathList;
	list<IMGPATH*> m_ImgPathlist;
	map<CString, CImage*> m_MapImage;

	VTXTEX*				m_pVtxTex;

public:
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
};