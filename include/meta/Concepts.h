#pragma once
#ifndef ComFramework_h_included
#	error Including this header directly may cause a circular dependency; include <comFramework.h> directly
#endif
#include "meta/TypeTraits.h"

namespace meta
{
	template <typename T>
	concept Character = is_any_of_v<T,char,wchar_t,char8_t,char16_t,char32_t>;

	template <typename T>
	concept Interface = std::is_base_of_v<::IUnknown,T>;

	template <typename T>
	concept CoClass = Interface<T>
	               && std::is_default_constructible_v<T>; // TODO: Requires `interface_tuple` member type -or- derives from com::implements<T...>
	
	template <typename T>
	concept DispInterface = std::is_base_of_v<::IDispatch,T>;
	
	template <typename T>
	concept ConvertibleToHResult = std::is_arithmetic_v<T> && !std::is_same_v<T,::HRESULT>;
		
	template <typename T>
	concept ConvertibleFromHResult = std::is_arithmetic_v<T> && !is_any_of_v<T,bool,::HRESULT>;
	
	template <typename T>
	concept Enumeration = std::is_enum_v<T>;

	template <typename From, typename To>
	concept ExplicitlyConvertible = requires(From&& f)
	{
		static_cast<To>(f);
	};

	template <typename T>
	concept IntegralOrEnum = std::is_integral_v<T> || Enumeration<T>;
	
	template <typename T>
	concept RealNumber = std::is_arithmetic_v<T> && !Character<T>;
}
