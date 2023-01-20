#pragma once
#include "library/core.COM.h"
#include "core/CallAdapter.h"
#include "com/HResult.h"

namespace core::com::detail
{
	template <meta::Interface Interface, typename... Parameters>
	class ComMethodFunctor 
	{
		using signature_t = ::HRESULT (__stdcall Interface::*)(Parameters...);

		signature_t  m_method;

	public:
		constexpr 
		ComMethodFunctor(signature_t signature) noexcept
			: m_method{signature}
		{}

		template <typename CoClass>
		::HRESULT constexpr 
		operator()(CoClass& object, Parameters... args) const
		{
			static_assert(std::is_base_of_v<Interface,CoClass>, "Type 'CoClass' must implement 'Interface'");

			ThrowingHResult hr = (object.*this->m_method)(args...);
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
	function(::HRESULT (__stdcall *fx)(Parameters...)) noexcept
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

	template <unsigned NumResults = 0, typename Interface, typename... Parameters>
	auto constexpr
	method(::HRESULT (__stdcall Interface::*method)(Parameters...)) noexcept
	{
		auto callable = detail::ComMethodFunctor{method};
		return core::detail::makeCallAdapter<NumResults, 
			                                 sizeof...(Parameters), 
			                                 decltype(callable), 
			                                 Parameters...>(std::move(callable));
	}
}  // namespace core::com
