#pragma once
#include "comFramework.h"
#include "com/Version.h"
#include "core/ConstantString.h"

[uuid("A10C8092-3549-4C2E-95D7-F264286720B9")]
struct CustomLib_t
{
	auto constexpr 
	static library_name = ConstantString{L"CustomLib"};

	auto constexpr 
	static library_version = com::version{1,0};
};
