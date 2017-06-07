#pragma once

#include "Include.h"

class CTexture;
class CDevice;
class CTextureMgr
{
public:
	DECLARE_SINGLETON(CTextureMgr)

private:
	CTextureMgr();

public:
	~CTextureMgr();

public:
	void SetPathName(const wstring& wstrImgPath);
	const TEXINFO* GetTexture(const wstring& wstrObjKey, const wstring& wstrStateKey = TEXT(""), const wstring& wstrDirectionKey = TEXT(""), const int iCount = 0);
	int GetImgCount(const wstring& wstrObjKey, const wstring& wstrStateKey, const wstring& wstrDirectionKey);
	const wstring& GetPathName() const;
	const LPDIRECT3DCUBETEXTURE9 GetCubeTex(BYTE byDrawId);
	map<BYTE, LPDIRECT3DCUBETEXTURE9>* GetCubeTexMap(void);
	HRESULT CubeTextureInsert(void);

public:
	HRESULT InsertTexture(const wstring& wstrFilePath, const wstring& wstrObjKey, TEXTYPE eTexType, const wstring& wstrStateKey = TEXT(""), const wstring& wstrDirectionKey = TEXT(""), const int iCount = 0);
	HRESULT InsertCubeTexture(const wstring& wstrFilePath, BYTE byDrawId);
	void ReadMultiTexture(const wstring& wstrImgPath);

private:
	void Release();

private:
	wstring m_wstrImgPath;
	map<wstring, CTexture*> m_MapTexture;
	map<BYTE, LPDIRECT3DCUBETEXTURE9> m_mapCubeTex;

private:
	CDevice*	m_pDevice;
};