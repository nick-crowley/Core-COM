#pragma once
#include "library/core.COM.h"
#include "com/Guid.h"
#include "com/CoClassTraits.h"
#include "nstd/OutPtr.h"

namespace core::com
{
	template <meta::CoClass CoClass, meta::Interface Interface = nstd::tuple_front_t<typename CoClass::interface_tuple>>
	class SetLastError
	{
		using traits = coclass_traits<CoClass>;

		::HRESULT m_result;

	public:
		SetLastError(::HRESULT result, std::string_view const msg) noexcept
		  : SetLastError(result, std::wstring{msg.begin(), msg.end()})
		{}

		SetLastError(::HRESULT result, std::wstring_view const msg) noexcept
		  : m_result(result)
		{
			shared_ptr<::ICreateErrorInfo> info;	
			if (auto hr = ::CreateErrorInfo(std::out_ptr(info)); FAILED(hr))	// BUG: double-AddRef
				this->m_result = hr;
			else
			{
				info->SetDescription(const_cast<wchar_t*>(msg.data()));					// TODO: Check this works
				info->SetSource(const_cast<wchar_t*>(traits::program_id.data()));
				info->SetGUID(guid_v<Interface>);
				
				if (hr = ::SetErrorInfo(0, shared_ptr<::IErrorInfo>{info}); FAILED(hr))
					this->m_result = hr;
			}
		}

		implicit operator 
		::HRESULT() const noexcept
		{
			return this->m_result;
		}
	};
}
