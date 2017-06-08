// AniTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "AniTool.h"
#include "ToolView.h"
#include "TileMap.h"
#include "ObjMgr.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "MainFrm.h"
#include "Cube.h"
#include "ObjFactory.h"
#include "MathMgr.h"
#include "Pipeline.h"
#include "TimeMgr.h"
#include "TileMap.h"


// CAniTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAniTool, CDialog)

CAniTool::CAniTool(CWnd* pParent /*=NULL*/)
	: CDialog(CAniTool::IDD, pParent)
	, m_pCubeTex(NULL)
	, m_pConvertCubeTex(NULL)
	, m_bParents(false)
	, m_bEditMode(false)
{

}

CAniTool::~CAniTool()
{
	Release();
}

void CAniTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT12, m_tCubeData.fPosX);
	DDX_Text(pDX, IDC_EDIT13, m_tCubeData.fPosY);
	DDX_Text(pDX, IDC_EDIT14, m_tCubeData.fPosZ);
	DDX_Text(pDX, IDC_EDIT6, m_tCubeData.fScaleX);
	DDX_Text(pDX, IDC_EDIT7, m_tCubeData.fScaleY);
	DDX_Text(pDX, IDC_EDIT8, m_tCubeData.fScaleZ);
	DDX_Text(pDX, IDC_EDIT9, m_tCubeData.fAngle[ANGLE_X]);
	DDX_Text(pDX, IDC_EDIT10, m_tCubeData.fAngle[ANGLE_Y]);
	DDX_Text(pDX, IDC_EDIT11, m_tCubeData.fAngle[ANGLE_Z]);
	DDX_Control(pDX, IDC_LIST3, m_ObjectList);
	DDX_Control(pDX, IDC_LIST1, m_ActiveList);
	DDX_Text(pDX, IDC_EDIT1, m_tCubeData.fTime);
	DDX_Text(pDX, IDC_EDIT4, m_tCubeData.iImageNum);
}


BEGIN_MESSAGE_MAP(CAniTool, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON19, &CAniTool::OnBnClickedParentsCreate)
	ON_LBN_SELCHANGE(IDC_LIST3, &CAniTool::OnLbnSelchangeSelectObject)
	ON_BN_CLICKED(IDC_BUTTON24, &CAniTool::OnBnClickedCubeDataChange)
	ON_BN_CLICKED(IDC_BUTTON20, &CAniTool::OnBnClickedChildCreate)
	ON_BN_CLICKED(IDC_BUTTON23, &CAniTool::OnBnClickedCubeDelete)
	ON_BN_CLICKED(IDC_BUTTON21, &CAniTool::OnBnClickedAddActive)
	ON_LBN_SELCHANGE(IDC_LIST1, &CAniTool::OnLbnSelchangeActiveList)
	ON_BN_CLICKED(IDC_BUTTON13, &CAniTool::OnBnClickedObjectCreateSucces)
	ON_BN_CLICKED(IDC_BUTTON15, &CAniTool::OnBnClickedAnimationPlay)
	ON_BN_CLICKED(IDC_BUTTON16, &CAniTool::OnBnClickedAniStop)
	ON_BN_CLICKED(IDC_BUTTON22, &CAniTool::OnBnClickedActiveDelete)
	ON_BN_CLICKED(IDC_BUTTON17, &CAniTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON14, &CAniTool::OnBnClickedObjectSave)
	ON_BN_CLICKED(IDC_BUTTON25, &CAniTool::OnBnClickedObjectLoad)
	ON_BN_CLICKED(IDC_BUTTONALLDELETE, &CAniTool::OnBnClickedAllDelete)
	ON_BN_CLICKED(IDC_BUTTON1, &CAniTool::OnBnClickedAniTimeChange)
	ON_BN_CLICKED(IDC_BUTTON18, &CAniTool::OnBnClickedAnimationLoad)
	ON_BN_CLICKED(IDC_BUTTON26, &CAniTool::OnBnClickedImageNumChange)
END_MESSAGE_MAP()


// CAniTool 메시지 처리기입니다.

