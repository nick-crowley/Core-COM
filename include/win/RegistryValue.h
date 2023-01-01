#pragma once
#include "comFramework.h"

namespace win
{
	using RegistryValue = std::variant<
		std::wstring_view,
		std::wstring,
		uint32_t,
		uint64_t,
		std::vector<std::byte>,
		std::vector<std::wstring>
	>;
}
