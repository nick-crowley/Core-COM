#pragma once
#include "library/core.COM.h"
#include "core/CallAdapter.h"
#include "com/HResult.h"

namespace core::com
{
	template <typename... Parameters>
	using function_pointer_t = ::HRESULT (__stdcall *)(Parameters...);

	template <meta::Interface Interface, typename... Parameters>
	using method_pointer_t = ::HRESULT (__stdcall Interface::*)(Parameters...);
}

namespace core::com::detail
{
	template <meta::Interface Interface, typename... Parameters>
	class ComMethodFunctor 
	{
		method_pointer_t<Interface,Parameters...>  m_method;

	public:
		constexpr 
		ComMethodFunctor(method_pointer_t<Interface,Parameters...> mx) noexcept
			: m_method{mx}
		{}

		template <typename CoClass>
		::HRESULT constexpr 
		operator()(CoClass& object, Parameters... args) const
		{
			static_assert(std::is_base_of_v<Interface,CoClass> || std::is_convertible_v<CoClass,Interface&>, 
				"Type 'CoClass' must implement 'Interface'");

			ThrowingHResult hr = (static_cast<Interface&>(object).*this->m_method)(args...);
			return hr;
		}
	};
}

namespace core::detail 
{
	template <meta::Interface Interface, typename... Parameters>
	bool constexpr is_method_v<com::detail::ComMethodFunctor<Interface,Parameters...>> = true;
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

		return core::detail::makeCallAdapter<NumResults, 
		                                     sizeof...(Parameters), 
		                                     decltype(callable), 
		                                     Parameters...>(std::move(callable));
	}

	template <unsigned NumResults = 0, meta::Interface Interface, typename... Parameters>
	auto constexpr
	method(method_pointer_t<Interface,Parameters...> method) noexcept
	{
		auto callable = detail::ComMethodFunctor{method};
		return core::detail::makeCallAdapter<NumResults, 
			                                 sizeof...(Parameters), 
			                                 decltype(callable), 
			                                 Parameters...>(std::move(callable));
	}

	//! @brief	Type of COM method wrapper (which can be quite complicated to determine)
	template <unsigned NumResults, meta::Interface Interface, typename... Parameters>
	using method_t = decltype(method<NumResults>(std::declval<method_pointer_t<Interface,Parameters...>>()));

}  // namespace core::com
