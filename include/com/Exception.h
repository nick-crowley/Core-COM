#pragma once
#include "library/core.COM.h"

namespace core::com
{
	[[noreturn]] 
	void 
	throw_exception(::HRESULT hr, std::source_location loc = std::source_location::current());

	[[noreturn]] 
	void 
	throw_exception(::HRESULT hr, std::string_view msg, std::source_location loc = std::source_location::current());
}