void CAniTool::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if(m_AniDialog.GetSafeHwnd() == NULL)
	{
		m_AniDialog.Create(IDD_ANIDIALOG);
		m_AniDialog.SetAniTool(this);
	}
	RECT rc;
	GetWindowRect(&rc);
	m_AniDialog.SetWindowSize(rc);
	m_AniDialog.ShowWindow(SW_SHOW);
}

void CAniTool::SetAniView(CAniView* _pAniView)
{
	m_pAniView = _pAniView;

	m_pMainView->SetAniView(_pAniView);
}
BOOL CAniTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_iObjCnt = 0; // 오브젝트 총개수 초기화
	m_tCube.iDrawId = 0;
	m_tCube.dwType = 0;
	m_iMode = 0;
	m_iXYZ = 0;
	m_tCube.vPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	m_pTileMap = CTileMap::GetInstance();
	m_pObjMgr = CObjMgr::GetInstance();
	m_pBufferMgr = CBufferMgr::GetInstance();
	m_pTextureMgr = CTextureMgr::GetInstance();
	m_pCubeTex = new VTXCUBE[8];
	m_pConvertCubeTex = new VTXCUBE[8];

	m_pMainView = ((CMainFrame*)AfxGetMainWnd())->m_pMainView;
	m_pBufferMgr->GetVtxInfo(TEXT("CubeTex"), m_pCubeTex);

	UpdateData(true);
	m_tCubeData.iImageNum = 0;
	m_tCubeData.fTime = 0.f;
	m_tCubeData.fPosX = 0.f;
	m_tCubeData.fPosY = 0.f;
	m_tCubeData.fPosZ = 0.f;
	m_tCubeData.fScaleX = 1.f;
	m_tCubeData.fScaleY = 1.f;
	m_tCubeData.fScaleZ = 1.f;
	m_tCubeData.fAngle[ANGLE_X] = 0;
	m_tCubeData.fAngle[ANGLE_Y] = 0;
	m_tCubeData.fAngle[ANGLE_Z] = 0;
	UpdateData(false);

	m_bParents = false;
	m_bSucces = false;

	ResetCubeData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAniTool::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_AniDialog.ShowWindow(SW_HIDE);
	m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pCubeTex);

	CDialog::OnClose();
}

void CAniTool::Release(void)
{
	if(m_pCubeTex)
		::Safe_Delete_Array(m_pCubeTex);
	if(m_pConvertCubeTex)
		::Safe_Delete_Array(m_pConvertCubeTex);

	for_each(m_vecAnimation.begin(), m_vecAnimation.end(), CDeleteObj());
	m_vecAnimation.clear();
	for_each(m_vecCubeData.begin(), m_vecCubeData.end(), CDeleteObj());
	m_vecCubeData.clear();

	for(vector<CUBEDATA*>::iterator iter = m_vecActive.begin();
		iter != m_vecActive.end(); ++iter)
		::Safe_Delete_Array(*iter);
	m_vecActive.clear();

	for(vector<ANI*>::iterator iter = m_vecAni.begin();
		iter != m_vecAni.end(); ++iter)
		::Safe_Delete_Array(*iter);
	m_vecAni.clear();
}
void CAniTool::OnBnClickedParentsCreate()
{
	UpdateData(true);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(!m_bParents && !m_bSucces)
	{
		m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pCubeTex);
		CCube*	pCube = (CCube*)CObjFactory<CCube>::CreateObj();
		CUBEDATA* pCubeData = new CUBEDATA;

		pCube->SetCubeInfo(m_tCube);
		pCube->SetAniToolMode(true);
		SetCubeDataCopy(*pCubeData, m_tCubeData);

		InsertCube(pCube, pCubeData);
		TransformCube(pCube);
		m_bParents = true;

		m_ObjectList.AddString(L"00.Parents");
	}
	UpdateData(false);
}

void CAniTool::OnBnClickedChildCreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_bParents && !m_bSucces)
	{
		m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pCubeTex);
		CCube*	pCube = (CCube*)CObjFactory<CCube>::CreateObj();
		CUBEDATA* pCubeData = new CUBEDATA;

		pCube->SetCubeInfo(m_tCube);
		pCube->SetAniToolMode(true);
		SetCubeDataCopy(*pCubeData, m_tCubeData);

		InsertCube(pCube, pCubeData);
		TransformCube(pCube);
		pCube->SetParentsMatrix(m_vecAnimation[0]->GetParentMatrix());

		int iNum = m_vecAnimation.size()-1;
		TCHAR szChildName[MIN_STR] = L"";
		if(iNum < 10)
			wsprintf(szChildName, L"0%d.Child", iNum);
		else
			wsprintf(szChildName, L"%d.Child", iNum);

		m_ObjectList.AddString(szChildName);
	}
	UpdateData(false);
}

