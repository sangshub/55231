#include "StdAfx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice(void)
: m_p3D(NULL)
, m_pDevice(NULL)
, m_pSprite(NULL)
, m_pFont(NULL)
, m_pLine(NULL)
, m_pSound(NULL)
{
}

CDevice::~CDevice(void)
{
	Release();
}

HRESULT CDevice::InitDevice(const WINMODE& eWinMode)
{
	m_p3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9	devicecaps;
	ZeroMemory(&devicecaps, sizeof(D3DCAPS9));

	if(FAILED(m_p3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &devicecaps)))
	{
		ERR_MSG(TEXT("Get DeviceCpas Fail"));

		return E_FAIL;
	}

	DWORD	vp;

	if(devicecaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp, eWinMode);

	if(FAILED(m_p3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL
		,g_hWnd, vp, &d3dpp, &m_pDevice)))
	{
		ERR_MSG(TEXT("Create Device Fail"));

		return E_FAIL;
	}

	if(FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		ERR_MSG(TEXT("Create Sprite Fail"));

		return E_FAIL;
	}

	D3DXFONT_DESCW		FontInfo;
	ZeroMemory(&FontInfo, sizeof(D3DXFONT_DESCW));

	FontInfo.Height = 20;
	FontInfo.Width = 10;
	FontInfo.Weight = FW_HEAVY;
	FontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(FontInfo.FaceName, TEXT("궁서"));

	if(FAILED(D3DXCreateFontIndirect(m_pDevice, &FontInfo, &m_pFont)))
	{
		ERR_MSG(TEXT("Create Font Fail"));

		return E_FAIL;
	}

	if(FAILED(D3DXCreateLine(m_pDevice, &m_pLine)))
	{
		ERR_MSG(TEXT("Create Line Fail"));

		return E_FAIL;
	}

	//사운드 디바이스 생성
	if(FAILED(DirectSoundCreate(NULL, &m_pSound, NULL)))
	{
		ERR_MSG(TEXT("Sound Device Create Fail"));

		return E_FAIL;
	}

	//사운드 디바이스 협조레벨 설정.
	if(FAILED(m_pSound->SetCooperativeLevel(g_hWnd, DSSCL_PRIORITY)))
	{
		ERR_MSG(TEXT("Sound Device CooperativeLevel Fail"));

		return E_FAIL;
	}

	LoadBGM();

	return S_OK;
}
HRESULT CDevice::LoadWave(TCHAR* pFileName)
{
	HMMIO	hFile;

	//CreateFile
	hFile = mmioOpen(pFileName, NULL, MMIO_READ);//wave파일을 연다.

	//정크구조체.
	MMCKINFO	pParent;
	memset(&pParent, 0, sizeof(pParent));
	pParent.fccType = mmioFOURCC('W','A','V','E');
	mmioDescend(hFile, &pParent, NULL, MMIO_FINDRIFF);

	MMCKINFO	pChild;
	memset(&pChild, 0, sizeof(pChild));
	pChild.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(hFile, &pChild, &pParent, MMIO_FINDCHUNK);

	WAVEFORMATEX	wft;
	memset(&wft, 0, sizeof(wft));
	mmioRead(hFile, (char*)&wft, sizeof(wft));

	mmioAscend(hFile, &pChild, 0);
	pChild.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hFile, &pChild, &pParent, MMIO_FINDCHUNK);

	DSBUFFERDESC	BuffInfo;
	memset(&BuffInfo, 0, sizeof(DSBUFFERDESC));
	BuffInfo.dwBufferBytes = pChild.cksize;
	BuffInfo.dwSize = sizeof(DSBUFFERDESC);
	BuffInfo.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE;
	BuffInfo.lpwfxFormat = &wft;

	LPDIRECTSOUNDBUFFER		SoundBuff;

	if(FAILED(m_pSound->CreateSoundBuffer(&BuffInfo, &SoundBuff
		, NULL)))
	{
		ERR_MSG(TEXT("SoundBuffer Create Fail"));

		return E_FAIL;
	}

	void *pWrite1 = NULL, *pWrite2 = NULL;
	DWORD dwlength1, dwlength2;

	SoundBuff->Lock(0, pChild.cksize, &pWrite1, &dwlength1
		, &pWrite2, &dwlength2, 0);

	if(pWrite1 > 0)
		mmioRead(hFile, (char*)pWrite1, dwlength1);
	if(pWrite2 > 0)
		mmioRead(hFile, (char*)pWrite2, dwlength2);

	SoundBuff->Unlock(pWrite1, dwlength1, pWrite2, dwlength2);	

	mmioClose(hFile, 0);

	wstring wstrFullPath = pFileName;

	CString strFullPath= wstrFullPath.c_str();

	strFullPath.Delete(0, 9);
	strFullPath.Delete(strFullPath.GetLength() - 4, 4);

	m_mapSoundBuff.insert(map<CString, LPDIRECTSOUNDBUFFER>::value_type(strFullPath, SoundBuff));
	
	return S_OK;
}

