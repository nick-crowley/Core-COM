#pragma once
#include "comFramework.h"

namespace nt
{
	[[noreturn]] void 
	throw_exception(::NTSTATUS status);
}
