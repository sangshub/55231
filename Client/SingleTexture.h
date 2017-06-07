#pragma once

#include "Texture.h"

class CSingleTexture : public CTexture
{
public:
	CSingleTexture();
	~CSingleTexture();

public:
	virtual const TEXINFO* GetTexture(const wstring& wstrStateKey = TEXT(""), const wstring& wstrDirectionKey = TEXT(""), const int iCount = 0);

public:
	HRESULT InsertTexture(const wstring& wstrFilePath, const wstring& wstrStateKey = TEXT(""), const wstring& wstrDirectionKey = TEXT(""), const int iCount = 0);

private:
	virtual void Release();

private:
	TEXINFO* m_pTexInfo;
};