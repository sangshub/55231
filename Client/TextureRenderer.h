#pragma once

#include "..\headers\renbase.h"

struct __declspec(uuid("{71771540-2017-11cf-ae26-0020afd79767}")) CLSID_TextureRenderer;

class CTextureRenderer : public CBaseVideoRenderer
{
private:
	LPDIRECT3DDEVICE9	m_pDevice;

private:
	LONG m_lVidWidth;   // Video width
	LONG m_lVidHeight;  // Video Height
	LONG m_lVidPitch;   // Video Pitch

private:
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DFORMAT m_TextureFormat;

public:
	CTextureRenderer(LPUNKNOWN pUnk, HRESULT* phr);
	~CTextureRenderer();

public:
	HRESULT CheckMediaType(const CMediaType *pmt );     // Format acceptable?
	HRESULT SetMediaType(const CMediaType *pmt );       // Video format notification
	HRESULT DoRenderSample(IMediaSample *pMediaSample); // New video sample

public:
	void SetTexture(DWORD dwStage);

public:
	void ReleaseTexture(void);
};
