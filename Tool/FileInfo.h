#pragma once

#include "Include.h"

class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static wstring ConvertRelativePath(const wstring& szFullPath);
	static CString GetFileName(const wstring& szFullPath, int iRelative = 0);
	static int GetFileNumber(CString& strFileName);
	static wstring ChooseSaveFolder(const wstring &szFolderName);
	static void DirtoryInfoExtraction(const wstring& wstrPath, list<IMGPATH*>& rPathInfoList);
	static void DirtoryInfoExtraction_Ex(const wstring& wstrPath, list<IMGPATH*>& rPathInfolist);
	static int DirtoryFileCount(const wstring& wstrPath);
};