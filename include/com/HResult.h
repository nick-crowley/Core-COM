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
			
			_CaseStringify(CLASS_E_NOAGGREGATION);
			_CaseStringify(CLASS_E_CLASSNOTAVAILABLE);
			_CaseStringify(CLASS_E_NOTLICENSED);

			_CaseStringify(CAT_E_CATIDNOEXIST);
			_CaseStringify(CAT_E_NODESCRIPTION);

			_CaseStringify(CS_E_ADMIN_LIMIT_EXCEEDED);
			_CaseStringify(CS_E_CLASS_NOTFOUND);
			_CaseStringify(CS_E_INTERNAL_ERROR);
			_CaseStringify(CS_E_INVALID_PATH);
			_CaseStringify(CS_E_INVALID_VERSION);
			_CaseStringify(CS_E_NETWORK_ERROR);
			_CaseStringify(CS_E_NOT_DELETABLE);
			_CaseStringify(CS_E_NO_CLASSSTORE);
			_CaseStringify(CS_E_OBJECT_ALREADY_EXISTS);
			_CaseStringify(CS_E_OBJECT_NOTFOUND);
			_CaseStringify(CS_E_PACKAGE_NOTFOUND);
			_CaseStringify(CS_E_SCHEMA_MISMATCH);

			_CaseStringify(CACHE_E_NOCACHE_UPDATED);
			_CaseStringify(INPLACE_E_NOTUNDOABLE);
			_CaseStringify(INPLACE_E_NOTOOLSPACE);

			_CaseStringify(OLE_E_ADVF);
			_CaseStringify(OLE_E_ADVISENOTSUPPORTED);
			_CaseStringify(OLE_E_BLANK);
			_CaseStringify(OLE_E_CANTCONVERT);
			_CaseStringify(OLE_E_CANT_BINDTOSOURCE);
			_CaseStringify(OLE_E_CANT_GETMONIKER);
			_CaseStringify(OLE_E_CLASSDIFF);
			_CaseStringify(OLE_E_ENUM_NOMORE);
			_CaseStringify(OLE_E_INVALIDHWND);
			_CaseStringify(OLE_E_INVALIDRECT);
			_CaseStringify(OLE_E_NOCACHE);
			_CaseStringify(OLE_E_NOCONNECTION);
			_CaseStringify(OLE_E_NOSTORAGE);
			_CaseStringify(OLE_E_NOTRUNNING);
			_CaseStringify(OLE_E_NOT_INPLACEACTIVE);
			_CaseStringify(OLE_E_OLEVERB);
			_CaseStringify(OLE_E_PROMPTSAVECANCELLED);
			_CaseStringify(OLE_E_STATIC);
			_CaseStringify(OLE_E_WRONGCOMPOBJ);
			_CaseStringify(OLE_S_MAC_CLIPFORMAT);
			_CaseStringify(OLE_S_STATIC);
			_CaseStringify(OLE_S_USEREG);
			_CaseStringify(OLEOBJ_E_INVALIDVERB);
			_CaseStringify(OLEOBJ_E_NOVERBS);
			_CaseStringify(OLEOBJ_S_CANNOT_DOVERB_NOW);
			_CaseStringify(OLEOBJ_S_INVALIDHWND);
			_CaseStringify(OLEOBJ_S_INVALIDVERB);

			_CaseStringify(CO_E_ACCESSCHECKFAILED);
			_CaseStringify(CO_E_ACESINWRONGORDER);
			_CaseStringify(CO_E_ACNOTINITIALIZED);
			_CaseStringify(CO_E_ACTIVATIONFAILED);
			_CaseStringify(CO_E_ACTIVATIONFAILED_CATALOGERROR);
			_CaseStringify(CO_E_ACTIVATIONFAILED_EVENTLOGGED);
			_CaseStringify(CO_E_ACTIVATIONFAILED_TIMEOUT);
			_CaseStringify(CO_E_ALREADYINITIALIZED);
			_CaseStringify(CO_E_APPDIDNTREG);
			_CaseStringify(CO_E_APPNOTFOUND);
			_CaseStringify(CO_E_APPSINGLEUSE);
			_CaseStringify(CO_E_ASYNC_WORK_REJECTED);
			_CaseStringify(CO_E_ATTEMPT_TO_CREATE_OUTSIDE_CLIENT_CONTEXT);
			_CaseStringify(CO_E_BAD_PATH);
			_CaseStringify(CO_E_BAD_SERVER_NAME);
			_CaseStringify(CO_E_CALL_OUT_OF_TX_SCOPE_NOT_ALLOWED);
			_CaseStringify(CO_E_CANCEL_DISABLED);
			_CaseStringify(CO_E_CANTDETERMINECLASS);
			_CaseStringify(CO_E_CANT_REMOTE);
			_CaseStringify(CO_E_CLASSSTRING);
			_CaseStringify(CO_E_CLASS_CREATE_FAILED);
			_CaseStringify(CO_E_CLASS_DISABLED);
			_CaseStringify(CO_E_CLRNOTAVAILABLE);
			_CaseStringify(CO_E_CLSREG_INCONSISTENT);
			_CaseStringify(CO_E_CONVERSIONFAILED);
			_CaseStringify(CO_E_CREATEPROCESS_FAILURE);
			_CaseStringify(CO_E_DBERROR);
			_CaseStringify(CO_E_DECODEFAILED);
			_CaseStringify(CO_E_DLLNOTFOUND);
			_CaseStringify(CO_E_ELEVATION_DISABLED);
			_CaseStringify(CO_E_ERRORINAPP);
			_CaseStringify(CO_E_ERRORINDLL);
			_CaseStringify(CO_E_EXCEEDSYSACLLIMIT);
			_CaseStringify(CO_E_EXIT_TRANSACTION_SCOPE_NOT_CALLED);
			_CaseStringify(CO_E_FAILEDTOCLOSEHANDLE);
			_CaseStringify(CO_E_FAILEDTOCREATEFILE);
			_CaseStringify(CO_E_FAILEDTOGENUUID);
			_CaseStringify(CO_E_FAILEDTOGETSECCTX);
			_CaseStringify(CO_E_FAILEDTOGETTOKENINFO);
			_CaseStringify(CO_E_FAILEDTOGETWINDIR);
			_CaseStringify(CO_E_FAILEDTOIMPERSONATE);
			_CaseStringify(CO_E_FAILEDTOOPENPROCESSTOKEN);
			_CaseStringify(CO_E_FAILEDTOOPENTHREADTOKEN);
			_CaseStringify(CO_E_FAILEDTOQUERYCLIENTBLANKET);
			_CaseStringify(CO_E_FAILEDTOSETDACL);
			_CaseStringify(CO_E_IIDREG_INCONSISTENT);
			_CaseStringify(CO_E_IIDSTRING);
			_CaseStringify(CO_E_INCOMPATIBLESTREAMVERSION);
			_CaseStringify(CO_E_INITIALIZATIONFAILED);
			_CaseStringify(CO_E_INIT_CLASS_CACHE);
			_CaseStringify(CO_E_INIT_MEMORY_ALLOCATOR);
			_CaseStringify(CO_E_INIT_ONLY_SINGLE_THREADED);
			_CaseStringify(CO_E_INIT_RPC_CHANNEL);
			_CaseStringify(CO_E_INIT_SCM_EXEC_FAILURE);
			_CaseStringify(CO_E_INIT_SCM_FILE_MAPPING_EXISTS);
			_CaseStringify(CO_E_INIT_SCM_MAP_VIEW_OF_FILE);
			_CaseStringify(CO_E_INIT_SCM_MUTEX_EXISTS);
			_CaseStringify(CO_E_INIT_SHARED_ALLOCATOR);
			_CaseStringify(CO_E_INIT_TLS);
			_CaseStringify(CO_E_INIT_TLS_CHANNEL_CONTROL);
			_CaseStringify(CO_E_INIT_TLS_SET_CHANNEL_CONTROL);
			_CaseStringify(CO_E_INIT_UNACCEPTED_USER_ALLOCATOR);
			_CaseStringify(CO_E_INVALIDSID);
			_CaseStringify(CO_E_ISOLEVELMISMATCH);
			_CaseStringify(CO_E_LAUNCH_PERMSSION_DENIED);
			_CaseStringify(CO_E_LOOKUPACCNAMEFAILED);
			_CaseStringify(CO_E_LOOKUPACCSIDFAILED);
			_CaseStringify(CO_E_MALFORMED_SPN);
			_CaseStringify(CO_E_MISSING_DISPLAYNAME);
			_CaseStringify(CO_E_MSI_ERROR);
			_CaseStringify(CO_E_NETACCESSAPIFAILED);
			_CaseStringify(CO_E_NOCOOKIES);
			_CaseStringify(CO_E_NOIISINTRINSICS);
			_CaseStringify(CO_E_NOMATCHINGNAMEFOUND);
			_CaseStringify(CO_E_NOMATCHINGSIDFOUND);
			_CaseStringify(CO_E_NOSYNCHRONIZATION);
			_CaseStringify(CO_E_NOTCONSTRUCTED);
			_CaseStringify(CO_E_NOTINITIALIZED);
			_CaseStringify(CO_E_NOTPOOLED);
			_CaseStringify(CO_E_NOT_SUPPORTED);
			_CaseStringify(CO_E_NO_SECCTX_IN_ACTIVATE);
			_CaseStringify(CO_E_OBJISREG);
			_CaseStringify(CO_E_OBJNOTCONNECTED);
			_CaseStringify(CO_E_OBJNOTREG);
			_CaseStringify(CO_E_OBJSRV_RPC_FAILURE);
			_CaseStringify(CO_E_OLE1DDE_DISABLED);
			_CaseStringify(CO_E_PATHTOOLONG);
			_CaseStringify(CO_E_PREMATURE_STUB_RUNDOWN);
			_CaseStringify(CO_E_RELEASED);
			_CaseStringify(CO_E_RELOAD_DLL);
			_CaseStringify(CO_E_REMOTE_COMMUNICATION_FAILURE);
			_CaseStringify(CO_E_RUNAS_CREATEPROCESS_FAILURE);
			_CaseStringify(CO_E_RUNAS_LOGON_FAILURE);
			_CaseStringify(CO_E_RUNAS_SYNTAX);
			_CaseStringify(CO_E_RUNAS_VALUE_MUST_BE_AAA);
			_CaseStringify(CO_E_SCM_ERROR);
			_CaseStringify(CO_E_SCM_RPC_FAILURE);
			_CaseStringify(CO_E_SERVER_EXEC_FAILURE);
			_CaseStringify(CO_E_SERVER_INIT_TIMEOUT);
			_CaseStringify(CO_E_SERVER_NOT_PAUSED);
			_CaseStringify(CO_E_SERVER_PAUSED);
			_CaseStringify(CO_E_SERVER_START_TIMEOUT);
			_CaseStringify(CO_E_SERVER_STOPPING);
			_CaseStringify(CO_E_SETSERLHNDLFAILED);
			_CaseStringify(CO_E_START_SERVICE_FAILURE);
			_CaseStringify(CO_E_SXS_CONFIG);
			_CaseStringify(CO_E_THREADINGMODEL_CHANGED);
			_CaseStringify(CO_E_THREADPOOL_CONFIG);
			_CaseStringify(CO_E_TRACKER_CONFIG);
			_CaseStringify(CO_E_TRUSTEEDOESNTMATCHCLIENT);
			_CaseStringify(CO_E_UNREVOKED_REGISTRATION_ON_APARTMENT_SHUTDOWN);
			_CaseStringify(CO_E_WRONGOSFORAPP);
			_CaseStringify(CO_E_WRONGTRUSTEENAMESYNTAX);
			_CaseStringify(CO_E_WRONG_SERVER_IDENTITY);
			_CaseStringify(CO_S_MACHINENAMENOTFOUND);
			_CaseStringify(CO_S_NOTALLINTERFACES);

			_CaseStringify(REGDB_E_BADTHREADINGMODEL);
			_CaseStringify(REGDB_E_CLASSNOTREG);
			_CaseStringify(REGDB_E_IIDNOTREG);
			_CaseStringify(REGDB_E_INVALIDVALUE);
			_CaseStringify(REGDB_E_KEYMISSING);
			_CaseStringify(REGDB_E_PACKAGEPOLICYVIOLATION);
			_CaseStringify(REGDB_E_READREGDB);
			_CaseStringify(REGDB_E_WRITEREGDB);
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
