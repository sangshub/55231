#pragma once

#include "../Headers/Include.h"

class CDevice
{
public:
	DECLARE_SINGLETON(CDevice)

private:
	CDevice(void);

public:
	~CDevice(void);

public:
	const LPDIRECT3DDEVICE9 GetDevice(void) const;
	const LPD3DXSPRITE GetSprite(void) const;
	const LPD3DXFONT GetFont(void) const;
	const LPD3DXLINE GetLine(void) const;

public:
	void SetRenderState(_D3DRENDERSTATETYPE eType, DWORD dwValue);
	void SetTransform(_D3DTRANSFORMSTATETYPE eType, const D3DXMATRIX* pMatrix);

public:
	HRESULT InitDevice(void);
	void Render_Begin(void);
	void Render_End(HWND hWnd = g_hWnd);

private:
	void SetParameters(D3DPRESENT_PARAMETERS& d3dpp);
	void Release(void);

private:
	LPDIRECT3D9			m_p3D;
	LPDIRECT3DDEVICE9	m_pDevice;
	LPD3DXSPRITE		m_pSprite;
	LPD3DXFONT			m_pFont;
	LPD3DXLINE			m_pLine;
};