#pragma once
#include "comFramework.h"
#include "core/CallAdapter.h"
#include "nt/Exception.h"
#include "nt/NtStatus.h"

namespace nt
{
	template <unsigned NumResults = 0, typename... Parameters>
	auto constexpr 
	function(::NTSTATUS (__stdcall *fx)(Parameters...)) noexcept
	{
		auto const callable = [fx](Parameters... args) -> NTSTATUS 
		{
			if (NtStatus r = (*fx)(std::forward<Parameters>(args)...); !r)
				throw_exception(r);
			else
				return r;
		};

		return ::detail::makeCallAdapter<
			NumResults, sizeof...(Parameters), decltype(callable), Parameters...
		>(
			std::move(callable)
		);
	}
}  // namespace nt