void CAniTool::InsertCube(CCube* pCube, CUBEDATA* pCubeData)
{
	pCube->SetAniViewSwitch(true);
	m_vecAnimation.push_back(pCube);
	m_vecCubeData.push_back(pCubeData);
}

void CAniTool::SetCubeDataCopy(CUBEDATA& tOut, const CUBEDATA& tIn)
{
	UpdateData(TRUE);
	tOut.iImageNum = tIn.iImageNum;
	tOut.fTime = tIn.fTime;
	tOut.fAngle[ANGLE_X] = tIn.fAngle[ANGLE_X]; 
	tOut.fAngle[ANGLE_Y] = tIn.fAngle[ANGLE_Y]; 
	tOut.fAngle[ANGLE_Z] = tIn.fAngle[ANGLE_Z];
	tOut.fPosX = tIn.fPosX;
	tOut.fPosY = tIn.fPosY;
	tOut.fPosZ = tIn.fPosZ;
	tOut.fScaleX = tIn.fScaleX;
	tOut.fScaleY = tIn.fScaleY;
	tOut.fScaleZ = tIn.fScaleZ;
	UpdateData(false);
}

vector<CCube*>* CAniTool::GetAnimationCube(void)
{
	return &m_vecAnimation;
}

void CAniTool::ResetCubeData(void)
{
	memset(&m_tCube, 0, sizeof(CUBE));
}

void CAniTool::TransformCube(CCube* pCube)
{
	UpdateData(true);
	D3DXMATRIX matWorld;
	D3DXMATRIX matLocal;

	CreateWorldMatrix(matWorld);

	pCube->SetWorldMatrix(matWorld);

	if(m_vecAnimation[0] == pCube)
	{
		CreateParentsMatrix(matLocal);
		pCube->SetChildMatrix(matLocal);
		m_matParents = matLocal;
	}
	
	UpdateData(false);
}

void CAniTool::CreateWorldMatrix(D3DXMATRIX& matWorld) // 행렬만들기
{
	UpdateData(true);
	float fAngel[ANGLE_END];
	for(int i = 0; i < ANGLE_END; ++i)
		fAngel[i] = D3DXToRadian(m_tCubeData.fAngle[i]);

	CPipeline::MakeWorldMatrix(matWorld
		, D3DXVECTOR3(m_tCubeData.fScaleX, m_tCubeData.fScaleY, m_tCubeData.fScaleZ)
		, fAngel
		, D3DXVECTOR3(m_tCubeData.fPosX, m_tCubeData.fPosY, m_tCubeData.fPosZ));

	UpdateData(false);
}

void CAniTool::CreateParentsMatrix(D3DXMATRIX& matWorld) // 부모행렬만들기
{
	UpdateData(true);
	float fAngel[ANGLE_END];
	for(int i = 0; i < ANGLE_END; ++i)
		fAngel[i] = D3DXToRadian(m_tCubeData.fAngle[i]);

	CPipeline::MakeWorldMatrix(matWorld
		, D3DXVECTOR3(1.f, 1.f, 1.f)
		, fAngel
		, D3DXVECTOR3(m_tCubeData.fPosX, m_tCubeData.fPosY, m_tCubeData.fPosZ));

	UpdateData(false);
}

void CAniTool::OnLbnSelchangeSelectObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(true);
	int iSelect = m_ObjectList.GetCurSel();

	if(iSelect > -1)
	{
		SetCubeDataCopy(m_tCubeData, *m_vecCubeData[iSelect]);
		m_vecAnimation[iSelect]->SetCubeTexture(m_vecCubeData[iSelect]->iImageNum);
	}
	UpdateData(false);
}

