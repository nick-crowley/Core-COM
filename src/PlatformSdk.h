#pragma once

#ifndef _ATL_ATTRIBUTES
	#define _ATL_ATTRIBUTES 
#endif

#include <atlbase.h>               // This header must come first otherwise ATL headers won't compile
#pragma warning (disable : 4467)   // Disable:  warning C4467: usage of ATL attributes is deprecated

// Including SDKDDKVer.h defines the highest available Windows platform.
// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <Windows.h>

#include <Objbase.h>
