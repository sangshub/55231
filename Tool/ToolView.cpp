// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
#include "Tool.h"

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "Include.h"
#include "Device.h"
#include "TimeMgr.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "CamMgr.h"
#include "TileMap.h"
#include "MainCamObj.h"
#include "ObjFactory.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "CubeView.h"
#include "aniview.h"
#include "MyForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWnd;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CToolView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView()
: m_bInit(false)
, m_bDes(false)
, m_pDevice(CDevice::GetInstance())
, m_pTimeMgr(CTimeMgr::GetInstance())
, m_pBufferMgr(CBufferMgr::GetInstance())
, m_pTextureMgr(CTextureMgr::GetInstance())
, m_pTileMap(CTileMap::GetInstance())
, m_pObjMgr(CObjMgr::GetInstance())
, m_pKeyMgr(CKeyMgr::GetInstance())
, m_pCamera(NULL)
, m_pCubeView(NULL)
, m_pAniView(NULL)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
}

CToolView::~CToolView()
{
	if(!m_bDes)
		OnDestroy();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CToolView �׸���

void CToolView::OnDraw(CDC* pDC)
{
	if(!m_bInit)
		OnInitialUpdate();

	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_pTimeMgr->SetTime();
	m_pKeyMgr->KeyCheck();

	m_pCamera->Progress();
	m_pTileMap->Progress();
	m_pObjMgr->Progress();

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	m_pDevice->Render_Begin();

	m_pTileMap->Render();
	m_pObjMgr->Render();

	m_pDevice->Render_End();

	// ť��並 ���� �۾�////////////////
	if(m_pCubeView)
	{
		m_pCubeView->OnDraw(pDC);
	}
	/////////////////////////////////////
	// �ִϺ並 ���� �۾�////////////////
	if(m_pAniView)
	{
		m_pAniView->OnDraw(pDC);
	}
	////////////////////////////////////
	Invalidate(true);
}

// CToolView �μ�


void CToolView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CToolView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CToolView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}

// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG

// CToolView �޽��� ó����
void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	g_hWnd = m_hWnd;

	if(FAILED(m_pDevice->InitDevice()))
	{
		AfxMessageBox(TEXT("Device Initialize Fail"));

		return;
	}

	CMainFrame* pMainFrame = ((CMainFrame*)AfxGetMainWnd());

	RECT rcWindow;
	pMainFrame->GetWindowRect(&rcWindow);

	SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);

	RECT rcMainView;
	GetClientRect(&rcMainView);

	int iRowFrame = rcWindow.right - rcMainView.right;
	int iColFrame = rcWindow.bottom - rcMainView.bottom;

	pMainFrame->SetWindowPos(NULL, 0, 0, WINCX + iRowFrame, WINCY + iColFrame, SWP_NOZORDER);

	m_pTimeMgr->InitTime();

	if(FAILED(m_pBufferMgr->AddBuffer(TEXT("RcTex"), BUFFERTYPE_RCTEX)))
		return;
	if(FAILED(m_pBufferMgr->AddBuffer(TEXT("CubeTex"), BUFFERTYPE_CUBE)))
		return;
	if(FAILED(m_pBufferMgr->AddBuffer(TEXT("UnitTex"), BUFFERTYPE_UNITTEX)))
		return;



	if(FAILED(m_pTextureMgr->InsertTexture(TEXT("../Texture/Tile/Tile%d.png"), TEXT("Tile"), TEXTYPE_MULTI, TEXT("Tile"), TEXT(""), 18)))
		return;
	if(FAILED(m_pTextureMgr->InsertTexture(TEXT("../Texture/Tile/TileA%d.png"), TEXT("Tile"), TEXTYPE_MULTI, TEXT("TileA"), TEXT(""), 18)))
		return;
	if(FAILED(m_pTextureMgr->InsertTexture(TEXT("../Texture/Tile/TileB%d.png"), TEXT("Tile"), TEXTYPE_MULTI, TEXT("TileB"), TEXT(""), 18)))
		return;

	/*if(FAILED(m_pTextureMgr->InsertTexture(TEXT("../Texture/Monster/Blue_Slime/Normal/Normal_%d.png"), TEXT("Blue_Slime"), TEXTYPE_MULTI, TEXT("Normal"), TEXT(""), 8)))
		return;*/

	CubeTextureInsert();

	m_pCamera = (CMainCamObj*)CObjFactory<CMainCamObj>::CreateObj(D3DXVECTOR3(10.f, 0.f, 10.f));

	m_pTileMap->Initialize();
	m_pObjMgr->Initialize();

	//m_pObjMgr->AddObj(OBJ_MONSTER,CObjFactory<CMonster>::CreateObj());

	// ���������� Init �Ǿ��� Ȯ���ϴ� ��, ���� �������� true�� �� ��
	m_bInit = true;
}

