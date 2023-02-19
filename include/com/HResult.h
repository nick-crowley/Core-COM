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
			switch (this->m_value)
			{
			default: return to_hexString<8>(this->m_value);
				
#define _CaseStringify(value)  \
			case value: return #value

			_CaseStringify(S_FALSE);
			_CaseStringify(S_OK);
			_CaseStringify(E_ABORT);
			_CaseStringify(E_ACCESSDENIED);
			_CaseStringify(E_APPLICATION_EXITING);
			_CaseStringify(E_APPLICATION_VIEW_EXITING);
			_CaseStringify(E_ASYNC_OPERATION_NOT_STARTED);
			_CaseStringify(E_BOUNDS);
			_CaseStringify(E_CHANGED_STATE);
			_CaseStringify(E_FAIL);
			_CaseStringify(E_HANDLE);
			_CaseStringify(E_ILLEGAL_DELEGATE_ASSIGNMENT);
			_CaseStringify(E_ILLEGAL_STATE_CHANGE);
			_CaseStringify(E_ILLEGAL_METHOD_CALL);
			_CaseStringify(E_INVALIDARG);
			_CaseStringify(E_NO_TASK_QUEUE);
			_CaseStringify(E_NOINTERFACE);
			_CaseStringify(E_NOTIMPL);
			_CaseStringify(E_NOT_SET);
			_CaseStringify(E_NOT_SUFFICIENT_BUFFER);
			_CaseStringify(E_NOT_VALID_STATE);
			_CaseStringify(E_OUTOFMEMORY);
			_CaseStringify(E_PENDING);
			_CaseStringify(E_POINTER);
			_CaseStringify(E_STRING_NOT_NULL_TERMINATED);
			_CaseStringify(E_UAC_DISABLED);
			_CaseStringify(E_UNEXPECTED);
			
			_CaseStringify(CO_E_INIT_TLS);
			_CaseStringify(CO_E_INIT_SHARED_ALLOCATOR);
			_CaseStringify(CO_E_INIT_MEMORY_ALLOCATOR);
			_CaseStringify(CO_E_INIT_CLASS_CACHE);
			_CaseStringify(CO_E_INIT_RPC_CHANNEL);
			_CaseStringify(CO_E_INIT_TLS_SET_CHANNEL_CONTROL);
			_CaseStringify(CO_E_INIT_TLS_CHANNEL_CONTROL);
			_CaseStringify(CO_E_INIT_UNACCEPTED_USER_ALLOCATOR);
			_CaseStringify(CO_E_INIT_SCM_MUTEX_EXISTS);
			_CaseStringify(CO_E_INIT_SCM_FILE_MAPPING_EXISTS);
			_CaseStringify(CO_E_INIT_SCM_MAP_VIEW_OF_FILE);
			_CaseStringify(CO_E_INIT_SCM_EXEC_FAILURE);
			_CaseStringify(CO_E_INIT_ONLY_SINGLE_THREADED);
			_CaseStringify(CO_E_CANT_REMOTE);
			_CaseStringify(CO_E_BAD_SERVER_NAME);
			_CaseStringify(CO_E_WRONG_SERVER_IDENTITY);
			_CaseStringify(CO_E_OLE1DDE_DISABLED);
			_CaseStringify(CO_E_CREATEPROCESS_FAILURE);
			_CaseStringify(CO_E_RUNAS_SYNTAX);
			_CaseStringify(CO_E_RUNAS_CREATEPROCESS_FAILURE);
			_CaseStringify(CO_E_RUNAS_LOGON_FAILURE);
			_CaseStringify(CO_E_LAUNCH_PERMSSION_DENIED);
			_CaseStringify(CO_E_START_SERVICE_FAILURE);
			_CaseStringify(CO_E_REMOTE_COMMUNICATION_FAILURE);
			_CaseStringify(CO_E_SERVER_START_TIMEOUT);
			_CaseStringify(CO_E_CLSREG_INCONSISTENT);
			_CaseStringify(CO_E_IIDREG_INCONSISTENT);
			_CaseStringify(CO_E_NOT_SUPPORTED);
			_CaseStringify(CO_E_RELOAD_DLL);
			_CaseStringify(CO_E_MSI_ERROR);
			_CaseStringify(CO_E_ATTEMPT_TO_CREATE_OUTSIDE_CLIENT_CONTEXT);
			_CaseStringify(CO_E_SERVER_PAUSED);
			_CaseStringify(CO_E_SERVER_NOT_PAUSED);
			_CaseStringify(CO_E_CLASS_DISABLED);
			_CaseStringify(CO_E_CLRNOTAVAILABLE);
			_CaseStringify(CO_E_ASYNC_WORK_REJECTED);
			_CaseStringify(CO_E_SERVER_INIT_TIMEOUT);
			_CaseStringify(CO_E_UNREVOKED_REGISTRATION_ON_APARTMENT_SHUTDOWN);
			_CaseStringify(CO_E_PREMATURE_STUB_RUNDOWN);
#undef _CaseStringify
			}
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
