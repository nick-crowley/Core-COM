#pragma once
#include "library/core.COM.h"
#include "com/Guid.h"
#include "com/ClassFactory.h"
#include "com/Version.h"

namespace core::com
{
	template <meta::CoClass CoClass>
	struct coclass_traits
	{
		using factory_type = ClassFactory<CoClass>;
		using library_type = typename CoClass::library_type;

		auto constexpr 
		static class_name = CoClass::class_name;

		auto constexpr 
		static class_version = CoClass::class_version;

		auto const 
		inline static class_guid = Guid{ __uuidof(CoClass) };
		
		auto constexpr 
		static program_id = library_type::library_name + L'.' + CoClass::class_name;
	};
}
