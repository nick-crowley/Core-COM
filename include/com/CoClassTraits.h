#pragma once
#include "library/core.COM.h"
#include "core/LiteralString.h"
#include "com/Guid.h"
#include "com/ClassFactory.h"
#include "com/Version.h"
#include "com/ThreadingModel.h"
#include "com/LibraryTraits.h"

namespace core::com
{
	template <typename CoClass, typename = void>
	com::ThreadingModel constexpr
	coclass_apartment_v = com::ThreadingModel::Isolated;

	template <typename CoClass> requires requires { CoClass::apartment; }
	com::ThreadingModel constexpr
	coclass_apartment_v<CoClass,void> = CoClass::apartment;
	

	template <typename CoClass, typename = void>
	struct coclass_factory : std::type_identity<ClassFactory<CoClass>> {};

	template <typename CoClass> requires requires { CoClass::factory_type; }
	struct coclass_factory<CoClass,void> : std::type_identity<typename CoClass::factory_type> {};
	

	template <typename CoClass, typename = void>
	com::Guid constexpr
	coclass_guid_v = __uuidof(CoClass);

	template <typename CoClass> requires requires { CoClass::class_guid; }
	com::Guid constexpr
	coclass_guid_v<CoClass,void> = CoClass::class_guid;


	template <typename CoClass> requires requires { CoClass::class_name; }
	LiteralString constexpr
	coclass_name_v = CoClass::class_name;


	template <typename CoClass, typename = void>
	com::Version constexpr
	coclass_version_v = com::Version{1,0};

	template <typename CoClass> requires requires { CoClass::class_version; }
	com::Version constexpr
	coclass_version_v<CoClass,void> = CoClass::class_version;
	

	template <meta::CoImpl CoClass>
		requires meta::CoreCoLibrary<typename CoClass::library_type>
	 && requires {CoClass::class_name;}
	struct coclass_traits
	{
		using factory_type = typename coclass_factory<CoClass>::type;

		using library_type = typename CoClass::library_type;

		ThreadingModel constexpr 
		static apartment = coclass_apartment_v<CoClass>;
		
		Guid constexpr 
		static class_guid = coclass_guid_v<CoClass>;
		
		LiteralString constexpr 
		static class_name = coclass_name_v<CoClass>;

		Version constexpr 
		static class_version = coclass_version_v<CoClass>;

		LiteralString constexpr 
		static program_id = library_name_v<library_type> + L'.' + coclass_name_v<CoClass>;
	};
}

namespace core::meta
{
	//! @brief	Core COM class (ie. one which possesses valid traits)
	template <typename T>
	concept CoreCoClass = CoImpl<T> && HasGuid<T> && requires {
		{ com::coclass_traits<T>::apartment } -> std::convertible_to<com::ThreadingModel>;
		{ com::coclass_traits<T>::class_name } -> std::convertible_to<std::wstring_view>;
		{ com::coclass_traits<T>::class_guid } -> std::convertible_to<com::Guid>;
		{ com::coclass_traits<T>::class_version } -> std::convertible_to<com::Version>;
		{ com::coclass_traits<T>::program_id } -> std::convertible_to<std::wstring_view>;
		//{ com::coclass_traits<T>::factory_type } -> std::derived_from<::IClassFactory>;
		//{ com::coclass_traits<T>::library_type } -> CoreCoLibrary;
		com::coclass_traits<T>::factory_type;
		com::coclass_traits<T>::library_type;
	};
}

namespace core::com
{
	template <meta::CoreCoClass CoClass> 
	using coclass_factory_t = typename coclass_factory<CoClass>::type;
}
