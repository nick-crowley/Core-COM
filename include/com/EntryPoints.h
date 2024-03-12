/* o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o */ /*!
* @copyright	Copyright (c) 2023, Nick Crowley. All rights reserved.
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
#include "library/core.COM.h"
#include "com/CoClassTraits.h"
#include "win/RegistryKey.h"
#include "com/Function.h"
#include "com/GlobalRefCount.h"
#include "com/SharedPtr.h"
#include "com/ServerLocation.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com
{
	namespace detail {
		std::wstring_view constexpr
		keyNameFor(ServerLocation loc) noexcept 
		{
			switch (loc) {
			case ServerLocation::InProc:    return L"InProcServer32";
			case ServerLocation::Local:     return L"LocalServer";
			case ServerLocation::Service:   return L"Service";
			default:                        std::unreachable();
			}
		}
	}
	/* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
	* @brief	Retrieve a factory instance for any co-class
	* @param[in]	clsId	Implementation ID
	* @param[in]	iid		Interface ID
	* @param[out]	ppv		Upon return, points to the instance
	*/
	template <meta::CoreCoClass CoClass>
	win::HResult
	getClassObject(Guid clsId, ::IID const& iid, void** ppv)
	{
		if (clsId != coclass_guid_v<CoClass>)
			return CLASS_E_CLASSNOTAVAILABLE;

		shared_ptr<::IClassFactory> factory = make_shared<::IClassFactory,coclass_factory_t<CoClass>>();
		return factory->QueryInterface(iid,ppv);
	}

	/* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
	* @brief	Query whether lifetime of all objects constructed by this module have expired
	*/
	win::HResult
	canUnloadNow() noexcept
	{
		return win::HResult{ Boolean{GlobalRefCount::value() == 0} };
	}

	/* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
	* @brief	Populate all registry entries for @p CoClass co-classes
	*/
	template <meta::CoreCoClass CoClass, ServerLocation Location, typename Traits = coclass_traits<CoClass>>
	win::HResult
	registerServer(std::wstring_view modulePath) 
	try {
		ThrowIfEmpty(modulePath);
		using namespace core::win;

		auto constexpr classGuid = Traits::class_guid.wstr();
		auto constexpr className = Traits::class_name.wstr();
		auto constexpr programId = Traits::program_id.wstr();

		// Insert class-id registration
		RegistryKey CLSID{win::ClassesRoot, L"CLSID", KeyRight::All};
		RegistryKey ourClassId = CLSID.subkey(create_new, classGuid);
		ourClassId[use_default] = className;
		RegistryKey ourServerPath = ourClassId.subkey(create_new, detail::keyNameFor(Location));
		ourServerPath[use_default] = modulePath;
		switch (Traits::apartment) {
		case ThreadingModel::Isolated: ourServerPath[L"ThreadingModel"] = L"Apartment"sv; break;
		case ThreadingModel::Shared:   ourServerPath[L"ThreadingModel"] = L"Free"sv;      break;
		case ThreadingModel::Any:      ourServerPath[L"ThreadingModel"] = L"Both"sv;      break;
		}
		
		// Insert program-id registration
		RegistryKey ourProgId{create_new, win::ClassesRoot, programId, KeyRight::All};
		ourProgId[use_default] = className;
		
		// Link the two ids
		RegistryKey{create_new, ourClassId, L"ProgId", KeyRight::All}[use_default] = programId;
		RegistryKey{create_new, ourProgId, L"CLSID", KeyRight::All}[use_default] = classGuid;

		return S_OK;
	}
	catch (std::exception const& /*e*/)
	{
		return E_FAIL;
	}

	/* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
	* @brief	Remove all registry entries for @p CoClass co-classes
	*/
	template <meta::CoreCoClass CoClass, ServerLocation Location, typename Traits = coclass_traits<CoClass>>
	win::HResult
	unregisterServer()
	try {
		using namespace core::win;
		
		auto constexpr classGuid = Traits::class_guid.wstr();
		auto constexpr programId = Traits::program_id.wstr();

		// Remove class-id registration
		RegistryKey CLSID{win::ClassesRoot, L"CLSID", KeyRight::All};
		scoped {
			auto ourClassId = CLSID / classGuid;
			ourClassId.removeKey(L"ProgId");
			ourClassId.removeKey(detail::keyNameFor(Location));
		}
		CLSID.removeKey(classGuid);
		
		// Remove program-id registration
		RegistryKey allClasses{win::ClassesRoot, KeyRight::All};
		scoped {
			RegistryKey ourProgId = allClasses / programId;
			ourProgId.removeKey(L"CLSID");
		}
		allClasses.removeKey(programId);
		return S_OK;
	}
	catch (std::exception const& /*e*/)
	{
		return E_FAIL;
	}
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o