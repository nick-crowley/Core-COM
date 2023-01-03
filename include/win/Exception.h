#pragma once
//#include "comFramework.h"       // Warning: To avoid circular dependency, include <comFramework.h> directly
#include "../src/PlatformSdk.h"
#include "../src/StdLibrary.h"

namespace win
{
	[[noreturn]] 
	void 
	throw_exception(::LRESULT r);

	[[noreturn]] 
	void 
	throw_exception(::LRESULT r, std::string message);
}
