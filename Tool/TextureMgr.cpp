#include "stdafx.h"
#include "TextureMgr.h"
#include "Device.h"
#include "Texture.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
: m_pDevice(CDevice::GetInstance())
{
}

CTextureMgr::~CTextureMgr()
{
	Release();
}

void CTextureMgr::Release()
{
	for_each(m_MapTexture.begin(), m_MapTexture.end(), ::CDeleteMapObj());
	m_MapTexture.clear();

	for(map<BYTE, LPDIRECT3DCUBETEXTURE9>::iterator iter = m_mapCubeTex.begin();
		iter != m_mapCubeTex.end(); ++iter)
		iter->second->Release();
	m_mapCubeTex.clear();
}

void CTextureMgr::SetPathName(const wstring& wstrImgPath)
{
	m_wstrImgPath = wstrImgPath;
}

const TEXINFO* CTextureMgr::GetTexture(const wstring& wstrObjKey, const wstring& wstrStateKey, const wstring& wstrDirectionKey, const int iCount)
{
	map<wstring, CTexture*>::const_iterator iter = m_MapTexture.find(wstrObjKey);

	if(iter == m_MapTexture.end())
		return NULL;

	return iter->second->GetTexture(wstrStateKey, wstrDirectionKey, iCount);
}

int CTextureMgr::GetImgCount(const wstring& wstrObjKey, const wstring& wstrStateKey, const wstring& wstrDirectionKey)
{
	map<wstring, CTexture*>::const_iterator iter = m_MapTexture.find(wstrObjKey);

	if(iter == m_MapTexture.end())
		return NULL;

	return ((CMultiTexture*)iter->second)->GetCount(wstrStateKey, wstrDirectionKey);
}

const wstring& CTextureMgr::GetPathName() const
{
	return m_wstrImgPath;
}

HRESULT CTextureMgr::InsertTexture(const wstring &wstrFilePath, const wstring &wstrObjKey, TEXTYPE eTexType, const wstring &wstrStateKey, const wstring &wstrDirectionKey, const int iCount)
{
	map<wstring, CTexture*>::const_iterator iter = m_MapTexture.find(wstrObjKey);

	if(iter != m_MapTexture.end() && eTexType == TEXTYPE_MULTI)
	{
		if(FAILED(m_MapTexture[wstrObjKey]->InsertTexture(wstrFilePath, wstrStateKey, wstrDirectionKey, iCount)))
		{
			ERR_MSG(TEXT("MultiTexture Insert Fail"));

			return E_FAIL;
		}
		SetPathName(wstrFilePath);

		return S_OK;
	}

	if(iter == m_MapTexture.end())
	{
		CTexture* pTexture = NULL;

		switch(eTexType)
		{
		case TEXTYPE_SINGLE:
			pTexture = new CSingleTexture;
			break;
		case TEXTYPE_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if(FAILED(pTexture->InsertTexture(wstrFilePath, wstrStateKey, wstrDirectionKey, iCount)))
		{
			ERR_MSG(TEXT("SingleTexture Insert Fail"));

			return E_FAIL;
		}
		SetPathName(wstrFilePath);

		m_MapTexture.insert(map<wstring, CTexture*>::value_type(wstrObjKey, pTexture));
	}
	else
	{
		if(eTexType == TEXTYPE_MULTI)
		{
			SetPathName(wstrFilePath);

			iter->second->InsertTexture(wstrFilePath, wstrStateKey, wstrDirectionKey, iCount);
		}
	}
	return S_OK;
}
// 큐브 관련 함수들 //////////////////////////////////////////////
const LPDIRECT3DCUBETEXTURE9 CTextureMgr::GetCubeTex(BYTE byDrawId)
{
	map<BYTE, LPDIRECT3DCUBETEXTURE9>::iterator iter = m_mapCubeTex.find(byDrawId);
	if(iter == m_mapCubeTex.end())
		return NULL;
	return iter->second;
}

HRESULT CTextureMgr::InsertCubeTexture(const wstring& wstrFilePath, BYTE byDrawId)
{
	map<BYTE, LPDIRECT3DCUBETEXTURE9>::iterator iter = m_mapCubeTex.find(byDrawId);
	if(iter != m_mapCubeTex.end())
		return E_FAIL;

	LPDIRECT3DCUBETEXTURE9 pCubeTexture;

	if(FAILED(D3DXCreateCubeTextureFromFile(m_pDevice->GetDevice()
		, wstrFilePath.c_str(), &pCubeTexture)))
	{
		ERR_MSG(L"CubeTexture Load false");
		return E_FAIL;
	}

	m_mapCubeTex.insert(map<BYTE, LPDIRECT3DCUBETEXTURE9>::value_type(byDrawId,pCubeTexture));

	return S_OK;
}

map<BYTE, LPDIRECT3DCUBETEXTURE9>* CTextureMgr::GetCubeTexMap(void)
{
	return &m_mapCubeTex;
}

////////////////////////////////////////////////////////////////////