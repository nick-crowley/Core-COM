#pragma once
#include "library/core.COM.h"
#include "core/ToHexString.h"
#include "win/SystemError.h"

namespace core::com
{
	class HResult
	{
		::HRESULT m_value;
	public:
		satisfies(HResult,
			NotDefaultConstructible,
            IsCopyable noexcept,
			IsEqualityComparable noexcept,
			NotSortable
		);

		template <meta::ConvertibleToHResult T>
		implicit 
		HResult(T&&) = delete;

		implicit
		HResult(::HRESULT value) 
		  : m_value{value}
		{
		}
		
		std::string
		str() const {
#define _makeSwitch(value)  case value: return #value
			switch (this->m_value)
			{
			default: return to_hexString<8>(this->m_value);
			_makeSwitch(S_FALSE);
			_makeSwitch(S_OK);
			_makeSwitch(E_UNEXPECTED);
			_makeSwitch(E_NOTIMPL);
			_makeSwitch(E_OUTOFMEMORY);
			_makeSwitch(E_INVALIDARG);
			_makeSwitch(E_NOINTERFACE);
			_makeSwitch(E_POINTER);
			_makeSwitch(E_HANDLE);
			_makeSwitch(E_ABORT);
			_makeSwitch(E_FAIL);
			_makeSwitch(E_ACCESSDENIED);
			}
#undef _makeSwitch
		}

		explicit operator
		bool() const {
			return SUCCEEDED(this->m_value);
		}

		implicit operator
		::HRESULT() const {
			return this->m_value;
		}
		
		template <meta::ConvertibleFromHResult T>
		implicit operator
		T() const = delete;
	};

	std::string
	inline to_string(HResult const& hr)
	{
		return hr.str();
	}
	
	std::wstring
	to_wstring(HResult const&) = delete;
	
	class ThrowingHResult
	{
		::HRESULT m_value = S_OK;
	public:
		satisfies(ThrowingHResult,
			IsDefaultConstructible,
			NotCopyConstructible,
			IsCopyAssignable,
			IsEqualityComparable noexcept,
			NotSortable
		);

		template <meta::ConvertibleToHResult T>
		implicit
		ThrowingHResult(T&&, std::source_location = std::source_location::current()) = delete;

		implicit
		ThrowingHResult(::HRESULT value, std::source_location loc = std::source_location::current()) 
		  : m_value{value}
		{
			if (FAILED(value))
				throw win::system_error{value};
		}
		
		std::string
		str() const {
			return HResult{this->m_value}.str();
		}

		explicit operator
		bool() const {
			return SUCCEEDED(this->m_value);
		}

		implicit operator
		::HRESULT() const {
			return this->m_value;
		}
		
		template <meta::ConvertibleFromHResult T>
		implicit operator
		T() const = delete;
	};

	std::string
	inline to_string(ThrowingHResult const& hr)
	{
		return hr.str();
	}

	std::wstring
	to_wstring(ThrowingHResult const&) = delete;
}
