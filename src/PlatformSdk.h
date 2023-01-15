#pragma once

#ifndef _ATL_ATTRIBUTES
#	error Core-COM must be built with _ATL_ATTRIBUTES
#endif

#include <atlbase.h>               // This header must come first otherwise ATL headers won't compile
#pragma warning (disable : 4467)   // Disable:  warning C4467: usage of ATL attributes is deprecated

// Including SDKDDKVer.h defines the highest available Windows platform.
// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.
#include <SDKDDKVer.h>
#include <Windows.h>

// NB: Add library-specific Windows dependencies here
#include <Objbase.h>