void CToolView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	::Safe_Destroy(m_pDevice);
	::Safe_Destroy(m_pTimeMgr);
	::Safe_Destroy(m_pBufferMgr);
	::Safe_Destroy(m_pTextureMgr);
	::Safe_Destroy(m_pTileMap);
	::Safe_Destroy(m_pCamMgr);
	::Safe_Delete(m_pCamera);
	::Safe_Destroy(m_pObjMgr);
	::Safe_Destroy(m_pKeyMgr);
	//((CMainFrame*)AfxGetMainWnd())->m_pMyForm->GetCubeTool()->Release();
}

BOOL CToolView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return true;
}

void CToolView::SetCubeView(CCubeView* _pCubeView)
{
	m_pCubeView = _pCubeView;
}

void CToolView::CubeTextureInsert(void)
{
	if(FAILED(m_pTextureMgr->InsertCubeTexture(L"../Texture/Cube/Cube1.dds", 0)))
		return;
	if(FAILED(m_pTextureMgr->InsertCubeTexture(L"../Texture/Cube/Cube2.dds", 1)))
		return;
	if(FAILED(m_pTextureMgr->InsertCubeTexture(L"../Texture/Cube/Cube3.dds", 2)))
		return;
	if(FAILED(m_pTextureMgr->InsertCubeTexture(L"../Texture/Cube/Cube4.dds", 3)))
		return;
	if(FAILED(m_pTextureMgr->InsertCubeTexture(L"../Texture/Cube/Cube5.dds", 4)))
		return;
	if(FAILED(m_pTextureMgr->InsertCubeTexture(L"../Texture/Cube/Cube6.dds", 5)))
		return;
	if(FAILED(m_pTextureMgr->InsertCubeTexture(L"../Texture/Cube/Cube7.dds", 6)))
		return;
	if(FAILED(m_pTextureMgr->InsertCubeTexture(L"../Texture/Cube/Cube8.dds", 7)))
		return;
	if(FAILED(m_pTextureMgr->InsertCubeTexture(L"../Texture/Cube/Cube9.dds", 8)))
		return;
	if(FAILED(m_pTextureMgr->InsertCubeTexture(L"../Texture/Cube/Cube10.dds", 9)))
		return;
	if(FAILED(m_pTextureMgr->InsertCubeTexture(L"../Texture/Cube/Cube11.dds", 10)))
		return;
	if(FAILED(m_pTextureMgr->InsertCubeTexture(L"../Texture/Cube/Cube12.dds", 11)))
		return;
	if(FAILED(m_pTextureMgr->InsertCubeTexture(L"../Texture/Cube/Cube13.dds", 12)))
		return;
	if(FAILED(m_pTextureMgr->InsertCubeTexture(L"../Texture/Cube/Cube14.dds", 13)))
		return;
	if(FAILED(m_pTextureMgr->InsertCubeTexture(L"../Texture/Cube/Cube15.dds", 14)))
		return;
	if(FAILED(m_pTextureMgr->InsertCubeTexture(L"../Texture/Cube/Cube16.dds", 15)))
		return;
	if(FAILED(m_pTextureMgr->InsertCubeTexture(L"../Texture/Cube/Cube17.dds", 16)))
		return;
	if(FAILED(m_pTextureMgr->InsertCubeTexture(L"../Texture/Cube/Cube18.dds", 17)))
		return;
	if(FAILED(m_pTextureMgr->InsertCubeTexture(L"../Texture/Cube/Cube19.dds", 18)))
		return;
	if(FAILED(m_pTextureMgr->InsertCubeTexture(L"../Texture/Cube/Cube20.dds", 19)))
		return;
}

void CToolView::SetAniView(CAniView* _pAniView)
{
	m_pAniView = _pAniView;
}
