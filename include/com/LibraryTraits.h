#pragma once
#include "library/core.COM.h"
#include "core/LiteralString.h"
#include "com/Guid.h"
#include "com/Version.h"

namespace core::com
{
	//! @brief	@c Library::library_guid if present, otherwise @c __uuidof(Library)
	template <typename Library, typename = void>
	com::Guid constexpr
	library_guid_v;

	template <typename Library> requires requires { Library::library_guid; }
	com::Guid constexpr
	library_guid_v<Library,void> = Library::library_guid;
	
	template <meta::HasGuid Library> requires (!requires { Library::library_guid; })
	com::Guid constexpr
	library_guid_v<Library,void> {__uuidof(Library)};

	
	//! @brief	Always @c Library::class_name
	template <typename Library> requires requires { Library::library_name; }
	LiteralString constexpr
	library_name_v = Library::library_name;
	
	
	//! @brief	@c Library::class_version if present, otherwise @c core::com::Version{1,0}
	template <typename Library, typename = void>
	com::Version constexpr
	library_version_v = com::Version{1,0};

	template <typename Library> requires requires { Library::library_version; }
	com::Version constexpr
	library_version_v<Library,void> = Library::library_version;
}

namespace core::meta::detail
{	
	//! @brief	Well-formed COM library declaration
	template <typename Library>
	concept CoLibraryDeclaration = requires {
			{ Library::library_name } -> std::convertible_to<std::wstring_view>;
		} 
		&& Library::library_name != std::wstring_view{}
		&& com::library_guid_v<Library> != com::Guid{};
}

namespace core::com
{
	//! @brief	Compile-time COM library metadata
	template <meta::detail::CoLibraryDeclaration Library>
	struct library_traits
	{
		LiteralString constexpr 
		static library_name = library_name_v<Library>;

		Version constexpr 
		static library_version = library_version_v<Library>;

		Guid constexpr
		static library_guid = library_guid_v<Library>; 
	};
}

namespace core::meta
{
	//! @brief	Well-formed Core COM library (ie. one which possesses valid traits)
	template <typename T>
	concept CoLibrary = requires {
		{ com::library_traits<T>::library_name } -> std::convertible_to<std::wstring_view>;
		{ com::library_traits<T>::library_guid } -> std::convertible_to<com::Guid>;
		{ com::library_traits<T>::library_version } -> std::convertible_to<com::Version>;
	} && com::library_traits<T>::library_name != std::wstring_view{}
	  && com::library_traits<T>::library_guid != com::Guid{}
	  && com::library_traits<T>::library_version >= com::Version{1,0};
}

namespace core::com::detail::testing
{
	// Verify GUID is optional
	struct LibraryWithoutGuid{};
	static_assert(library_guid_v<LibraryWithoutGuid> == Guid{});

	// Verify library GUID is detected from either source
	MIDL_INTERFACE("714C8163-8D3F-4247-8BA8-9C152F131E91")
	LibraryWithoutMemberGuid{};
	struct LibraryWithMemberGuid{
		Guid constexpr
		static library_guid = Guid::fromString("714C8163-8D3F-4247-8BA8-9C152F131E91");
	};
	static_assert(library_guid_v<LibraryWithoutMemberGuid> == library_guid_v<LibraryWithMemberGuid>);
	static_assert(!meta::CoLibrary<LibraryWithMemberGuid>);

	// Verify library version is detected or defaulted
	struct LibraryWithoutVersion{};
	struct LibraryWithVersion{
		Version constexpr
		static library_version = Version{1,0};
	};
	static_assert(library_version_v<LibraryWithoutVersion> == library_version_v<LibraryWithVersion>);
	static_assert(!meta::CoLibrary<LibraryWithVersion>);
	
	// Verify library name is detected
	struct LibraryWithoutName{};
	struct LibraryWithName{
		LiteralString constexpr
		static library_name = L"LibraryWithName";
	};
	static_assert(!meta::CoLibrary<LibraryWithName>);

	// Verify library concept
	MIDL_INTERFACE("E5C012CA-0A03-46C9-A996-2601DBEF465B")
	ValidCoLibrary{
		LiteralString constexpr
		static library_name = L"ValidCoLibrary";
	};
	static_assert(meta::CoLibrary<ValidCoLibrary>);

}