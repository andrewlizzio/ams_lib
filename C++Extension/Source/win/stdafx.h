// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <Windowsx.h>
#include <Commctrl.h>
#include <Shlwapi.h>
#include <MMSystem.h>
#include <Psapi.h>
#include <Iphlpapi.h>

#pragma comment(lib, "IPHLPAPI.lib")

#ifndef WM_MOUSEHWHEEL
    #define WM_MOUSEHWHEEL 0x020E
#endif


// TODO: reference additional headers your program requires here
