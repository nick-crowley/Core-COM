#pragma once
#include "comFramework.h"
#include "win/Exception.h"

namespace win
{
	class LResult
	{
		::LRESULT m_value;
	public:
		implicit
		LResult(::LRESULT value, std::source_location loc = std::source_location::current()) 
		  : m_value{value}
		{
		}
		
		[[noreturn]]
		void 
		throw_always(std::string msg) {
			throw_exception(this->m_value, msg);
		}
	
		[[noreturn]]
		void 
		throw_if_failed(std::string msg) {
			if (!*this)
				throw_exception(this->m_value, msg);
		}

		explicit operator
		bool() const {
			return this->m_value == ERROR_SUCCESS;
		}

		implicit operator
		::LRESULT() const {
			return this->m_value;
		}
	};
		
	struct LastError : public LResult 
	{
		LastError(std::source_location loc = std::source_location::current()) : LResult{::GetLastError(), loc} 
		{}
	};

	class ThrowingLResult
	{
		::LRESULT m_value;
	public:
		implicit
		ThrowingLResult(::LRESULT value, std::source_location loc = std::source_location::current()) 
		  : m_value{value}
		{
			if (!*this)
				throw_exception(value);
		}
		
		explicit operator
		bool() const {
			return this->m_value == ERROR_SUCCESS;
		}

		implicit operator
		::LRESULT() const {
			return this->m_value;
		}
	};
}
