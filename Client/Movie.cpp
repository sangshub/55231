#include "stdafx.h"
#include "Movie.h"
#include "Device.h"
#include "SceneMgr.h"
#include "BufferMgr.h"
#include "TextureMgr.h"
#include "MediaMgr.h"
#include "ObjMgr.h"

unsigned int _stdcall ImgLoadThreadFunc(void* pArg);

CMovie::CMovie(void)
: m_pMediaMgr(CMediaMgr::GetInstance())
{
}

CMovie::~CMovie(void)
{
	Release();
}

HRESULT CMovie::Initialize()
{
	m_pDevice->SoundPlay(TEXT("Intro"), 0);

	m_pMediaMgr->VideoPlay(TEXT("../Video/Video.avi"));

	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, ImgLoadThreadFunc, this, 0, NULL);
	
	return S_OK;
}

void CMovie::Progress()
{
	m_pMediaMgr->Progress();

	if(CTextureMgr::GetInstance()->GetPathName() == TEXT("Loading Complete"))
	{
		if(GetAsyncKeyState(VK_RETURN))
			NextStage();
	}
}

void CMovie::Render()
{
	//-DirectXShow----------------------------------------------------------------------------------------
	m_pMediaMgr->Render();
	//-DirectXShow----------------------------------------------------------------------------------------
	SetWindowText(g_hWnd, m_pTextureMgr->GetPathName().c_str());
}

void CMovie::Release()
{
	::Safe_Destroy(m_pMediaMgr);
	m_pDevice->SoundStop(TEXT("Intro"));
}

void CMovie::StopBgm()
{
}

void CMovie::NextStage()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_Crt);

	m_pSceneMgr->SetSceneChange(SCENE_LOGO);
}

unsigned int __stdcall ImgLoadThreadFunc(void* pArg)
{
	CMovie* pMovie = (CMovie*)pArg;

	CTextureMgr* pTextureMgr = CTextureMgr::GetInstance();
	CBufferMgr* pBufferMgr = CBufferMgr::GetInstance();

	if(FAILED(pBufferMgr->AddBuffer(TEXT("RcCol"), BUFFERTYPE_RCCOL)))
		return E_FAIL;
	if(FAILED(pBufferMgr->AddBuffer(TEXT("RcTex"), BUFFERTYPE_RCTEX)))
		return E_FAIL;
	if(FAILED(pBufferMgr->AddBuffer(TEXT("CubeTex"), BUFFERTYPE_CUBE)))
		return E_FAIL;
	if(FAILED(pBufferMgr->AddBuffer(TEXT("UITex"), BUFFERTYPE_UI)))
		return E_FAIL;

	CObjMgr::GetInstance()->Initialize();

	// 타일
	if(FAILED(pTextureMgr->InsertTexture(TEXT("../Texture/Tile/Tile%d.png"), TEXT("Tile"), TEXTYPE_MULTI, TEXT("Tile"), TEXT(""), 18)))
		return E_FAIL;
	if(FAILED(pTextureMgr->InsertTexture(TEXT("../Texture/Tile/TileA%d.png"), TEXT("Tile"), TEXTYPE_MULTI, TEXT("TileA"), TEXT(""), 18)))
		return E_FAIL;
	if(FAILED(pTextureMgr->InsertTexture(TEXT("../Texture/Tile/TileB%d.png"), TEXT("Tile"), TEXTYPE_MULTI, TEXT("TileB"), TEXT(""), 18)))
		return E_FAIL;

	// 아이템
	pTextureMgr->ReadMultiTexture(TEXT("../Data/Path/ItemPath.xml"));

	// 몬스터
	pTextureMgr->ReadMultiTexture(TEXT("../Data/Path/MonsterPath.xml"));

	// 이펙트
	pTextureMgr->ReadMultiTexture(TEXT("../Data/Path/EffectPath.xml"));

	// UI
	pTextureMgr->ReadMultiTexture(TEXT("../Data/Path/UIPath.xml"));

	// 텍스트
	pTextureMgr->ReadMultiTexture(TEXT("../Data/Path/TextPath.xml"));

	// 기타 오브젝트
	pTextureMgr->ReadMultiTexture(TEXT("../Data/Path/ObjPath.xml"));

	pTextureMgr->InsertTexture(TEXT("../Texture/UI/Logo/Normal/Normal_%d.png"),
		TEXT("Loading"), TEXTYPE_MULTI, TEXT("Normal"), TEXT(""), 6);

	if(FAILED(pTextureMgr->CubeTextureInsert()))
		return E_FAIL;

	pTextureMgr->SetPathName(TEXT("Loading Complete"));

	_endthreadex(0);

	return 0;
}