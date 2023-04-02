#pragma once
#ifndef ComFramework_hS_included
#	define ComFramework_h_included
#endif

#ifndef _ATL_ATTRIBUTES
	#define _ATL_ATTRIBUTES 
#endif

#ifdef _INC_WINDOWS
#	error <Windows.h> has been included prior <core.COM.h>
#endif

#include "library/core.Platform.h"

#include "../../src/PlatformSdk.h"
#include "../../src/libBoost.h"

#include "../../src/library/ComExport.h"

#include "com/Concepts.h"
