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
#include "com/Version.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com::detail
{
	//! @brief	@c Library::library_guid if present, otherwise @c __uuidof(Library)
	template <typename Library, typename = void>
	Guid constexpr
	library_guid_v;

	template <typename Library> 
		requires 
			requires { Library::library_guid; }
	Guid constexpr
	library_guid_v<Library,void> = Library::library_guid;
	
	template <meta::HasGuid Library> 
		requires 
			(!requires { Library::library_guid; })
	Guid constexpr
	library_guid_v<Library,void> {__uuidof(Library)};


	//! @brief	@c Library::library_name if present, otherwise unqualified class name
	template <typename Library, typename = void> 
	LiteralString constexpr
	library_name_v = LiteralString<char,unqualified_class_name_v<Library>.length()+1>{ unqualified_class_name_v<Library>.data() };
	
	template <typename Library> 
		requires 
			requires { Library::library_name; }
	LiteralString constexpr
	library_name_v<Library,void> = Library::library_name;
	

	//! @brief	@c Library::library_version if present, otherwise @c core::com::Version{1,0}
	template <typename Library, typename = void>
	Version constexpr
	library_version_v = Version{1,0};

	template <typename Library> 
		requires 
			requires { Library::library_version; }
	Version constexpr
	library_version_v<Library,void> = Library::library_version;
}

namespace core::meta::detail
{	
	//! @brief	Well-formed declaration containing mandatory minimum properties for a COM library
	template <typename Library>
	concept CoreLibraryDeclaration = 
	  requires {
		{ com::detail::library_name_v<Library> } -> std::convertible_to<std::string_view>;
		com::detail::library_name_v<Library> + '.';
		com::detail::library_name_v<Library> + com::detail::library_name_v<Library>;
	  }
	  && com::detail::library_name_v<Library> != std::string_view{}
	  && com::detail::library_guid_v<Library> != com::Guid{}
	  && com::detail::library_version_v<Library> >= com::Version{1,0};
}

namespace core::com
{
	//! @brief	Compile-time metadata for a COM library
	template <meta::detail::CoreLibraryDeclaration Library>
	metafunc library_traits
	{
		LiteralString constexpr 
		static library_name = detail::library_name_v<Library>;

		Version constexpr 
		static library_version = detail::library_version_v<Library>;

		Guid constexpr
		static library_guid = detail::library_guid_v<Library>; 
	};
}

namespace core::meta
{
	//! @brief	Well-formed COM library possessesing valid traits (either deduced or explicitly specialized)
	template <typename T>
	concept CoreLibrary = requires {
		{ com::library_traits<T>::library_name } -> std::convertible_to<std::string_view>;
		{ com::library_traits<T>::library_guid } -> std::convertible_to<com::Guid>;
		{ com::library_traits<T>::library_version } -> std::convertible_to<com::Version>;
	} && com::library_traits<T>::library_name != std::string_view{}
	  && com::library_traits<T>::library_guid != com::Guid{}
	  && com::library_traits<T>::library_version >= com::Version{1,0};
}

namespace core::com
{
	//! @brief	GUID for well-formed COM library @p Library
	template <meta::CoreLibrary Library>
	Guid constexpr
	library_guid_v = library_traits<Library>::library_guid;
	
	//! @brief	Name string for well-formed COM library @p Library
	template <meta::CoreLibrary Library>
	LiteralString constexpr
	library_name_v = library_traits<Library>::library_name;
	
