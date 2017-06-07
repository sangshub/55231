#pragma once

#include "Include.h"

class CDevice;

class CTexture
{
public:
	CTexture();
	virtual ~CTexture();

public:
	virtual const TEXINFO* GetTexture(const wstring& wstrStateKey = TEXT(""), const wstring& wstrDirectionKey = TEXT(""), const int iCount = 0)PURE;

public:
	virtual HRESULT InsertTexture(const wstring& wstrFilePath, const wstring& wstrStateKey = TEXT(""), const wstring& wstrDirectionKey = TEXT(""), const int iCount = 0)PURE;

private:
	virtual void Release()PURE;

protected:
	CDevice* m_pDevice;
};