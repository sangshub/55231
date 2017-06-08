#pragma once

#include "Texture.h"

class CMultiTexture : public CTexture
{
public:
	CMultiTexture();
	~CMultiTexture();

public:
	virtual const TEXINFO* GetTexture(const wstring& wstrStateKey, const wstring& wstrDirectionKey, const int iCount);
	const int GetCount(const wstring& wstrStateKey, const wstring& wstrDirectionKey);

public:
	HRESULT InsertTexture(const wstring& wstrFilePath, const wstring& wstrStateKey, const wstring& wstrDirectionKey, const int iCount);

private:
	virtual void Release();

private:
	map<wstring, vector<TEXINFO*>> m_MapMultiTexture;
};