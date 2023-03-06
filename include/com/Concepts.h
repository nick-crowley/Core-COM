#pragma once
#ifndef ComFramework_h_included
#	error Including this header directly may cause a circular dependency; include <comFramework.h> directly
#endif
#include "meta/TypeTraits.h"
#include "../src/PlatformSdk.h"
#include "../src/libBoost.h"

namespace core::com {
	enum class ThreadingModel;
	class Guid;
	class Version;
}

namespace core::meta
{
	template <typename T>
	concept ConvertibleToHResult = std::is_arithmetic_v<T> && !std::is_same_v<T,::HRESULT>;
		
	template <typename T>
	concept ConvertibleFromHResult = std::is_arithmetic_v<T> && !is_any_of_v<T,bool,::HRESULT>;
	
	template <typename T>
	concept ForwardSequence = requires {
		typename mpl::begin<T>::type;
		typename mpl::end<T>::type;
		typename mpl::empty<T>::type;
		typename mpl::front<T>::type;
		typename mpl::size<T>::type;
	};

	template <typename T>
	concept HasGuid = requires { __uuidof(T); };

	//! @brief	Any interface decorated with ::GUID and derived from ::IUnknown
	template <typename T>
	concept Interface = HasGuid<T>
	                  && std::derived_from<T,::IUnknown>
	                  && std::is_class_v<T> && std::is_abstract_v<T>;

	//! @brief	Any concrete class which realizes ::IUnknown
	template <typename T>
	concept CoImpl = std::is_base_of_v<::IUnknown,T>
	              && std::is_class_v<T> && !std::is_abstract_v<T>;
	
	//! @brief	Any interface derived from ::IDispatch
	template <typename T>
	concept DispInterface = std::derived_from<T,::IDispatch>;
}