void CAniTool::OnBnClickedObjectSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog		Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.dat", this);

	TCHAR		szDirtoryPath[MAX_PATH];
	GetCurrentDirectory(sizeof(szDirtoryPath), szDirtoryPath);
	SetCurrentDirectory(szDirtoryPath);

	PathRemoveFileSpec(szDirtoryPath);
	lstrcat(szDirtoryPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szDirtoryPath;

	if(Dlg.DoModal() == IDCANCEL)
		return;

	HANDLE hFile = NULL;
	DWORD dwByte = 0;

	hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS
		, FILE_ATTRIBUTE_NORMAL, NULL);

	for(vector<CUBEDATA*>::iterator iter = m_vecCubeData.begin();
		iter != m_vecCubeData.end(); ++iter)
	{
		WriteFile(hFile, *iter, sizeof(CUBEDATA), &dwByte, NULL);
	}
	CloseHandle(hFile);
}

void CAniTool::OnBnClickedObjectLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for_each(m_vecAnimation.begin(), m_vecAnimation.end(), CDeleteObj());
	m_vecAnimation.clear();
	for_each(m_vecCubeData.begin(), m_vecCubeData.end(), CDeleteObj());
	m_vecCubeData.clear();

	CFileDialog		Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.dat", this);

	if(Dlg.DoModal() == IDCANCEL)
		return;

	HANDLE hFile = NULL;
	DWORD dwByte = 0;

	hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, NULL, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL, NULL);

	m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pCubeTex);

	while(true)
	{
		CUBEDATA* pCubeData = new CUBEDATA;
		ReadFile(hFile, pCubeData, sizeof(CUBEDATA), &dwByte, NULL);

		if(dwByte == 0)
		{
			Safe_Delete(pCubeData);
			break;
		}

		m_vecCubeData.push_back(pCubeData); // 저장할때 쓸 데이터
	}
	CloseHandle(hFile);

	UpdateData(TRUE);

	for(vector<CUBEDATA*>::iterator iter = m_vecCubeData.begin();
		iter != m_vecCubeData.end(); ++iter)
	{
		if(*iter == m_vecCubeData[0])
		{
			m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pCubeTex);
			CCube*	pCube = (CCube*)CObjFactory<CCube>::CreateObj();

			pCube->SetCubeInfo(m_tCube);
			pCube->SetAniToolMode(true);
			pCube->SetAniViewSwitch(true);
			pCube->SetCubeTexture((*iter)->iImageNum);
			m_vecAnimation.push_back(pCube);
			SetCubeDataCopy(m_tCubeData, *(*iter));
			TransformCube(pCube);
			m_bParents = true;  // 부모생성 완료

			m_ObjectList.AddString(L"00.Parents");
		}
		else if(m_bParents)
		{
			m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pCubeTex);
			CCube*	pCube = (CCube*)CObjFactory<CCube>::CreateObj();

			pCube->SetCubeInfo(m_tCube);
			pCube->SetAniToolMode(true);
			pCube->SetAniViewSwitch(true);
			pCube->SetCubeTexture((*iter)->iImageNum);
			m_vecAnimation.push_back(pCube);
			SetCubeDataCopy(m_tCubeData, *(*iter));
			TransformCube(pCube);
			pCube->SetParentsMatrix(m_vecAnimation[0]->GetParentMatrix());

			int iNum = m_vecAnimation.size()-1;
			TCHAR szChildName[MIN_STR] = L"";
			if(iNum < 10)
				wsprintf(szChildName, L"0%d.Child", iNum);
			else
				wsprintf(szChildName, L"%d.Child", iNum);

			m_ObjectList.AddString(szChildName);
		}
	}
	m_iObjCnt = m_vecCubeData.size();
	UpdateData(FALSE);
}

