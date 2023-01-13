#pragma once
#include "comFramework.h"
#include "com/Version.h"
#include "core/LiteralString.h"

[uuid("A10C8092-3549-4C2E-95D7-F264286720B9")]
struct CustomLib_t
{
	auto constexpr 
	static library_name = core::LiteralString{L"CustomLib"};

	auto constexpr 
	static library_version = core::com::version{1,0};
};
