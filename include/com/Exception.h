#pragma once
#include "comFramework.h"
#include "com/Guid.h"
#include "com/CoClassTraits.h"
#include "nstd/OutPtr.h"

namespace com
{
	template <typename CoClass, typename Interface>
	class Exception
	{
		using traits = coclass_traits<CoClass>;

		HRESULT m_result;

	public:
		Exception(HRESULT result, std::wstring_view const msg) noexcept
		  : m_result(result)
		{
			shared_ptr<ICreateErrorInfo> info;	
			if (auto hr = ::CreateErrorInfo(nstd::out_ptr<ICreateErrorInfo*>(info)); FAILED(hr))	// BUG: double-AddRef
				this->m_result = hr;
			else
			{
				info->SetDescription(const_cast<wchar_t*>(msg.data()));					// TODO: Check this works
				info->SetSource(const_cast<wchar_t*>(traits::program_id.data()));
				info->SetGUID(__uuidof(Interface));
				
				if (hr = ::SetErrorInfo(0, shared_ptr<IErrorInfo>{info}); FAILED(hr))
					this->m_result = hr;
			}
		}

		implicit operator 
		HRESULT() const noexcept
		{
			return this->m_result;
		}
	};
	
	[[noreturn]] void 
	throw_exception(::HRESULT hr, std::source_location loc = std::source_location::current());
}