void CAniTool::OnBnClickedAllDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_bParents = false;
	m_bSucces = false;
	m_ObjectList.ResetContent();
	m_ActiveList.ResetContent();
	m_tCubeData.iImageNum = 0;
	m_tCubeData.fTime = 0.f;
	m_tCubeData.fPosX = 0.f;
	m_tCubeData.fPosY = 0.f;
	m_tCubeData.fPosZ = 0.f;
	m_tCubeData.fScaleX = 1.f;
	m_tCubeData.fScaleY = 1.f;
	m_tCubeData.fScaleZ = 1.f;
	m_tCubeData.fAngle[ANGLE_X] = 0;
	m_tCubeData.fAngle[ANGLE_Y] = 0;
	m_tCubeData.fAngle[ANGLE_Z] = 0;

	for_each(m_vecAnimation.begin(), m_vecAnimation.end(), CDeleteObj());
	m_vecAnimation.clear();
	for_each(m_vecCubeData.begin(), m_vecCubeData.end(), CDeleteObj());
	m_vecCubeData.clear();

	for(vector<CUBEDATA*>::iterator iter = m_vecActive.begin();
		iter != m_vecActive.end(); ++iter)
		::Safe_Delete_Array(*iter);

	m_vecActive.clear();

	for(vector<ANI*>::iterator iter = m_vecAni.begin();
		iter != m_vecAni.end(); ++iter)
		::Safe_Delete_Array(*iter);

	m_vecAni.clear();

	UpdateData(FALSE);
}


void CAniTool::OnBnClickedCubeDataChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true);
	int iSelect = m_ObjectList.GetCurSel();

	if(iSelect > -1)
	{
		SetCubeDataCopy(*m_vecCubeData[iSelect], m_tCubeData);
		TransformCube(m_vecAnimation[iSelect]);
		m_vecAnimation[iSelect]->SetCubeTexture(m_vecCubeData[iSelect]->iImageNum);

		if(m_bSucces)
		{
			int iSelectActive = m_ActiveList.GetCurSel();
			if(iSelectActive > -1)
			{
				SetCubeDataCopy(m_vecActive[iSelectActive][iSelect], *m_vecCubeData[iSelect]);
				CreateAniData(m_vecAni[iSelectActive][iSelect], m_vecActive[iSelectActive][iSelect]);
			}
			
		}
	}
	UpdateData(false);
}

void CAniTool::OnBnClickedCubeDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iNum = m_ObjectList.GetCurSel();

	if(iNum > -1 && m_vecAnimation.size() > 1 && m_ObjectList.GetCurSel() != 0 && !m_bSucces)
	{
		for(vector<CCube*>::iterator iter = m_vecAnimation.begin();
			iter != m_vecAnimation.end(); ++iter)
		{
			if(*iter == m_vecAnimation[iNum])
			{
				::Safe_Delete(*iter);
				m_vecAnimation.erase(iter);
				break;
			}
		}
		for(vector<CUBEDATA*>::iterator iter = m_vecCubeData.begin();
			iter != m_vecCubeData.end(); ++iter)
		{
			if(*iter == m_vecCubeData[iNum])
			{
				::Safe_Delete(*iter);
				m_vecCubeData.erase(iter);
				break;
			}
		}
		m_ObjectList.DeleteString(iNum);
	}
	else if(iNum > -1 && m_vecAnimation.size() == 1 && !m_bSucces)
	{
		::Safe_Delete(m_vecAnimation.back());
		m_vecAnimation.pop_back();
		::Safe_Delete(m_vecCubeData.back());
		m_vecCubeData.pop_back();
		m_vecAnimation.clear();
		m_vecCubeData.clear();
		m_ObjectList.DeleteString(iNum);
		m_bParents = false;
	}
}

void CAniTool::OnBnClickedObjectCreateSucces() // 오브젝트 초기설정 완료
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if(m_vecAnimation.size() && !m_bSucces)
	{
		m_iObjCnt = m_vecAnimation.size();
		CUBEDATA* pCubeData = new CUBEDATA[m_iObjCnt];

		for(int i = 0; i < m_iObjCnt; ++i)
		{
			SetCubeDataCopy(pCubeData[i], *m_vecCubeData[i]);
		}
		TCHAR szObjActiveNum[MIN_STR] = L"";
		if(m_vecActive.size() < 10)
			wsprintf(szObjActiveNum, L"0%d", m_vecActive.size());
		else
			wsprintf(szObjActiveNum, L"%d", m_vecActive.size());
		m_ActiveList.AddString(szObjActiveNum);
		m_vecActive.push_back(pCubeData);

		// 애니 데이터 생성///////////////////////////////////////////
		ANI* pAni = new ANI[m_iObjCnt];

		for(int i = 0; i < m_iObjCnt; ++i)
			CreateAniData(pAni[i], m_vecActive[0][i]);
		m_vecAni.push_back(pAni);
		//////////////////////////////////////////////////////////////

		m_bSucces = true;
	}
	UpdateData(false);
}

