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
#include "com/AuthService.h"
#include "com/Authorization.h"
#include "com/Function.h"
#include "com/BasicString.h"
#include "com/TokenAccess.h"
#include "security/Descriptor.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com
{	
	struct ClientBlanket
	{
		wstring       Username;
		AuthLevel     Strength;
		AuthService   Authenticator;
		Authorization Mechanism;
	};

	struct ProxyBlanket { 
		wstring       Username;
		AuthLevel     Strength;
		AuthService   Authenticator;
		Authorization Mechanism;
		TokenAccess   Rights; 
	};

	struct OleAuthService {
		std::optional<wstring> Username;
		AuthLevel              Strength;
		AuthService            Authenticator;
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
	private:
		auto constexpr
		inline static fromOleAuthService = [](OleAuthService& s) -> SOLE_AUTHENTICATION_SERVICE {
			return { 
				.dwAuthnSvc = std::to_underlying(s.Authenticator), 
				.dwAuthzSvc = std::to_underlying(s.Strength), 
				.pPrincipalName = s.Username.value_or(wstring{}).data(),
				.hr = S_OK
			}; 
		};

		auto constexpr
		inline static authRegistrationFailed = [](SOLE_AUTHENTICATION_SERVICE& s) {
			return FAILED(s.hr);
		};
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
		* @brief	Set process-wide security for a client
		*
		* @param[in] rights  Impersonation level
		* @param[in] auth    Authentication level
		* 
		* @throws	std::system_error	Operation failed
		*/
		void
		virtual initializeSecurity(TokenAccess rights, AuthLevel auth) const 
		{
			win::HResult hr = ::CoInitializeSecurity(
				win::Unsecured,
				win::Unused<DWORD>,
				win::Unused<PSOLE_AUTHENTICATION_SERVICE>,
				win::Reserved<void*>,
				std::to_underlying(auth),
				std::to_underlying(rights),
				win::Unused<DWORD>,
				win::Unused<DWORD>,
				win::Reserved<void*>
			);
			hr.throwIfError("::CoInitializeSecurity() failed");
		}
		
		/* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
		* @brief	Set process-wide security for a server
		*
		* @param[in] access    Access permissions used to receive calls
		* @param[in] strength  Authentication level
		* @param[in] services  [optional] Permitted authentication services
		* 
		* @throws	std::system_error	Operation failed
		*/
		void
		virtual initializeSecurity(security::Descriptor access, AuthLevel strength, std::vector<OleAuthService> services) const
		{
			std::vector  _services{std::from_range, services | views::transform(ComApi::fromOleAuthService)};
			win::HResult hr = ::CoInitializeSecurity(
				access,
				_services.size() ? (DWORD)_services.size() : (DWORD)AuthService::Default,
				_services.data(),
				win::Reserved<void*>,
				std::to_underlying(strength),
				win::Unused<DWORD>,
				win::Unused<void*>,
				win::Unused<DWORD>,
				win::Reserved<void*>
			);
			if (auto failure = ranges::find_if(_services, ComApi::authRegistrationFailed); failure != _services.cend())
				win::HResult{failure->hr}.throwAlways("::CoInitializeSecurity() failed to register {} for '{}'", 
					static_cast<AuthService>(failure->dwAuthnSvc),
					core::cnarrow(failure->pPrincipalName ? failure->pPrincipalName : L"current-user")
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
		ClientBlanket
		virtual queryClientBlanket() const 
		{
			DWORD             authService{};
			DWORD             authorization{};
			wstring           username{};
			DWORD             authLevel{};
			RPC_AUTHZ_HANDLE  privileges{};
			DWORD             capabilities{};
			win::HResult hr = ::CoQueryClientBlanket(
				&authService,
				&authorization,
				std::out_ptr<wchar_t*>(username, adopt),
				&authLevel,
				win::Unused<DWORD*>,
				&privileges,
				&capabilities
			);
			hr.throwIfError("::CoQueryClientBlanket() failed");
			return ClientBlanket{
				.Username = username,
				.Strength = static_cast<AuthLevel>(authLevel),
				.Authenticator = static_cast<AuthService>(authService),
				.Mechanism = static_cast<Authorization>(authorization)
			};
		}
		
		/* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
		* @brief	Retrieve proxy security blanket
		*
		* @returns	Proxy properties
		*
		* @throws	std::invalid_argument  Missing argument
		* @throws	std::system_error      Operation failed
		*/
		ProxyBlanket
		virtual queryProxyBlanket(::IUnknown* proxy) const 
		{
			ThrowIfNull(proxy);

			// NB: std::tuple<DWORD,DWORD,wchar_t*,DWORD,DWORD,RPC_AUTH_IDENTITY_HANDLE,DWORD>
			auto [
				authenticationService, 
				authorizationService, 
				serverPrincipal, 
				authenticationLevel, 
				impersonationLevel,
				clientIdentity,
				capabilities
			] = com::function<7>(::CoQueryProxyBlanket)(proxy);

			return ProxyBlanket{
				.Username = com::wstring{serverPrincipal, adopt},
				.Strength = static_cast<AuthLevel>(authenticationLevel),
				.Authenticator = static_cast<AuthService>(authenticationService),
				.Mechanism = static_cast<Authorization>(authorizationService),
				.Rights = static_cast<TokenAccess>(impersonationLevel)
			};
		}
		
		/* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
		* @brief	Manipulate proxy security blanket
		*
		* @param[in] proxy    
		* @param[in] rights   Impersonation level
		* @param[in] auth.....Authentication service
		* @param[in] strength Authentication level
		* @param[in] authz    Authorization service
		* @param[in] user     [optional] Principal used for authentication
		* 
		* @throws	std::invalid_argument  Missing argument
		* @throws	std::system_error      Operation failed
		*/
		void
		virtual setProxyBlanket(::IUnknown* proxy, TokenAccess rights, AuthService auth, AuthLevel strength, Authorization authz, std::optional<wstring> user) const 
		{
			ThrowIfNull(proxy);
			
			win::HResult hr = ::CoSetProxyBlanket(proxy,
				std::to_underlying(auth),
				std::to_underlying(authz),
				user ? user->data() : COLE_DEFAULT_PRINCIPAL,
				std::to_underlying(strength), 
				std::to_underlying(rights),
				win::Unused<::RPC_AUTH_IDENTITY_HANDLE*>, 
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