void CDevice::SoundPlay(const CString& strFileName, const DWORD dwFlag)
{
	map<CString, LPDIRECTSOUNDBUFFER>::iterator iter = m_mapSoundBuff.find(strFileName);

	if(iter == m_mapSoundBuff.end())
		return;

	iter->second->SetCurrentPosition(0);

	DWORD dwFre = 0;

	iter->second->GetFrequency(&dwFre);
	iter->second->Play(0, 0, dwFlag);
}

void CDevice::SoundStop(const CString& strFileName)
{
	map<CString, LPDIRECTSOUNDBUFFER>::iterator iter = m_mapSoundBuff.find(strFileName);

	if(iter == m_mapSoundBuff.end())
		return;

	iter->second->Stop();
}

bool CDevice::SoundPlaying(const CString& strFileName)
{
	map<CString, LPDIRECTSOUNDBUFFER>::iterator iter = m_mapSoundBuff.find(strFileName);

	if(iter == m_mapSoundBuff.end())
		return false;
	
	DWORD dwStatus = 0;
	iter->second->GetStatus(&dwStatus);

	if(dwStatus & DSBSTATUS_PLAYING)
		return true;

	return false;
}

void CDevice::SetSoundVolume(const CString& strFileName, const int iVolume)
{
	map<CString, LPDIRECTSOUNDBUFFER>::iterator iter = m_mapSoundBuff.find(strFileName);

	if(iter == m_mapSoundBuff.end())
		return;

	iter->second->SetVolume(iVolume);
}