void CAniTool::OnBnClickedAddActive()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if(m_vecAnimation.size() && m_bSucces)
	{
		CUBEDATA* pCubeData = new CUBEDATA[m_iObjCnt];

		for(int i = 0; i < m_iObjCnt; ++i)
			SetCubeDataCopy(pCubeData[i], *m_vecCubeData[i]);
		
		TCHAR szObjActiveNum[MIN_STR] = L"";
		int iActiveSize = m_vecActive.size();
		if(iActiveSize < 10)
			wsprintf(szObjActiveNum, L"0%d", iActiveSize);
		else
			wsprintf(szObjActiveNum, L"%d", iActiveSize);
		m_ActiveList.AddString(szObjActiveNum);
		m_vecActive.push_back(pCubeData);

		// 애니 데이터 생성///////////////////////////////////////////
		ANI* pAni = new ANI[m_iObjCnt];

		for(int i = 0; i < m_iObjCnt; ++i)
			CreateAniData(pAni[i], m_vecActive[iActiveSize][i]);
		m_vecAni.push_back(pAni);
		//////////////////////////////////////////////////////////////
	}
	UpdateData(false);
}
void CAniTool::OnLbnSelchangeActiveList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int iSelect = m_ActiveList.GetCurSel();
	m_ObjectList.SetCurSel(-1);

	if(iSelect > -1)
	{
		for(int i = 0; i < m_iObjCnt; ++i)
		{
			SetCubeDataCopy(*m_vecCubeData[i], m_vecActive[iSelect][i]);
			SetCubeDataCopy(m_tCubeData, m_vecActive[iSelect][i]);
			TransformCube(m_vecAnimation[i]);
		}
	}
	UpdateData(false);
}
void CAniTool::OnBnClickedAnimationPlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_bSucces)
	{
		for(int i = 0; i < m_iObjCnt; ++i)
			m_vecAnimation[i]->ReleaseAnimation();

		for(vector<ANI*>::iterator iter = m_vecAni.begin();
			iter != m_vecAni.end(); ++iter)
		{
			for(int i = 0; i < m_iObjCnt; ++i)
			{
				m_vecAnimation[i]->SetAniData((*iter)[i]);
				m_vecAnimation[i]->SetAniPlay(true);
				m_vecAnimation[i]->SetCubeTexture(m_vecCubeData[i]->iImageNum);
			}
		}
	}
}

void CAniTool::CreateAniData(ANI& tAni, CUBEDATA& tCubeData)
{
	tAni.vPos = D3DXVECTOR3( tCubeData.fPosX, tCubeData.fPosY, tCubeData.fPosZ );	/// 위치 키(0,0,0)
	tAni.vScale = D3DXVECTOR3( tCubeData.fScaleX, tCubeData.fScaleY, tCubeData.fScaleZ );	/// 위치 키(0,0,0)

	D3DXQuaternionRotationYawPitchRoll( &tAni.qRot
		, D3DXToRadian(tCubeData.fAngle[ANGLE_Y])
	, D3DXToRadian(tCubeData.fAngle[ANGLE_X])
	, D3DXToRadian(tCubeData.fAngle[ANGLE_Z]));	/// Y, X, Z 순서

	tAni.fTime = tCubeData.fTime;
}

void CAniTool::ConvertAniData(void)
{
	UpdateData(TRUE);
	for(vector<ANI*>::iterator iter = m_vecAni.begin();
		iter != m_vecAni.end(); ++iter)
		::Safe_Delete_Array(*iter);
	m_vecAni.clear();

	for(vector<CUBEDATA*>::iterator iter = m_vecActive.begin();
		iter != m_vecActive.end(); ++iter)
	{
		ANI* pAni = new ANI[m_iObjCnt];

		for(int i = 0; i < m_iObjCnt; ++i)
		{
			CreateAniData(pAni[i], (*iter)[i]);
		}
		m_vecAni.push_back(pAni);
	}
	UpdateData(FALSE);
}

