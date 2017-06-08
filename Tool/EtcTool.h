#pragma once

#include "afxwin.h"
#include "Include.h"

// CEtcTool ��ȭ �����Դϴ�.

class CTextureMgr;
class CBufferMgr;
class CObjMgr;
class CTileMap;

class CEtcTool : public CDialog
{
	DECLARE_DYNAMIC(CEtcTool)

public:
	CEtcTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEtcTool();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ETCTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedReturn();

public:
	void Release();

public:
	void CreateObj(D3DXVECTOR3 vPos);

	void SaveToMyForm(const CString& strPath);
	void LoadToMyForm(const CString& strPath);

	void DeleteEtcData(D3DXVECTOR3 vPos);

private:
	VTXTEX* m_pVtxTex;
	CTextureMgr* m_pTextureMgr;
	CBufferMgr* m_pBufferMgr;
	CObjMgr* m_pObjMgr;
	CTileMap* m_pTileMap;

	list<SLIME*> m_ObjDataList;
	list<IMGPATH*> m_ImgPathList;
	map<CString, CImage*> m_mapImage;

	wstring	m_wstrCurSelName;
	CListBox m_ListBox;
	CStatic m_Picture;
};