void CDevice::LoadBGM()
{
	// BGM
	LoadWave(TEXT("../Sound/Intro.wav"));
	LoadWave(TEXT("../Sound/Menu.wav"));
	LoadWave(TEXT("../Sound/Tutorial.wav"));
	LoadWave(TEXT("../Sound/Zone1_1.wav"));
	LoadWave(TEXT("../Sound/Zone1_1_Shop.wav"));
	LoadWave(TEXT("../Sound/Zone2_1.wav"));
	LoadWave(TEXT("../Sound/Zone2_1_Shop.wav"));
	LoadWave(TEXT("../Sound/Zone3_1.wav"));
	LoadWave(TEXT("../Sound/Zone3_1_Shop.wav"));
	LoadWave(TEXT("../Sound/Boss_1.wav"));

	// EFFECT
	LoadWave(TEXT("../Sound/Bat_Attack.wav"));
	LoadWave(TEXT("../Sound/Bat_Hit.wav"));
	LoadWave(TEXT("../Sound/Bat_Death.wav"));

	LoadWave(TEXT("../Sound/Slime_Attack.wav"));
	LoadWave(TEXT("../Sound/Slime_Hit.wav"));
	LoadWave(TEXT("../Sound/Slime_Death.wav"));

	LoadWave(TEXT("../Sound/Ghost_Attack.wav"));
	LoadWave(TEXT("../Sound/Ghost_Death.wav"));

	LoadWave(TEXT("../Sound/Dragon_Attack.wav"));
	LoadWave(TEXT("../Sound/Dragon_Hit.wav"));
	LoadWave(TEXT("../Sound/Dragon_Death.wav"));
	LoadWave(TEXT("../Sound/Dragon_Cry.wav"));
	LoadWave(TEXT("../Sound/Dragon_Move.wav"));

	LoadWave(TEXT("../Sound/Monkey_Hit.wav"));
	LoadWave(TEXT("../Sound/Monkey_Grab.wav"));

	LoadWave(TEXT("../Sound/Skel_Attack.wav"));
	LoadWave(TEXT("../Sound/Skel_Hit.wav"));
	LoadWave(TEXT("../Sound/Skel_Head_Loss.wav"));

	LoadWave(TEXT("../Sound/Wraith_Attack.wav"));
	LoadWave(TEXT("../Sound/Wraith_Death.wav"));
	LoadWave(TEXT("../Sound/Wraith_Cry.wav"));

	LoadWave(TEXT("../Sound/BigBat_Attack.wav"));
	LoadWave(TEXT("../Sound/BigBat_Hit.wav"));
	LoadWave(TEXT("../Sound/BigBat_Death.wav"));

	LoadWave(TEXT("../Sound/Minotaur_Attack.wav"));
	LoadWave(TEXT("../Sound/Minotaur_Charge.wav"));
	LoadWave(TEXT("../Sound/Minotaur_Cry.wav"));
	LoadWave(TEXT("../Sound/Minotaur_Death.wav"));
	LoadWave(TEXT("../Sound/Minotaur_Hit.wav"));
	LoadWave(TEXT("../Sound/Minotaur_Stun.wav"));

	LoadWave(TEXT("../Sound/Banshee_Attack.wav"));
	LoadWave(TEXT("../Sound/Banshee_Cry.wav"));
	LoadWave(TEXT("../Sound/Banshee_Death.wav"));
	LoadWave(TEXT("../Sound/Banshee_Hit.wav"));
	LoadWave(TEXT("../Sound/Banshee_Loop.wav"));

	LoadWave(TEXT("../Sound/Golem_Attack.wav"));
	LoadWave(TEXT("../Sound/Golem_Death.wav"));
	LoadWave(TEXT("../Sound/Golem_Hit.wav"));
	LoadWave(TEXT("../Sound/Golem_Move.wav"));

	LoadWave(TEXT("../Sound/Player_Blunderbuss.wav"));
	LoadWave(TEXT("../Sound/Player_FireBall.wav"));
	LoadWave(TEXT("../Sound/Player_Freeze.wav"));
	LoadWave(TEXT("../Sound/Player_Hit.wav"));
	LoadWave(TEXT("../Sound/Player_Weapon_Pick.wav"));
	LoadWave(TEXT("../Sound/Player_Att1.wav"));
	LoadWave(TEXT("../Sound/Player_Att2.wav"));
	LoadWave(TEXT("../Sound/Player_Att3.wav"));
	LoadWave(TEXT("../Sound/Player_Att4.wav"));
	LoadWave(TEXT("../Sound/Player_Dig.wav"));
	LoadWave(TEXT("../Sound/Player_DigFail.wav"));
	LoadWave(TEXT("../Sound/Player_Food_Pick.wav"));
	LoadWave(TEXT("../Sound/Player_Gold_Pick.wav"));
	LoadWave(TEXT("../Sound/Player_Heal.wav"));
	LoadWave(TEXT("../Sound/Player_Spell_Heal.wav"));
	LoadWave(TEXT("../Sound/Player_Damage.wav"));
	LoadWave(TEXT("../Sound/ComboStart.wav"));
	LoadWave(TEXT("../Sound/ComboStop.wav"));

	LoadWave(TEXT("../Sound/Bomb_Ready.wav"));
	LoadWave(TEXT("../Sound/Bomb.wav"));
	LoadWave(TEXT("../Sound/Door.wav"));
	LoadWave(TEXT("../Sound/Trap_Move.wav"));
	LoadWave(TEXT("../Sound/Trap_Spike.wav"));

	LoadWave(TEXT("../Sound/Harpoon_Fire.wav"));

	LoadWave(TEXT("../Sound/Boss_Init.wav"));
	LoadWave(TEXT("../Sound/Boss_Smash.wav"));
	LoadWave(TEXT("../Sound/Boss_Chase.wav"));
	LoadWave(TEXT("../Sound/Boss_In.wav"));
	LoadWave(TEXT("../Sound/Boss_Out.wav"));
	LoadWave(TEXT("../Sound/Boss_Ok.wav"));
	LoadWave(TEXT("../Sound/Boss_Fail.wav"));
	LoadWave(TEXT("../Sound/Boss_Attack1.wav"));
	LoadWave(TEXT("../Sound/Boss_Attack2.wav"));
	LoadWave(TEXT("../Sound/Boss_Hit.wav"));
	LoadWave(TEXT("../Sound/Boss_Welcome.wav"));
	LoadWave(TEXT("../Sound/Boss_Pattren1.wav"));
	LoadWave(TEXT("../Sound/Boss_Pattren2.wav"));
	LoadWave(TEXT("../Sound/Boss_Death.wav"));

	LoadWave(TEXT("../Sound/Pickup_Purchase.wav"));
	LoadWave(TEXT("../Sound/Pickup_Weapon.wav"));
	LoadWave(TEXT("../Sound/Player_Armor.wav"));
	LoadWave(TEXT("../Sound/Chest_Open.wav"));
	LoadWave(TEXT("../Sound/Kwang.wav"));

	LoadWave(TEXT("../Sound/StageEnd0.wav"));
	LoadWave(TEXT("../Sound/StageEnd1.wav"));
}