void CAniTool::OnBnClickedAniStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for(int i = 0; i < m_iObjCnt; ++i)
	{
		m_vecAnimation[i]->SetAniPlay(false);
	}
}

void CAniTool::OnBnClickedActiveDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iSelect = m_ActiveList.GetCurSel();

	if(iSelect > -1)
	{
		for(vector<ANI*>::iterator iter = m_vecAni.begin();
			iter != m_vecAni.end(); ++iter)
		{
			if(*iter == m_vecAni[iSelect])
			{
				::Safe_Delete_Array(*iter);
				m_vecAni.erase(iter);
				break;
			}
		}

		for(vector<CUBEDATA*>::iterator iter = m_vecActive.begin();
			iter != m_vecActive.end(); ++iter)
		{
			if(*iter == m_vecActive[iSelect])
			{
				::Safe_Delete_Array(*iter);
				m_vecActive.erase(iter);
				break;
			}
		}

		m_ActiveList.DeleteString(iSelect);
	}
}

void CAniTool::OnBnClickedAniTimeChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int iActiveNum = m_ActiveList.GetCurSel();

	if(iActiveNum > -1)
	{
		for(int i = 0; i < m_iObjCnt; ++i)
		{
			m_vecAni[iActiveNum][i].fTime = m_tCubeData.fTime;
			m_vecActive[iActiveNum][i].fTime = m_tCubeData.fTime;
		}
	}

	UpdateData(FALSE);
}

void CAniTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_vecActive.size() > 1)
	{
		CFileDialog		Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
			, L"*.dat", this);

		TCHAR		szDirtoryPath[MAX_PATH];
		GetCurrentDirectory(sizeof(szDirtoryPath), szDirtoryPath);
		SetCurrentDirectory(szDirtoryPath);

		PathRemoveFileSpec(szDirtoryPath);
		lstrcat(szDirtoryPath, L"\\Data");

		Dlg.m_ofn.lpstrInitialDir = szDirtoryPath;

		if(Dlg.DoModal() == IDCANCEL)
			return;

		HANDLE hFile = NULL;
		DWORD dwByte = 0;

		hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS
			, FILE_ATTRIBUTE_NORMAL, NULL);

		for(vector<CUBEDATA*>::iterator iter = m_vecActive.begin();
			iter != m_vecActive.end(); ++iter)
		{
			for(int i = 0; i < m_iObjCnt; ++i)
				WriteFile(hFile, &(*iter)[i], sizeof(CUBEDATA), &dwByte, NULL);
		}
		CloseHandle(hFile);
	}
}
void CAniTool::OnBnClickedAnimationLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	//삭제/////////////////////////////////////////////////////////////////////////
	for(vector<CUBEDATA*>::iterator iter = m_vecActive.begin();
		iter != m_vecActive.end(); ++iter)
		::Safe_Delete_Array(*iter);
	m_vecActive.clear();

	for(vector<ANI*>::iterator iter = m_vecAni.begin();
		iter != m_vecAni.end(); ++iter)
		::Safe_Delete_Array(*iter);
	m_vecAni.clear();

	m_ActiveList.ResetContent();
	/////////////////////////////////////////////////////////////////////////////////

	m_bParents = true;
	m_bSucces = true;

	CFileDialog		Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.dat", this);

	if(Dlg.DoModal() == IDCANCEL)
		return;

	HANDLE hFile = NULL;
	DWORD dwByte = 0;

	hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, NULL, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL, NULL);

	m_pBufferMgr->SetVtxInfo(TEXT("CubeTex"), m_pCubeTex);

	while(true)
	{
		CUBEDATA* pCubeData;

		if(m_iObjCnt > 0)
			pCubeData = new CUBEDATA[m_iObjCnt];
		else
			return;

		for(int i = 0; i < m_iObjCnt; ++i)
			ReadFile(hFile, &pCubeData[i], sizeof(CUBEDATA), &dwByte, NULL);

		if(dwByte == 0)
		{
			::Safe_Delete_Array(pCubeData);
			break;
		}

		TCHAR szObjActiveNum[MIN_STR] = L"";
		int iActiveSize = m_vecActive.size();
		if(iActiveSize < 10)
			wsprintf(szObjActiveNum, L"0%d", iActiveSize);
		else
			wsprintf(szObjActiveNum, L"%d", iActiveSize);
		m_ActiveList.AddString(szObjActiveNum);

		m_vecActive.push_back(pCubeData); // 저장할때 쓸 데이터
	}
	CloseHandle(hFile);

	for(vector<CUBEDATA*>::iterator iter = m_vecActive.begin();
		iter != m_vecActive.end(); ++iter)
	{
		ANI* pAni = new ANI[m_iObjCnt];

		for(int i = 0; i < m_iObjCnt; ++i)
			CreateAniData(pAni[i], (*iter)[i]);

		m_vecAni.push_back(pAni);
	}
	UpdateData(FALSE);
}

