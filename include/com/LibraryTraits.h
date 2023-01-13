#pragma once
#include "comFramework.h"
#include "com/Guid.h"

namespace core::com
{
	template <typename Library>
	struct library_traits
	{
		auto constexpr 
		static library_name = Library::library_name;

		auto constexpr 
		static library_version = Library::library_version;

		auto const
		inline static library_guid = Guid{ __uuidof(Library) }; 
	};
}