void CDevice::SetParameters(D3DPRESENT_PARAMETERS& d3dpp, const WINMODE& eWinMode)
{
	d3dpp.BackBufferWidth = WINCX;
	d3dpp.BackBufferHeight = WINCY;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dpp.MultiSampleQuality = 0;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;

	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.EnableAutoDepthStencil = TRUE;

	d3dpp.hDeviceWindow = g_hWnd;
	d3dpp.Windowed = eWinMode;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

void CDevice::Release(void)
{
	::Safe_Release(m_pLine);
	::Safe_Release(m_pFont);
	::Safe_Release(m_pSprite);
	::Safe_Release(m_pDevice);
	::Safe_Release(m_p3D);

	for_each(m_mapSoundBuff.begin(), m_mapSoundBuff.end(), ::ReleaseMapObj());
	m_mapSoundBuff.clear();

	::Safe_Release(m_pSound);
}

void CDevice::Render_Begin(void)
{
	m_pDevice->Clear(0, NULL, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
		, D3DCOLOR_ARGB(255, 0, 0, 0), 1.f, 0);
	m_pDevice->BeginScene();
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DX_FILTER_NONE);
}

void CDevice::Render_End(void)
{
	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, NULL, NULL);
}

const LPDIRECT3DDEVICE9 CDevice::GetDevice(void) const {return m_pDevice;}
const LPD3DXSPRITE CDevice::GetSprite(void) const {return m_pSprite;}
const LPD3DXFONT CDevice::GetFont(void) const {return m_pFont;}
const LPD3DXLINE CDevice::GetLine(void) const {return m_pLine;}

void CDevice::SetRenderState(const _D3DRENDERSTATETYPE& eType, const DWORD dwValue)
{
	m_pDevice->SetRenderState(eType, dwValue);
}

void CDevice::SetTransform(const _D3DTRANSFORMSTATETYPE& eType, const D3DXMATRIX* pMatrix)
{
	m_pDevice->SetTransform(eType, pMatrix);
}