	//! @brief	Version identifier for well-formed COM library @p Library
	template <meta::CoreLibrary Library>
	Version constexpr
	library_version_v = library_traits<Library>::library_version;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-~o Test Code o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com::testing
{
	//! @test  Verify @c com::detail::library_guid_v is empty string when GUID not present
	struct LibraryWithoutGuid{};
	static_assert(detail::library_guid_v<LibraryWithoutGuid> == Guid{});

	//! @test  Verify @c com::detail::library_guid_v is detected from metadata
	using namespace guid_literals;
	MIDL_INTERFACE("714C8163-8D3F-4247-8BA8-9C152F131E91")
	LibraryWithAttributeGuid{};
	static_assert(detail::library_guid_v<LibraryWithAttributeGuid> == "714C8163-8D3F-4247-8BA8-9C152F131E91"_guid);
	
	//! @test  Verify @c com::detail::library_guid_v is detected from static member variable
	struct LibraryWithMemberGuid{
		Guid constexpr
		static library_guid = Guid{"714C8163-8D3F-4247-8BA8-9C152F131E91"};
	};
	static_assert(detail::library_guid_v<LibraryWithMemberGuid> == "714C8163-8D3F-4247-8BA8-9C152F131E91"_guid);
	
	//! @test  Verify @c com::detail::library_guid_v is the same whether detected from either source
	static_assert(detail::library_guid_v<LibraryWithAttributeGuid> == detail::library_guid_v<LibraryWithMemberGuid>);


	//! @test  Verify @c com::detail::library_version_v is defaulted when not present
	struct LibraryWithoutVersion{};
	static_assert(detail::library_version_v<LibraryWithoutVersion> == Version{1,0});

	//! @test  Verify @c com::detail::library_version_v is detected from member variable
	struct LibraryWithVersion{
		Version constexpr
		static library_version = Version{1,0};
	};
	static_assert(detail::library_version_v<LibraryWithVersion> == Version{1,0});

	//! @test  Verify @c com::detail::library_version_v is the same whether detected from either source
	static_assert(detail::library_version_v<LibraryWithoutVersion> == detail::library_version_v<LibraryWithVersion>);

	
	//! @test  Verify @c com::detail::library_name_v is detected from member variable
	struct LibraryWithName{
		LiteralString constexpr
		static library_name = "CustomName";
	};
	static_assert(detail::library_name_v<LibraryWithName> == "CustomName");
	
	//! @test  Verify @c com::detail::library_name_v is defaulted via reflection
	struct LibraryWithoutName{};
	static_assert(detail::library_name_v<LibraryWithoutName> == "LibraryWithoutName");
	

	//! @test Verify library name, GUID, and version are mandatory to model @c meta::detail::CoreLibraryDeclaration
	struct LibraryWithEmptyGuid{
		Guid constexpr
		static library_guid;
	};
	struct LibraryWithEmptyName{
		LiteralString constexpr
		static library_name = "";
	};
	struct LibraryWithEmptyVersion{
		Version constexpr
		static library_version{0,0};
	};
	static_assert(meta::detail::CoreLibraryDeclaration<LibraryWithAttributeGuid>);
	static_assert(meta::detail::CoreLibraryDeclaration<LibraryWithMemberGuid>);
	static_assert(!meta::detail::CoreLibraryDeclaration<LibraryWithoutGuid>);
	static_assert(!meta::detail::CoreLibraryDeclaration<LibraryWithEmptyGuid>);
	static_assert(!meta::detail::CoreLibraryDeclaration<LibraryWithEmptyName>);
	static_assert(!meta::detail::CoreLibraryDeclaration<LibraryWithEmptyVersion>);


	//! @test Verify library name, GUID, and version are mandatory to model@c meta::CoreLibrary
	MIDL_INTERFACE("E5C012CA-0A03-46C9-A996-2601DBEF465B")
	ValidCoLibrary{};
	static_assert(meta::CoreLibrary<ValidCoLibrary>);
	static_assert(!meta::CoreLibrary<LibraryWithVersion>);
	static_assert(!meta::CoreLibrary<LibraryWithName>);
	static_assert(!meta::CoreLibrary<LibraryWithoutName>);
	static_assert(!meta::CoreLibrary<LibraryWithoutGuid>);
	static_assert(!meta::CoreLibrary<LibraryWithEmptyGuid>);
	static_assert(!meta::CoreLibrary<LibraryWithEmptyName>);
	static_assert(!meta::CoreLibrary<LibraryWithEmptyVersion>);


	//! @test Verify trait accessors produce same values as their implementations
	static_assert(library_guid_v<ValidCoLibrary> == detail::library_guid_v<ValidCoLibrary>);
	static_assert(library_name_v<ValidCoLibrary> == detail::library_name_v<ValidCoLibrary>);
	static_assert(library_version_v<ValidCoLibrary> == detail::library_version_v<ValidCoLibrary>);
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o