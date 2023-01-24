#pragma once
#ifndef ComFramework_h_included
#	error Including this header directly may cause a circular dependency; include <comFramework.h> directly
#endif
#include "meta/TypeTraits.h"
#include "../src/PlatformSdk.h"

namespace core::meta
{
	template <typename T>
	concept ConvertibleToHResult = std::is_arithmetic_v<T> && !std::is_same_v<T,::HRESULT>;
		
	template <typename T>
	concept ConvertibleFromHResult = std::is_arithmetic_v<T> && !is_any_of_v<T,bool,::HRESULT>;
	

	template <typename T>
	concept Interface = std::is_base_of_v<::IUnknown,T>;

	template <typename T>
	concept CoClass = Interface<T>
	               && std::is_default_constructible_v<T>; // TODO: Requires `interface_tuple` member type -or- derives from com::implements<T...>
	
	template <typename T>
	concept DispInterface = std::is_base_of_v<::IDispatch,T>;
}
