// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C�� ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#define MARKUP_STL

#include <vector>
#include <map>
#include <list>
#include <time.h>
#include <string>
#include <d3dx9.h>
#include <d3d9.h>
#include <algorithm>
#include <fstream>
#include <Shlwapi.h>
#include <atlstr.h>
#include <process.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")

#include <MMSystem.h>
#include <dsound.h>
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dsound.lib")


#include <crtdbg.h>

//DirectShow-----------------------------------------------
#include <streams.h>

#pragma comment(lib, "winmm.lib")
#ifdef _DEBUG
#pragma comment(lib, "lib/Strmbasd.lib")
#else
#pragma comment(lib, "lib/Strmbase.lib")
#endif
#pragma comment(lib, "lib/Strmiids.lib")
//---------------------------------------------------------


#ifdef _DEBUG
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif