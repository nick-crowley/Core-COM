#pragma once
#ifndef ComFramework_h_included
#	error Including this header directly may cause a circular dependency; include <comFramework.h> directly
#endif
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
