#include "StdAfx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice(void)
: m_p3D(NULL)
, m_pDevice(NULL)
, m_pSprite(NULL)
, m_pFont(NULL)
, m_pLine(NULL)
{
}

CDevice::~CDevice(void)
{
	Release();
}

HRESULT CDevice::InitDevice(void)
{
	m_p3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9		devicecaps;
	ZeroMemory(&devicecaps, sizeof(D3DCAPS9));

	if(FAILED(m_p3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &devicecaps)))
	{
		ERR_MSG(TEXT("Get DeviceCaps Fail"));

		return E_FAIL;
	}

	DWORD		vp;

	if(devicecaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp);

	if(FAILED(m_p3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL
		, g_hWnd, vp, &d3dpp, &m_pDevice)))
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
	lstrcpy(FontInfo.FaceName, TEXT("±Ã¼­"));

	if(FAILED(D3DXCreateFontIndirect(m_pDevice, &FontInfo, &m_pFont)))
	{
		ERR_MSG(TEXT("Font Create Fail"));

		return E_FAIL;
	}

	if(FAILED(D3DXCreateLine(m_pDevice, &m_pLine)))
	{
		ERR_MSG(TEXT("Line Create Fail"));

		return E_FAIL;
	}

	return S_OK;
}

void CDevice::SetParameters(D3DPRESENT_PARAMETERS& d3dpp)
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
	d3dpp.Windowed = TRUE;

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
}

void CDevice::Render_Begin(void)
{
	m_pDevice->Clear(0, NULL, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
		, D3DCOLOR_ARGB(255, 0, 0, 0), 1.f, 0);
	m_pDevice->BeginScene();
}

void CDevice::Render_End(HWND hWnd)
{
	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, hWnd, NULL);
}

const LPDIRECT3DDEVICE9 CDevice::GetDevice(void) const {return m_pDevice;}
const LPD3DXSPRITE CDevice::GetSprite(void) const {return m_pSprite;}
const LPD3DXFONT CDevice::GetFont(void) const {return m_pFont;}
const LPD3DXLINE CDevice::GetLine(void) const {return m_pLine;}

void CDevice::SetRenderState(_D3DRENDERSTATETYPE eType, DWORD dwValue)
{
	m_pDevice->SetRenderState(eType, dwValue);
}

void CDevice::SetTransform(_D3DTRANSFORMSTATETYPE eType, const D3DXMATRIX* pMatrix)
{
	m_pDevice->SetTransform(eType, pMatrix);
}