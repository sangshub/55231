#pragma once
#include "CubeTool.h"
#include "UnitTool.h"
#include "ObjTool.h"
#include "AniTool.h"
#include "ItemTool.h"
#include "PathTool.h"
#include "EtcTool.h"
#include "afxwin.h"

// CMyForm 폼 뷰입니다.

class CToolView;
class CTileMap;
class CBufferMgr;

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
	enum { IDD = IDD_MYFORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:// 포인터들
	CToolView*		m_pMainView;
	CBufferMgr*		m_pBufferMgr;

private:
	CCubeTool		m_CubeTool;
	CUnitTool		m_UnitTool;
	CItemTool		m_ItemTool;
	CAniTool		m_AniTool;
	CObjTool		m_ObjTool;
	CPathTool		m_PathTool;
	CEtcTool		m_EtcTool;

public:
	CCubeTool*	GetCubeTool(void);
	CAniTool*	GetAniTool(void);
	CObjTool*	GetObjTool(void);
	CItemTool*	GetItemTool();
	CEtcTool*	GetEtcTool();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCubeTool();
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedCreate();

private:
	int m_iTileX;
	int m_iTileZ;
	int m_iDrawId;


	CTileMap*		m_pTileMap;
	VTXCUBE*		m_pCubeTex;

	CListBox m_ListBox;
	CStatic m_Picture;

	list<wstring> m_ImgPathList;
	map<CString, CImage*> m_MapImage;

private:
	void TileLoad();
	void Release();

public:
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedTileSave();
	afx_msg void OnBnClickedButtonObjTool();
	afx_msg void OnBnClickedItemTool();
	afx_msg void OnBnClickedAnimationTool();
	afx_msg void OnBnClickedPathTool();
	afx_msg void OnBnClickedEtcTool();
};