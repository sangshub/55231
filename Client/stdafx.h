// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C의 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
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