/* o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o */ /*!
* @copyright	Copyright (c) 2018, Nick Crowley. All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted
* provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions
*    and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of
*    conditions and the following disclaimer in the documentation and/or other materials provided
*    with the distribution.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
* FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
* WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
* The views and conclusions contained in the software and documentation are those of the author 
* and should not be interpreted as representing official policies, either expressed or implied, of
* the projects which contain it.
*/
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Preprocessor Directives o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#pragma once
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Header Files o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#include "library/core.Com.h"
#include "com/AuthLevel.h"
#include "com/BasicString.h"
#include "com/TokenAccess.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com
{	
	struct ClientBlanket
	{
		wstring   Username; 
		AuthLevel Authentication; 
	};

	struct ProxyBlanket { 
		wstring     Username; 
		AuthLevel   Authentication; 
		TokenAccess Rights; 
	};

	/* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
	* @brief	Implements the high-level COM API
	* 
	* @details	This class wraps the production API and takes responsibility for performing boilerplate
	*			tasks such as argument verification, buffer allocation, and converting error-codes into 
	*           exceptions, for example.
	*/
	class ComExport ComApi
	{
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
	
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
	
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
	
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
	public:
		satisfies(ComApi, 
			IsDefaultConstructible,
			IsCopyable, 
			IsMovable noexcept
		);
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
	public:
		/* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
		* @brief	Set default security blanket
		*
		* @param[in] imp   Impersonation level
		* @param[in] auth  Authentication level
		* 
		* @throws	std::system_error	Operation failed
		*/
		[[nodiscard]]
		void
		virtual initializeSecurity(TokenAccess imp, AuthLevel auth) const 
		{
			::DWORD constexpr Capabilities{};
			::DWORD constexpr NumAuthServices{};
			::PVOID constexpr AuthProperties{};
			::PSOLE_AUTHENTICATION_SERVICE constexpr AuthServices{};
			win::HResult hr = ::CoInitializeSecurity(
				win::Unsecured,
				NumAuthServices,
				AuthServices,
				win::Unused<void*>,
				std::to_underlying(auth),
				std::to_underlying(imp),
				AuthProperties,
				Capabilities,
				win::Unused<void*>
			);
			hr.throwIfError("::CoInitializeSecurity() failed");
		}
		
		/* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
		* @brief	Impersonate the client
		*
		* @throws	std::system_error	Operation failed
		*/
		void
		virtual impersonate() const 
		{
			win::HResult hr = ::CoImpersonateClient();
			hr.throwIfError("::CoImpersonateClient() failed");
		}

		/* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
		* @brief	Stop impersonating the client
		*
		* @throws	std::system_error	Operation failed
		*/
		void
		virtual revertToSelf() const 
		{
			win::HResult hr = ::CoRevertToSelf();
			hr.throwIfError("::CoRevertToSelf() failed");
		}

		/* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
		* @brief	Retrieve client security blanket
		*
		* @returns	Client properties
		*
		* @throws	std::system_error	Operation failed
		*/
		[[nodiscard]]
		ClientBlanket
		virtual queryClientBlanket() const 
		{
			using win::Unused;

			wstring      username{};
			DWORD        auth{};
			win::HResult hr = ::CoQueryClientBlanket(
				Unused<DWORD*>,
				Unused<DWORD*>,
				std::out_ptr<wchar_t*>(username, adopt),
				&auth,
				Unused<DWORD*>,
				Unused<RPC_AUTHZ_HANDLE*>,
				Unused<DWORD*>
			);
			hr.throwIfError("::CoQueryClientBlanket() failed");
			return { username, static_cast<AuthLevel>(auth) };
		}
		
		/* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
		* @brief	Retrieve proxy security blanket
		*
		* @returns	Proxy properties
		*
		* @throws	std::system_error	Operation failed
		*/
		[[nodiscard]]
		ProxyBlanket
		virtual queryProxyBlanket(::IUnknown* proxy) const 
		{
			ThrowIfNull(proxy);

			using win::Unused;
			wstring username{};
			DWORD   auth{};
			DWORD   imp{};
			win::HResult hr = ::CoQueryProxyBlanket(
				proxy,
				Unused<DWORD*>,
				Unused<DWORD*>,
				std::out_ptr<wchar_t*>(username, adopt),
				&auth,
				&imp,
				Unused<RPC_AUTH_IDENTITY_HANDLE*>,
				Unused<DWORD*>
			);
			hr.throwIfError("::CoQueryProxyBlanket() failed");
			return { username, static_cast<AuthLevel>(auth), static_cast<TokenAccess>(imp) };
		}
		
		/* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
		* @brief	Manipulate proxy security blanket
		*
		* @param[in] proxy 
		* @param[in] imp
		* @param[in] auth
		* 
		* @throws	std::system_error	Operation failed
		*/
		[[nodiscard]]
		void
		virtual setProxyBlanket(::IUnknown* proxy, TokenAccess imp, AuthLevel auth) const 
		{
			ThrowIfNull(proxy);

			using win::Unused;
			win::HResult hr = ::CoSetProxyBlanket(proxy,
				RPC_C_AUTHN_DEFAULT, 
				Unused<::DWORD>, 
				Unused<wchar_t*>,
				std::to_underlying(auth), 
				std::to_underlying(imp),
				Unused<::RPC_AUTH_IDENTITY_HANDLE*>, 
				EOAC_NONE
			);
			hr.throwIfError("::CoSetProxyBlanket() failed");
		}

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
	};

	//! @brief	High-level COM API pointer
	using SharedComApi = std::shared_ptr<ComApi>;

}	// namespace core::com
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com 
{
	/* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
	* @brief	Retrieve implementation of high-level COM API
	*
	* @return	Non-null instance (possibly mocked)
	*
	* @related	core::com::IComApi
	*/
	SharedComApi
	ComExport com_api();
}   // namespace core::com
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o