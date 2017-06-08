// ToolView.h : CToolView Ŭ������ �������̽�
//

#pragma once

class CToolDoc;
class CDevice;
class CTimeMgr;
class CBufferMgr;
class CTextureMgr;
class CTileMap;
class CObjMgr;
class CCamMgr;
class CCubeView;
class CKeyMgr;
class CMainCamObj;
class CAniView;

class CToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void CubeTextureInsert(void);

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	void SetCubeView(CCubeView*	_pCubeView);
	void SetAniView(CAniView*	_pAniView);

private:
	bool m_bInit;
	bool m_bDes;

	CDevice*		m_pDevice;
	CTimeMgr*		m_pTimeMgr;
	CBufferMgr*		m_pBufferMgr;
	CTextureMgr*	m_pTextureMgr;
	CObjMgr*		m_pObjMgr;
	CTileMap*		m_pTileMap;
	CCamMgr*		m_pCamMgr;
	CMainCamObj*	m_pCamera;
	CKeyMgr*		m_pKeyMgr;
	CCubeView*		m_pCubeView;
	CAniView*		m_pAniView;
};

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

