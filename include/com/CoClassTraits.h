/* o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o */ /*!
* @copyright	Copyright (c) 2023, Nick Crowley. All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted
* provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions
*    and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of
*    conditions and the following disclaimer in the documentation and/or other materials provided
*    with the distribution.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
* FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
* WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
* The views and conclusions contained in the software and documentation are those of the author 
* and should not be interpreted as representing official policies, either expressed or implied, of
* the projects which contain it.
*/
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Preprocessor Directives o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#pragma once
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Header Files o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#include "library/core.COM.h"
#include "core/LiteralString.h"
#include "com/Guid.h"
#include "com/ClassFactory.h"
#include "com/Version.h"
#include "com/ThreadingModel.h"
#include "com/LibraryTraits.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
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
	metafunc coclass_factory : std::type_identity<ClassFactory<CoClass>> {};

	template <typename CoClass> requires requires { typename CoClass::factory_type; }
	metafunc coclass_factory<CoClass,void> : std::type_identity<typename CoClass::factory_type> {};
	

	//! @brief	@c CoClass::class_guid if present, otherwise @c __uuidof(CoClass)
	template <typename CoClass, typename = void>
	com::Guid constexpr
	coclass_guid_v { __uuidof(CoClass) };

	template <typename CoClass> requires requires { CoClass::class_guid; }
	com::Guid constexpr
	coclass_guid_v<CoClass,void> = CoClass::class_guid;
	

	//! @brief	Always @c CoClass::library_type
	template <typename CoClass> requires requires { typename CoClass::library_type; }
	metafunc coclass_library : std::type_identity<typename CoClass::library_type> {};
	
	//! @brief	@c CoClass::class_name if present, otherwise unqualified class name
	template <typename CoClass, typename = void> 
	auto constexpr
	coclass_name_v = LiteralString<char,unqualified_class_name_v<CoClass>.length()>{ unqualified_class_name_v<CoClass>.data() };

	template <typename CoClass> requires requires { CoClass::class_name; }
	LiteralString constexpr
	coclass_name_v<CoClass,void> = CoClass::class_name;

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
	template <typename T>
	concept CoreCoClassDeclaration = CoClass<T> 
		&& CoreLibraryDeclaration<typename T::library_type>
		&& requires {
			{ com::coclass_name_v<T> } -> std::convertible_to<std::string_view>;
			com::coclass_name_v<T> + '.';
			com::coclass_name_v<T> + com::coclass_name_v<T>;
		} 
		&& com::coclass_name_v<T> != std::string_view{}
		&& com::coclass_guid_v<T> != com::Guid{}
		&& meta::ForwardSequence<typename T::interfaces>;
}

namespace core::com
{
	//! @brief	Compile-time co-class metadata
	template <meta::detail::CoreCoClassDeclaration CoClass>
	metafunc coclass_traits
	{
		using factory_type = typename coclass_factory<CoClass>::type;

		using library_type = typename coclass_library<CoClass>::type;

		ThreadingModel constexpr 
		static apartment = coclass_apartment_v<CoClass>;
		
		Guid constexpr 
		static class_guid = coclass_guid_v<CoClass>;
		
		LiteralString constexpr 
		static class_name = coclass_name_v<CoClass>;

		Version constexpr 
		static class_version = coclass_version_v<CoClass>;

		LiteralString constexpr 
		static program_id = library_name_v<library_type> + '.' + coclass_name_v<CoClass>;
	};
}

namespace core::meta
{
	//! @brief	Well-formed Core co-class (ie. one which possesses valid traits)
	template <typename T>
	concept CoreCoClass = CoClass<T> && HasGuid<T> && requires {
		{ com::coclass_traits<T>::apartment } -> std::convertible_to<com::ThreadingModel>;
		{ com::coclass_traits<T>::class_name } -> std::convertible_to<std::string_view>;
		{ com::coclass_traits<T>::class_guid } -> std::convertible_to<com::Guid>;
		{ com::coclass_traits<T>::class_version } -> std::convertible_to<com::Version>;
		{ com::coclass_traits<T>::program_id } -> std::convertible_to<std::string_view>;
		typename com::coclass_traits<T>::factory_type;
		typename com::coclass_traits<T>::library_type;
	} && CoreLibrary<typename com::coclass_traits<T>::library_type>
	  && std::derived_from<typename com::coclass_traits<T>::factory_type,::IClassFactory>
	  && com::coclass_traits<T>::class_name != std::string_view{}
	  && com::coclass_traits<T>::class_guid != com::Guid{}
	  && com::coclass_traits<T>::class_version >= com::Version{1,0}
	  && com::coclass_traits<T>::program_id != std::string_view{};
}

namespace core::com
{
	//! @brief	Class factory type for @p CoClass
	template <meta::CoreCoClass CoClass> 
	using coclass_factory_t = typename coclass_factory<CoClass>::type;
	
	//! @brief	Library type for @p CoClass
	template <meta::CoreCoClass CoClass> 
	using coclass_library_t = typename coclass_library<CoClass>::type;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-~o Test Code o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
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
	static_assert(!meta::CoreCoClass<ClassWithMemberGuid>);
	
	// Verify class apartment is detected or defaulted
	struct ClassWithoutApartment{};
	struct ClassWithApartment{
		ThreadingModel constexpr
		static apartment = ThreadingModel::Isolated;
	};
	static_assert(coclass_apartment_v<ClassWithoutApartment> == coclass_apartment_v<ClassWithApartment>);
	static_assert(!meta::CoreCoClass<ClassWithApartment>);
	
	// Verify class version is detected or defaulted
	struct ClassWithoutVersion{};
	struct ClassWithVersion{
		Version constexpr
		static class_version = Version{1,0};
	};
	static_assert(coclass_version_v<ClassWithoutVersion> == coclass_version_v<ClassWithVersion>);
	static_assert(!meta::CoreCoClass<ClassWithVersion>);
	
	// Verify class name is detected
	struct ClassWithoutName{};
	struct ClassWithName{
		LiteralString constexpr
		static class_name = "ClassWithName";
	};
	static_assert(coclass_name_v<ClassWithName> == "ClassWithName");
	static_assert(!meta::CoreCoClass<ClassWithName>);

	// Verify class concept
	MIDL_INTERFACE("8F33278E-EF80-42E2-8C90-1749DBCD7836") 
	ValidCoClass : com::implements<IUnknown> {
		using library_type = ValidCoLibrary;

		LiteralString constexpr
		static class_name = "ValidCoClass";
	};
	static_assert(meta::CoreCoClass<ValidCoClass>);

}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o