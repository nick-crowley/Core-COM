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
#include "com/ApplicationTraits.h"
#include "com/LibraryTraits.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com::detail
{
	//! @brief	@c CoClass::apartment if present, otherwise @c core::com::ThreadingModel::Isolated
	template <typename CoClass, typename = void>
	ThreadingModel constexpr
	coclass_apartment_v = com::ThreadingModel::Isolated;

	template <typename CoClass> 
		requires 
			requires { CoClass::apartment; }
	ThreadingModel constexpr
	coclass_apartment_v<CoClass,void> = CoClass::apartment;
	

	//! @brief	@c CoClass::application_type if present, otherwise @c meta::undefined_t
	template <typename CoClass, typename = void>
	metafunc coclass_application : std::type_identity<meta::undefined_t> {};

	template <typename CoClass> 
		requires 
			requires { typename CoClass::application_type; }
	metafunc coclass_application<CoClass,void> : std::type_identity<typename CoClass::application_type> {};
	

	//! @brief	@c CoClass::factory_type if present, otherwise @c core::com::ClassFactory<CoClass>
	template <typename CoClass, typename = void>
	metafunc coclass_factory : std::type_identity<ClassFactory<CoClass>> {};

	template <typename CoClass> 
		requires 
			requires { typename CoClass::factory_type; }
	metafunc coclass_factory<CoClass,void> : std::type_identity<typename CoClass::factory_type> {};
	

	//! @brief	@c CoClass::class_guid if present, then @c __uuidof(CoClass) if present; otherwise empty guid
	template <typename CoClass, typename = void>
	Guid constexpr
	coclass_guid_v;

	template <typename CoClass> 
		requires 
			requires { CoClass::class_guid; }
	Guid constexpr
	coclass_guid_v<CoClass,void> = CoClass::class_guid;
	
	template <meta::HasGuid CoClass> 
		requires 
			(!requires { CoClass::class_guid; })
	Guid constexpr
	coclass_guid_v<CoClass,void> { __uuidof(CoClass) };


	//! @brief	Always @c CoClass::library_type
	template <typename CoClass> 
		requires 
			requires { typename CoClass::library_type; }
	metafunc coclass_library : std::type_identity<typename CoClass::library_type> {};
	

	//! @brief	@c CoClass::class_name if present, otherwise unqualified class name
	template <typename CoClass, typename = void> 
	auto constexpr
	coclass_name_v = LiteralString<char,unqualified_class_name_v<CoClass>.length()+1>{ unqualified_class_name_v<CoClass>.data() };

	template <typename CoClass> 
		requires 
			requires { CoClass::class_name; }
	LiteralString constexpr
	coclass_name_v<CoClass,void> = CoClass::class_name;


	//! @brief	@c CoClass::class_version if present, otherwise @c core::com::Version{1,0}
	template <typename CoClass, typename = void>
	Version constexpr
	coclass_version_v = Version{1,0};

	template <typename CoClass>
		requires 
			requires { CoClass::class_version; }
	Version constexpr
	coclass_version_v<CoClass,void> = CoClass::class_version;
}

namespace core::meta::detail
{
	template <typename T>
	concept OptionalApplicationDeclaration = 
	    (requires { typename T::application_type; } == false)
	 || CoreApplicationDeclaration<typename T::application_type>;

	//! @brief	Well-formed declaration containing mandatory minimum properties for a COM class
	template <typename T>
	concept CoreCoClassDeclaration = CoClass<T> 
		&& CoreLibraryDeclaration<typename T::library_type>
		&& OptionalApplicationDeclaration<T>
		&& requires {
			{ com::detail::coclass_name_v<T> } -> std::convertible_to<std::string_view>;
			com::detail::coclass_name_v<T> + '.';
			com::detail::coclass_name_v<T> + com::detail::coclass_name_v<T>;
		} 
		&& com::detail::coclass_name_v<T> != std::string_view{}
		&& com::detail::coclass_guid_v<T> != com::Guid{}
		&& com::detail::coclass_version_v<T> >= com::Version{1,0}
		&& meta::ForwardSequence<typename T::interfaces>;
}

namespace core::com
{
	//! @brief	Compile-time metadata for a COM class
	template <meta::detail::CoreCoClassDeclaration CoClass>
	metafunc coclass_traits
	{
		using application_type = typename detail::coclass_application<CoClass>::type;

		using factory_type = typename detail::coclass_factory<CoClass>::type;

		using library_type = typename detail::coclass_library<CoClass>::type;

		ThreadingModel constexpr 
		static apartment = detail::coclass_apartment_v<CoClass>;
		
		Guid constexpr 
		static class_guid = detail::coclass_guid_v<CoClass>;
		
		LiteralString constexpr 
		static class_name = detail::coclass_name_v<CoClass>;

		Version constexpr 
		static class_version = detail::coclass_version_v<CoClass>;

		LiteralString constexpr 
		static program_id = library_name_v<library_type> + '.' + detail::coclass_name_v<CoClass>;
	};
}

