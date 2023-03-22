#pragma once
#include "library/core.COM.h"
#include "core/SignatureAdapter.h"
#include "com/HResult.h"

namespace core::com
{
	template <typename... Parameters>
	using function_pointer_t = ::HRESULT (__stdcall *)(Parameters...);

	template <meta::Interface Interface, typename... Parameters>
	using method_pointer_t = ::HRESULT (__stdcall Interface::*)(Parameters...);
}

namespace core::com 
{
	template <unsigned NumResults = 0, typename... Parameters>
	auto constexpr
	function(function_pointer_t<Parameters...> fx) noexcept
	{
		auto const callable = [fx](Parameters... args) -> ::HRESULT
		{
			ThrowingHResult hr = (*fx)(std::forward<Parameters>(args)...);
			return hr;
		};

		return core::detail::adaptSignature<NumResults>(callable);
	}

	template <unsigned NumResults = 0, meta::Interface Interface, typename... Parameters>
	auto constexpr
	method(method_pointer_t<Interface,Parameters...> mx) noexcept
	{
		auto const callable = [mx](Interface& object, Parameters... args) -> ::HRESULT
		{
			ThrowingHResult hr = (object.*mx)(args...);
			return hr;
		};
		return core::detail::adaptSignature<NumResults>(callable);
	}

	//! @brief	Type of COM method wrapper (which can be quite complicated to determine)
	template <unsigned NumResults, meta::Interface Interface, typename... Parameters>
	using method_t = decltype(method<NumResults>(std::declval<method_pointer_t<Interface,Parameters...>>()));

}  // namespace core::com
