#pragma once
#include "library/core.COM.h"
#include "core/LiteralString.h"
#include "com/Guid.h"
#include "com/Version.h"

namespace core::com
{
	template <typename Library, typename = void>
	com::Guid constexpr
	library_guid_v = __uuidof(Library);

	template <typename Library> requires requires { Library::library_guid; }
	com::Guid constexpr
	library_guid_v<Library,void> = Library::library_guid;


	template <typename Library> requires requires { Library::library_name; }
	LiteralString constexpr
	library_name_v = Library::library_name;
	

	template <typename Library, typename = void>
	com::Version constexpr
	library_version_v = com::Version{1,0};

	template <typename Library> requires requires { Library::library_version; }
	com::Version constexpr
	library_version_v<Library,void> = Library::library_version;
	

	template <typename Library> requires requires {Library::library_name;}
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
	//! @brief	Core COM library (ie. one which possesses valid traits)
	template <typename T>
	concept CoreCoLibrary = requires {
		{ com::library_traits<T>::library_name } -> std::convertible_to<std::wstring_view>;
		{ com::library_traits<T>::library_guid } -> std::convertible_to<com::Guid>;
		{ com::library_traits<T>::library_version } -> std::convertible_to<com::Version>;
	};
}
