#pragma once
#include "Include.h"
#include "ViewDialog.h"
#include "afxwin.h"


// CCubeTool ��ȭ �����Դϴ�.

class CCubeView;
class CObjMgr;
class CTileMap;
class CToolView;
class CBufferMgr;
class CTextureMgr;
class CCubeTool : public CDialog
{
	DECLARE_DYNAMIC(CCubeTool)

public:
	CCubeTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCubeTool();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CUBETOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	CCubeView*		m_pCubeView; // ��â ������
	CObjMgr*		m_pObjMgr;
	CTileMap*		m_pTileMap;
	CToolView*		m_pMainView; // ���� ��
	CBufferMgr*		m_pBufferMgr;
	CTextureMgr*	m_pTextureMgr;

private:
	CViewDialog	m_ViewDIalog;

public:
	void SetCubeView(CCubeView*	_pCubeView);
	void GetCubeData(CUBE& tOut, D3DXVECTOR3 _vTilePos);
	void CreateCube(D3DXVECTOR3 vTilePos, int iCount); // �ʿ� �� ���뵵
	void CubeImageSetUp(void);
	void RemoveCubeData(CUBE& tOut);

	void SaveToMyForm(const CString& strPath);
	void LoadToMyForm(const CString& strPath);
	void DeleteCubeData(D3DXVECTOR3 vPos);

public:
	void Release(void);

private:
	CUBE			m_tCube;
	VTXCUBE*		m_pCubeTex;
	vector<CUBE*>	m_CubeData;
	list<CUBE*>		m_CubeDrop; // ���� �� ť�� ������

	map<BYTE, LPDIRECT3DCUBETEXTURE9>*	m_mapCubeTex;
	
private:
	CListBox	m_ImageList;
	CListBox	m_CubeList;
	CButton		m_Check[7];
	CString		m_CubeName;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLbnSelchangeImageList();
	afx_msg void OnBnClickedAddCubeData();	
	afx_msg void OnLbnSelchangeCubeDataList();
	afx_msg void OnBnClickedDeleteList();
	afx_msg void OnBnClickedChangeData();
	afx_msg void OnBnClickedDropCubeDelete();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
};