void CAniTool::OnBnClickedImageNumChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	int iObjectNum = m_ObjectList.GetCurSel();

	if(iObjectNum > -1)
	{
		m_vecAnimation[iObjectNum]->SetCubeTexture(m_vecCubeData[iObjectNum]->iImageNum);
	}

	UpdateData(FALSE);
}

bool CAniTool::KeyInput(void)
{
	if(GetAsyncKeyState(VK_NUMPAD9) & 0x0001)
	{
		if(m_bEditMode)
			m_bEditMode = false;
		else if(!m_bEditMode)
			m_bEditMode = true;
	}

	if(m_bEditMode)
	{
		m_fValue = 0.f;
		m_bKeyInput = false;

		if(GetAsyncKeyState('Q')) // 포스
			m_iMode = 1;
		if(GetAsyncKeyState('W')) // 스케일
			m_iMode = 2;
		if(GetAsyncKeyState('E')) // 회전
			m_iMode = 3;

		if(GetAsyncKeyState('1')) // X
			m_iXYZ = 1;
		if(GetAsyncKeyState('2')) // Y
			m_iXYZ = 2;
		if(GetAsyncKeyState('3')) // Z
			m_iXYZ = 3;

		if(GetAsyncKeyState(VK_NUMPAD8))
		{
			m_bKeyInput = true;
			m_fValue += 1.f * GETTIME; 
		}
		if(GetAsyncKeyState(VK_NUMPAD5))
		{
			m_bKeyInput = true;
			m_fValue -= 1.f * GETTIME;
		}

		UpdateData(TRUE);
		if(m_iMode == 1)
		{
			if(m_iXYZ == 1)
				m_tCubeData.fPosX += m_fValue;
			else if(m_iXYZ == 2)
				m_tCubeData.fPosY += m_fValue;
			else if(m_iXYZ == 3)
				m_tCubeData.fPosZ += m_fValue;
		}
		else if(m_iMode == 2)
		{
			if(m_iXYZ == 1)
				m_tCubeData.fScaleX += m_fValue;
			else if(m_iXYZ == 2)
				m_tCubeData.fScaleY += m_fValue;
			else if(m_iXYZ == 3)
				m_tCubeData.fScaleZ += m_fValue;
		}
		else if(m_iMode == 3)
		{
			if(m_iXYZ == 1)
				m_tCubeData.fAngle[ANGLE_X] += m_fValue*60;
			else if(m_iXYZ == 2)
				m_tCubeData.fAngle[ANGLE_Y] += m_fValue*60;
			else if(m_iXYZ == 3)
				m_tCubeData.fAngle[ANGLE_Z] += m_fValue*60;
		}
		UpdateData(FALSE);

		int iSelect = m_ObjectList.GetCurSel();

		UpdateData(TRUE);
		if(m_bKeyInput && iSelect > -1)
		{
			SetCubeDataCopy(*m_vecCubeData[iSelect], m_tCubeData);
			TransformCube(m_vecAnimation[iSelect]);
			m_vecAnimation[iSelect]->SetCubeTexture(m_vecCubeData[iSelect]->iImageNum);

			if(m_bSucces)
			{
				int iSelectActive = m_ActiveList.GetCurSel();
				if(iSelectActive > -1)
				{
					SetCubeDataCopy(m_vecActive[iSelectActive][iSelect], *m_vecCubeData[iSelect]);
					CreateAniData(m_vecAni[iSelectActive][iSelect], m_vecActive[iSelectActive][iSelect]);
				}

			}
		}
		UpdateData(FALSE);
	}

	return m_bEditMode;
}