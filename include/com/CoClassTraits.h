#pragma once
#include "library/core.COM.h"
#include "core/LiteralString.h"
#include "com/Guid.h"
#include "com/ClassFactory.h"
#include "com/Version.h"
#include "com/ThreadingModel.h"

namespace core::com
{
	template <meta::CoClass CoClass>
	struct coclass_traits
	{
		using factory_type = ClassFactory<CoClass>;
		using library_type = typename CoClass::library_type;

		ThreadingModel constexpr 
		static apartment = CoClass::apartment;

		LiteralString constexpr 
		static class_name = CoClass::class_name;

		Version constexpr 
		static class_version = CoClass::class_version;

		Guid const 
		inline static class_guid { __uuidof(CoClass) };
		
		LiteralString constexpr 
		static program_id = library_type::library_name + L'.' + CoClass::class_name;
	};
}
