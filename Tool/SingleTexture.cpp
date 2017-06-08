#include "stdafx.h"
#include "SingleTexture.h"
#include "Device.h"

CSingleTexture::CSingleTexture()
: m_pTexInfo(NULL)
{
}

CSingleTexture::~CSingleTexture()
{
	Release();
}

void CSingleTexture::Release()
{
	m_pTexInfo->pTexture->Release();
	::Safe_Delete(m_pTexInfo);
}

const TEXINFO* CSingleTexture::GetTexture(const wstring& wstrStateKey, const wstring& wstrDirectionKey, const int iCount)
{
	return m_pTexInfo;
}

HRESULT CSingleTexture::InsertTexture(const wstring& wstrFilePath, const wstring& wstrStateKey, const wstring& wstrDirectionKey, const int iCount)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	if(FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &m_pTexInfo->ImgInfo)))
	{
		ERR_MSG(wstrFilePath.c_str());

		return E_FAIL;
	}

	if(FAILED(D3DXCreateTextureFromFileEx(m_pDevice->GetDevice()
		, wstrFilePath.c_str()
		, m_pTexInfo->ImgInfo.Width
		, m_pTexInfo->ImgInfo.Height
		, m_pTexInfo->ImgInfo.MipLevels
		, 0
		, m_pTexInfo->ImgInfo.Format
		, D3DPOOL_MANAGED
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, NULL
		, &m_pTexInfo->ImgInfo
		, NULL
		, &m_pTexInfo->pTexture)))
	{
		return E_FAIL;
	}
	return S_OK;
}