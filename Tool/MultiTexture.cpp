#include "stdafx.h"
#include "MultiTexture.h"
#include "Device.h"

CMultiTexture::CMultiTexture()
{
}

CMultiTexture::~CMultiTexture()
{
	Release();
}

void CMultiTexture::Release()
{
	for(map<wstring, vector<TEXINFO*>>::iterator iter = m_MapMultiTexture.begin();
		iter != m_MapMultiTexture.end(); ++iter)
	{
		for_each(iter->second.begin(), iter->second.end(), ::CDeleteObj());
		iter->second.clear();
	}
	m_MapMultiTexture.clear();
}

const TEXINFO* CMultiTexture::GetTexture(const wstring& wstrStateKey, const wstring& wstrDirectionKey, const int iCount)
{
	map<wstring, vector<TEXINFO*>>::const_iterator iter = m_MapMultiTexture.find(wstrStateKey + TEXT("|") + wstrDirectionKey);

	if(iter == m_MapMultiTexture.end())
		return NULL;
	else
		return iter->second[iCount];
}

const int CMultiTexture::GetCount(const wstring& wstrStateKey, const wstring& wstrDirectionKey)
{
	map<wstring, vector<TEXINFO*>>::const_iterator iter = m_MapMultiTexture.find(wstrStateKey + TEXT("|") + wstrDirectionKey);

	if(iter == m_MapMultiTexture.end())
		return -1;

	return iter->second.size();
}

HRESULT CMultiTexture::InsertTexture(const wstring& wstrFilePath, const wstring& wstrStateKey, const wstring& wstrDirectionKey, const int iCount)
{
	TCHAR szPath[MAX_PATH] = TEXT("");
	vector<TEXINFO*> vecTexture;

	for(int i = 0; i < iCount; ++i)
	{
		CString strFilePath = wstrFilePath.c_str();

		if(i >= 10 && i < 100)
			strFilePath.Replace(TEXT("0%d"), TEXT("%d"));

		wsprintf(szPath, strFilePath, i);

		TEXINFO* pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		if(FAILED(D3DXGetImageInfoFromFile(szPath, &pTexInfo->ImgInfo)))
		{
			ERR_MSG(wstrFilePath.c_str());

			return E_FAIL;
		}

		if(FAILED(D3DXCreateTextureFromFileEx(m_pDevice->GetDevice()
			, szPath
			, pTexInfo->ImgInfo.Width
			, pTexInfo->ImgInfo.Height
			, pTexInfo->ImgInfo.MipLevels
			, 0
			, pTexInfo->ImgInfo.Format
			, D3DPOOL_MANAGED
			, D3DX_DEFAULT
			, D3DX_DEFAULT
			, NULL
			, &pTexInfo->ImgInfo
			, NULL
			, &pTexInfo->pTexture)))
		{
			return E_FAIL;
		}
		vecTexture.push_back(pTexInfo);
	}
	m_MapMultiTexture.insert(map<wstring, vector<TEXINFO*>>::value_type(wstrStateKey + TEXT("|") + wstrDirectionKey, vecTexture));

	return S_OK;
}