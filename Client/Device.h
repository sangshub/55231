#pragma once

#include "Include.h"

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
	void SetRenderState(const _D3DRENDERSTATETYPE& eType, const DWORD dwValue);
	void SetTransform(const _D3DTRANSFORMSTATETYPE& eType, const D3DXMATRIX* pMatrix);

public:
	HRESULT InitDevice(const WINMODE& eWinMode);
	void Render_Begin(void);
	void Render_End(void);

	
	HRESULT LoadWave(TCHAR* pFileName);	//���ϰ�θ� ���� ������ �о� �帮�ڴ�.
	void SoundPlay(const CString& strFileName, const DWORD dwFlag/*���ѷ���, �ѹ�*/); //������ ��� �ϰڴ�.
	void SoundStop(const CString& strFileName);
	bool SoundPlaying(const CString& strFileName);	// ���� iIndex��° ���尡 ����ǰ��ִ��� �ƴ���.
	void LoadBGM();
	void SetSoundVolume(const CString& strFileName, const int iVolume);

private:
	void SetParameters(D3DPRESENT_PARAMETERS& d3dpp, const WINMODE& eWinMode);
	void Release(void);

private:
	LPDIRECT3D9			m_p3D;
	LPDIRECT3DDEVICE9	m_pDevice;
	LPD3DXSPRITE		m_pSprite;
	LPD3DXFONT			m_pFont;
	LPD3DXLINE			m_pLine;
	LPDIRECTSOUND				m_pSound; //����ī�带 ��ǥ�ϴ� ��ü.
	map<CString, LPDIRECTSOUNDBUFFER> m_mapSoundBuff;
};