namespace core::meta
{
	//! @brief	Well-formed COM class possessesing valid traits (either deduced or explicitly specialized)
	template <typename T>
	concept CoreCoClass = CoClass<T>
	  && HasGuid<T> 
	  && requires {
		{ com::coclass_traits<T>::apartment } -> std::convertible_to<com::ThreadingModel>;
		{ com::coclass_traits<T>::class_name } -> std::convertible_to<std::string_view>;
		{ com::coclass_traits<T>::class_guid } -> std::convertible_to<com::Guid>;
		{ com::coclass_traits<T>::class_version } -> std::convertible_to<com::Version>;
		{ com::coclass_traits<T>::program_id } -> std::convertible_to<std::string_view>;
		typename com::coclass_traits<T>::application_type;
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
	//! @brief	Threading model for well-formed COM class @p CoClass
	template <meta::CoreCoClass CoClass>
	ThreadingModel constexpr 
	coclass_apartment_v = coclass_traits<CoClass>::apartment;
	
	//! @brief	[optional] Application class for well-formed COM class @p CoClass, if any, otherwise @c core::undefined_t
	template <meta::CoreCoClass CoClass>
	using coclass_application_t = typename coclass_traits<CoClass>::application_type;
	
	//! @brief	Factory class for well-formed COM class @p CoClass
	template <meta::CoreCoClass CoClass>
	using coclass_factory_t = typename coclass_traits<CoClass>::factory_type;
	
	//! @brief	GUID for well-formed COM class @p CoClass
	template <meta::CoreCoClass CoClass>
	Guid constexpr 
	coclass_guid_v = coclass_traits<CoClass>::class_guid;
	
	//! @brief	COM library class for well-formed COM class @p CoClass
	template <meta::CoreCoClass CoClass>
	using coclass_library_t = typename coclass_traits<CoClass>::library_type;
	
	//! @brief	Name string for well-formed COM class @p CoClass
	template <meta::CoreCoClass CoClass>
	LiteralString constexpr 
	coclass_name_v = coclass_traits<CoClass>::class_name;

	//! @brief	Version identifier for well-formed COM class @p CoClass
	template <meta::CoreCoClass CoClass>
	Version constexpr 
	coclass_version_v = coclass_traits<CoClass>::class_version;

	//! @brief	Program id string for well-formed COM class @p CoClass
	template <meta::CoreCoClass CoClass>
	LiteralString constexpr 
	program_id_v = coclass_traits<CoClass>::program_id;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-~o Test Code o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com::testing
{	
	//! @test  Verify @c com::detail::coclass_guid_v is empty when GUID not present
	struct ClassWithoutGuid{};
	static_assert(detail::coclass_guid_v<ClassWithoutGuid> == Guid{});

	//! @test  Verify @c com::detail::coclass_guid_v is detected from metadata
	using namespace guid_literals;
	MIDL_INTERFACE("1D61B23A-5A26-48DA-A280-CE743C1B53F1")
	ClassWithAttributeGuid{};
	static_assert(detail::coclass_guid_v<ClassWithAttributeGuid> == "1D61B23A-5A26-48DA-A280-CE743C1B53F1"_guid);
	
	//! @test  Verify @c com::detail::coclass_guid_v is detected from static member variable
	struct ClassWithMemberGuid{
		Guid constexpr
		static class_guid = Guid{"1D61B23A-5A26-48DA-A280-CE743C1B53F1"};
	};
	static_assert(detail::coclass_guid_v<ClassWithMemberGuid> == "1D61B23A-5A26-48DA-A280-CE743C1B53F1"_guid);
	
	//! @test  Verify @c com::detail::coclass_guid_v is the same whether detected from either source
	static_assert(detail::coclass_guid_v<ClassWithAttributeGuid> == detail::coclass_guid_v<ClassWithMemberGuid>);

	
	//! @test  Verify @c com::detail::coclass_apartment_v is defaulted when not defined
	struct ClassWithoutApartment{};
	static_assert(detail::coclass_apartment_v<ClassWithoutApartment> == ThreadingModel::Isolated);

	//! @test  Verify @c com::detail::coclass_apartment_v is detected from static member variable
	struct ClassWithApartment{
		ThreadingModel constexpr
		static apartment = ThreadingModel::Shared;
	};
	static_assert(detail::coclass_apartment_v<ClassWithApartment> == ThreadingModel::Shared);


	
	//! @test  Verify @c com::detail::coclass_version_v is defaulted when not present
	struct ClassWithoutVersion{};
	static_assert(detail::coclass_version_v<ClassWithoutVersion> == Version{1,0});

	//! @test  Verify @c com::detail::coclass_version_v is detected from member variable
	struct ClassWithVersion{
		Version constexpr
		static class_version = Version{1,0};
	};
	static_assert(detail::coclass_version_v<ClassWithVersion> == Version{1,0});

	//! @test  Verify @c com::detail::coclass_version_v is the same whether detected from either source
	static_assert(detail::coclass_version_v<ClassWithoutVersion> == detail::coclass_version_v<ClassWithVersion>);


	
	//! @test  Verify @c com::detail::coclass_name_v is detected from member variable
	struct ClassWithName{
		LiteralString constexpr
		static class_name = "CustomName";
	};
	static_assert(detail::coclass_name_v<ClassWithName> == "CustomName");
	
	//! @test  Verify @c com::detail::coclass_name_v is defaulted via reflection
	struct ClassWithoutName{};
	static_assert(detail::coclass_name_v<ClassWithoutName> == "ClassWithoutName");
	

	//! @test Verify @c com::detail::coclass_application detects @c application_type when present
	[uuid("C0CB0FB2-7507-4EEF-8848-E2CF40983741")]
	struct CoClassWithApplicationType : implements<IUnknown> {
		using application_type = ValidCoApplication;
		using library_type = ValidCoLibrary;
	};
	static_assert(std::is_same_v<typename detail::coclass_application<CoClassWithApplicationType>::type, ValidCoApplication>);

	//! @test Verify @c com::detail::coclass_application is undefined when omitted
	[uuid("C0CB0FB2-7507-4EEF-8848-E2CF40983741")]
	struct CoClassWithoutApplicationType : implements<IUnknown> {
		using library_type = ValidCoLibrary;
	};
	static_assert(std::is_same_v<typename detail::coclass_application<CoClassWithoutApplicationType>::type, meta::undefined_t>);

	
	//! @test Verify @c meta::detail::CoreCoClassDeclaration requires library, class-name, GUID, and base-class
	struct CoClassMissingGuid : implements<IUnknown> {
		using library_type = ValidCoLibrary;
	};
	[uuid("C0CB0FB2-7507-4EEF-8848-E2CF40983741")]
	struct CoClassMissingLibraryType : implements<IUnknown> {
	};
	[uuid("504E8D6A-D8C3-42F2-A341-98B79FCC16BE")]
	struct CoClassMissingIUnknownBase {
		using library_type = ValidCoLibrary;
	};
	static_assert(!meta::detail::CoreCoClassDeclaration<CoClassMissingGuid>);
	static_assert(!meta::detail::CoreCoClassDeclaration<CoClassMissingLibraryType>);
	static_assert(!meta::detail::CoreCoClassDeclaration<CoClassMissingIUnknownBase>);
	
	//! @test Verify @c meta::detail::CoreCoClassDeclaration validates required properties aren't empty
	struct ClassWithEmptyGuid : implements<IUnknown> {
		using library_type = ValidCoLibrary;

		Guid constexpr
		static class_guid;
	};
	[uuid("5A07BAE8-62C3-478A-9F48-6A23E514AD27")]
	struct ClassWithEmptyName : implements<IUnknown> {
		using library_type = ValidCoLibrary;

		LiteralString constexpr
		static class_name = "";
	};
	[uuid("4A3E768F-8744-43D8-9718-A06E21111DC5")]
	struct ClassWithEmptyVersion : implements<IUnknown> {
		using library_type = ValidCoLibrary;

		Version constexpr
		static class_version{0,0};
	};
	static_assert(!meta::detail::CoreCoClassDeclaration<ClassWithEmptyGuid>);
	static_assert(!meta::detail::CoreCoClassDeclaration<ClassWithEmptyName>);
	static_assert(!meta::detail::CoreCoClassDeclaration<ClassWithEmptyVersion>);
	

	//! @test Verify @c meta::detail::CoreCoClassDeclaration accepts @c application_type when present or when omitted
	static_assert(meta::detail::CoreCoClassDeclaration<CoClassWithApplicationType>);
	static_assert(meta::detail::CoreCoClassDeclaration<CoClassWithoutApplicationType>);


	//! @test Verify @c meta::CoreCoClass requires all traits to be valid
#pragma region Define coclasses with missing traits
	[uuid("E5C012CA-0A03-46C9-A996-2601DBEF465B")]
	struct ValidCoClass : implements<IUnknown> {
		using library_type = ValidCoLibrary;
	};
	[uuid("A696C014-6B8C-4994-BF76-724C9B91C536")]
	struct CoClassMissingApartment : ValidCoClass
	{};
	[uuid("C382E1F1-6B99-4F93-B459-829D8C209697")]
	struct CoClassMissingApplicationType : ValidCoClass
	{};
	[uuid("1F692D8E-330E-4EFB-AB1C-DB8632BFADB6")]
	struct CoClassMissingFactoryType : ValidCoClass
	{};
	[uuid("60BD3841-6C96-4F41-893E-26F67DB119E2")]
	struct CoClassMissingProgramId : ValidCoClass
	{};
	struct CoClassStockTraits {
		using library_type = long long;

		Guid constexpr 
		static class_guid {"D99400A8-9975-4DAC-9C8C-E2E7B3A0C049"};
		
		LiteralString constexpr 
		static class_name = "abc";

		Version constexpr 
		static class_version = Version{4,2};
	};
}
namespace core::com
{
	template <>
	struct coclass_traits<testing::CoClassMissingApartment> : testing::CoClassStockTraits
	{
		using factory_type = long long;

		LiteralString constexpr 
		static program_id = "abc";
	};
	
	template <>
	struct coclass_traits<testing::CoClassMissingApplicationType> : testing::CoClassStockTraits
	{
		using factory_type = long long;

		ThreadingModel constexpr 
		static apartment = ThreadingModel::Any;
		
		LiteralString constexpr 
		static program_id = "abc";
	};

	template <>
	struct coclass_traits<testing::CoClassMissingFactoryType> : testing::CoClassStockTraits
	{
		ThreadingModel constexpr 
		static apartment = ThreadingModel::Any;
		
		LiteralString constexpr 
		static program_id = "abc";
	};

	template <>
	struct coclass_traits<testing::CoClassMissingProgramId> : testing::CoClassStockTraits
	{
		using factory_type = long long;

		ThreadingModel constexpr 
		static apartment = ThreadingModel::Any;
	};
}
namespace core::com::testing
{
#pragma endregion
	static_assert(meta::CoreCoClass<ValidCoClass>);
	static_assert(!meta::CoreCoClass<CoClassMissingApartment>);
	static_assert(!meta::CoreCoClass<CoClassMissingApplicationType>);
	static_assert(!meta::CoreCoClass<CoClassMissingFactoryType>);
	static_assert(!meta::CoreCoClass<CoClassMissingProgramId>);
	static_assert(!meta::CoreCoClass<CoClassMissingGuid>);
	static_assert(!meta::CoreCoClass<CoClassMissingLibraryType>);
	static_assert(!meta::CoreCoClass<CoClassMissingIUnknownBase>);
	static_assert(!meta::CoreCoClass<ClassWithEmptyGuid>);
	static_assert(!meta::CoreCoClass<ClassWithEmptyName>);
	static_assert(!meta::CoreCoClass<ClassWithEmptyVersion>);
	
	
	//! @test  Verify program ID is correct
	static_assert(coclass_traits<ValidCoClass>::program_id == "ValidCoLibrary.ValidCoClass");
	static_assert(program_id_v<ValidCoClass> == "ValidCoLibrary.ValidCoClass");


	//! @test Verify trait accessors produce same values as their implementations
	static_assert(coclass_apartment_v<ValidCoClass> == detail::coclass_apartment_v<ValidCoClass>);
	static_assert(std::is_same_v<coclass_application_t<ValidCoClass>, coclass_traits<ValidCoClass>::application_type>);
	static_assert(std::is_same_v<coclass_factory_t<ValidCoClass>, coclass_traits<ValidCoClass>::factory_type>);
	static_assert(std::is_same_v<coclass_library_t<ValidCoClass>, coclass_traits<ValidCoClass>::library_type>);
	static_assert(coclass_guid_v<ValidCoClass> == detail::coclass_guid_v<ValidCoClass>);
	static_assert(coclass_name_v<ValidCoClass> == detail::coclass_name_v<ValidCoClass>);
	static_assert(coclass_version_v<ValidCoClass> == detail::coclass_version_v<ValidCoClass>);
	static_assert(program_id_v<ValidCoClass> == coclass_traits<ValidCoClass>::program_id);
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o