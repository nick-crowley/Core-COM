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
	//! @brief	@c CoClass::apartment if present, otherwise @c core::com::ThreadingModel::Isolated
	template <typename CoClass, typename = void>
	com::ThreadingModel constexpr
	coclass_apartment_v = com::ThreadingModel::Isolated;

	template <typename CoClass> requires requires { CoClass::apartment; }
	com::ThreadingModel constexpr
	coclass_apartment_v<CoClass,void> = CoClass::apartment;
	

	//! @brief	@c CoClass::factory_type if present, otherwise @c core::com::ClassFactory<CoClass>
	template <typename CoClass, typename = void>
	struct coclass_factory : std::type_identity<ClassFactory<CoClass>> {};

	template <typename CoClass> requires requires { CoClass::factory_type; }
	struct coclass_factory<CoClass,void> : std::type_identity<typename CoClass::factory_type> {};
	

	//! @brief	@c CoClass::class_guid if present, otherwise @c __uuidof(CoClass)
	template <typename CoClass, typename = void>
	com::Guid constexpr
	coclass_guid_v { __uuidof(CoClass) };

	template <typename CoClass> requires requires { CoClass::class_guid; }
	com::Guid constexpr
	coclass_guid_v<CoClass,void> = CoClass::class_guid;


	//! @brief	Always @c CoClass::class_name
	template <typename CoClass> requires requires { CoClass::class_name; }
	LiteralString constexpr
	coclass_name_v = CoClass::class_name;


	//! @brief	@c CoClass::class_version if present, otherwise @c core::com::Version{1,0}
	template <typename CoClass, typename = void>
	com::Version constexpr
	coclass_version_v = com::Version{1,0};

	template <typename CoClass> requires requires { CoClass::class_version; }
	com::Version constexpr
	coclass_version_v<CoClass,void> = CoClass::class_version;
}

namespace core::meta::detail
{
	//! @brief	Well-formed co-class declaration
	template <typename CoClass>
	concept CoClassDeclaration = CoImpl<CoClass> 
		&& CoLibraryDeclaration<typename CoClass::library_type>
		&& requires {
			{ CoClass::class_name } -> std::convertible_to<std::wstring_view>;
		} 
		&& CoClass::class_name != std::wstring_view{}
		&& com::coclass_guid_v<CoClass> != com::Guid{}
		&& meta::ForwardSequence<typename CoClass::interfaces>;
}

namespace core::com
{
	//! @brief	Compile-time co-class metadata
	template <meta::detail::CoClassDeclaration CoClass>
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
	//! @brief	Well-formed Core co-class (ie. one which possesses valid traits)
	template <typename T>
	concept CoClass = CoImpl<T> && HasGuid<T> && requires {
		{ com::coclass_traits<T>::apartment } -> std::convertible_to<com::ThreadingModel>;
		{ com::coclass_traits<T>::class_name } -> std::convertible_to<std::wstring_view>;
		{ com::coclass_traits<T>::class_guid } -> std::convertible_to<com::Guid>;
		{ com::coclass_traits<T>::class_version } -> std::convertible_to<com::Version>;
		{ com::coclass_traits<T>::program_id } -> std::convertible_to<std::wstring_view>;
		com::coclass_traits<T>::factory_type;
		com::coclass_traits<T>::library_type;
	} && CoLibrary<typename com::coclass_traits<T>::library_type>
	  && std::derived_from<typename com::coclass_traits<T>::factory_type,::IClassFactory>
	  && com::coclass_traits<T>::class_name != std::wstring_view{}
	  && com::coclass_traits<T>::class_guid != com::Guid{}
	  && com::coclass_traits<T>::class_version >= com::Version{1,0}
	  && com::coclass_traits<T>::program_id != std::wstring_view{};
}

namespace core::com
{
	//! @brief	Class factory type for @p CoClass
	template <meta::CoClass CoClass> 
	using coclass_factory_t = typename coclass_factory<CoClass>::type;
}

namespace core::com::detail::testing
{
	// Verify class GUID is detected from either source
	MIDL_INTERFACE("1D61B23A-5A26-48DA-A280-CE743C1B53F1")
	ClassWithoutMemberGuid{};
	struct ClassWithMemberGuid{
		Guid constexpr
		static class_guid = Guid::fromString("1D61B23A-5A26-48DA-A280-CE743C1B53F1");
	};
	static_assert(coclass_guid_v<ClassWithoutMemberGuid> == coclass_guid_v<ClassWithMemberGuid>);
	static_assert(!meta::CoClass<ClassWithMemberGuid>);
	
	// Verify class apartment is detected or defaulted
	struct ClassWithoutApartment{};
	struct ClassWithApartment{
		ThreadingModel constexpr
		static apartment = ThreadingModel::Isolated;
	};
	static_assert(coclass_apartment_v<ClassWithoutApartment> == coclass_apartment_v<ClassWithApartment>);
	static_assert(!meta::CoClass<ClassWithApartment>);
	
	// Verify class version is detected or defaulted
	struct ClassWithoutVersion{};
	struct ClassWithVersion{
		Version constexpr
		static class_version = Version{1,0};
	};
	static_assert(coclass_version_v<ClassWithoutVersion> == coclass_version_v<ClassWithVersion>);
	static_assert(!meta::CoClass<ClassWithVersion>);
	
	// Verify class name is detected
	struct ClassWithoutName{};
	struct ClassWithName{
		LiteralString constexpr
		static class_name = L"ClassWithName";
	};
	static_assert(coclass_name_v<ClassWithName> == L"ClassWithName");
	static_assert(!meta::CoClass<ClassWithName>);

	// Verify class concept
	MIDL_INTERFACE("8F33278E-EF80-42E2-8C90-1749DBCD7836") 
	ValidCoClass : com::implements<IUnknown> {
		using library_type = ValidCoLibrary;

		LiteralString constexpr
		static class_name = L"ValidCoClass";
	};
	static_assert(meta::CoClass<ValidCoClass>);

}