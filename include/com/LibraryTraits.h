#pragma once
#include "library/core.COM.h"
#include "core/LiteralString.h"
#include "com/Guid.h"
#include "com/Version.h"

namespace core::com
{
	template <typename Library>
	struct library_traits
	{
		LiteralString constexpr 
		static library_name = Library::library_name;

		version constexpr 
		static library_version = Library::library_version;

		Guid const
		inline static library_guid { __uuidof(Library) }; 
	};
}
