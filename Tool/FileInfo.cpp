#include "stdafx.h"
#include "FileInfo.h"

CFileInfo::CFileInfo()
{
}

CFileInfo::~CFileInfo()
{
}

wstring CFileInfo::ConvertRelativePath(const wstring& szFullPath)
{
	TCHAR szRelativePath[MAX_PATH] = TEXT("");
	TCHAR szDirtoryPath[MAX_PATH] = TEXT("");

	GetCurrentDirectory(sizeof(szDirtoryPath), szDirtoryPath);
	PathRelativePathTo((LPWSTR)szRelativePath, szDirtoryPath, FILE_ATTRIBUTE_DIRECTORY, 
		szFullPath.c_str(), FILE_ATTRIBUTE_NORMAL);

	return szRelativePath;
}

CString CFileInfo::GetFileName(const wstring& szFullPath, int iRelative)
{
	CString strFileName = TEXT("");

	if(iRelative == 0)
	{
		wstring wstrRelativePath = ConvertRelativePath(szFullPath);
		strFileName = PathFindFileName(wstrRelativePath.c_str());
		PathRemoveExtension((LPWSTR)strFileName.operator LPCWSTR());
	}
	else
	{
		strFileName = PathFindFileName(szFullPath.c_str());
		PathRemoveExtension((LPWSTR)strFileName.operator LPCWSTR());
	}
	return strFileName;
}

int CFileInfo::GetFileNumber(CString& strFileName)
{
	int iFileNumber = 0;

	int i = 0;

	for(i = 0; i < strFileName.GetLength(); ++i)
	{
		if(isdigit(strFileName[i]) != 0)
			break;
	}
	strFileName.Delete(0, i);

	iFileNumber = _tstoi(strFileName);

	return iFileNumber;
}

wstring CFileInfo::ChooseSaveFolder(const wstring &szFolderName)
{
	TCHAR szDirtoryPath[MAX_PATH] = TEXT("");

	GetCurrentDirectory(sizeof(szDirtoryPath), szDirtoryPath);
	SetCurrentDirectory(szDirtoryPath);

	PathRemoveFileSpec(szDirtoryPath);
	lstrcat(szDirtoryPath, szFolderName.c_str());

	return szDirtoryPath;
}

void CFileInfo::DirtoryInfoExtraction(const wstring& wstrPath, list<IMGPATH*>& rPathInfoList)
{
	CFileFind Find;
	Find.FindFile(wstrPath.c_str());

	int iContinue = 1;

	while(iContinue)
	{
		iContinue = Find.FindNextFile();

		if(Find.IsDots())
			continue;
		if(Find.IsDirectory())
			DirtoryInfoExtraction(wstring(Find.GetFilePath() + TEXT("\\*.*")), rPathInfoList);
		else
		{
			if(Find.IsSystem())
				continue;

			IMGPATH* pImgPath = new IMGPATH;
			TCHAR szTempPath[MAX_PATH] = TEXT("");
			lstrcpy(szTempPath, Find.GetFilePath().operator LPCWSTR());
			PathRemoveFileSpec(szTempPath);

			pImgPath->iCount = DirtoryFileCount(szTempPath);

			wstring wstrTextureName = Find.GetFileTitle().operator LPCWSTR();
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + TEXT("%d.png");

			TCHAR szPath[MAX_PATH] = TEXT("");
			lstrcpy(szPath, (LPWSTR)Find.GetFilePath().operator LPCWSTR());
			PathRemoveFileSpec(szPath);

			PathCombine(szPath, szPath, (LPWSTR)wstrTextureName.c_str());
			pImgPath->wstrPath = ConvertRelativePath(szPath);

			/*PathRemoveFileSpec(szPath);
			pImgPath->wstrDirectionKey = PathFindFileName(szPath);*/

			PathRemoveFileSpec(szPath);
			pImgPath->wstrStateKey = PathFindFileName(szPath);

			PathRemoveFileSpec(szPath);
			pImgPath->wstrObjKey = PathFindFileName(szPath);

			rPathInfoList.push_back(pImgPath);

			return;
		}
	}
}

void CFileInfo::DirtoryInfoExtraction_Ex(const wstring& wstrPath, list<IMGPATH*>& rPathInfolist)
{
	CFileFind		Find;
	Find.FindFile(wstrPath.c_str());

	int		iContinue = 1;
	while(iContinue)
	{
		iContinue = Find.FindNextFile();

		if(Find.IsDots())
			continue;

		if(Find.IsDirectory())
		{
			DirtoryInfoExtraction_Ex( wstring(Find.GetFilePath() + L"\\*.*"), rPathInfolist);
		}
		else
		{
			if(Find.IsSystem()) 
				continue;

			IMGPATH*		pImgPath = new IMGPATH;
			TCHAR			szTempPath[MAX_PATH] = L"";
			lstrcpy(szTempPath, Find.GetFilePath().operator LPCWSTR());
			PathRemoveFileSpec(szTempPath);
			pImgPath->iCount = DirtoryFileCount(szTempPath);

			wstring		wstrTextureName = Find.GetFileTitle().operator LPCWSTR();
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";

			//이미지 이름 변환
			TCHAR		szPath[MAX_PATH] = L"";
			lstrcpy(szPath, (LPWSTR)Find.GetFilePath().operator LPCWSTR());
			PathRemoveFileSpec(szPath);

			//상대 경로
			PathCombine(szPath, szPath, (LPWSTR)wstrTextureName.c_str());
			pImgPath->wstrPath = ConvertRelativePath(szPath);

			//StateKey
			PathRemoveFileSpec(szPath);
			pImgPath->wstrStateKey = PathFindFileName(szPath);

			//ObjKey
			PathRemoveFileSpec(szPath);
			pImgPath->wstrObjKey = PathFindFileName(szPath);

			rPathInfolist.push_back(pImgPath);

			return;
		}
	}
}


int CFileInfo::DirtoryFileCount(const wstring& wstrPath)
{
	wstring wstrTemp = wstrPath + TEXT("\\*.png");

	CFileFind Find;
	Find.FindFile(wstrTemp.c_str());

	int iFileCount = 0;
	int iContinue = 1;

	while(iContinue)
	{
		iContinue = Find.FindNextFile();

		if(!Find.IsDirectory())
			++iFileCount;
	}
	return iFileCount;
}