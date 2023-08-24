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
#include "com/Boolean.h"
#include "com/Function.h"
#include "com/SharedPtr.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com
{
	std::atomic_long 
	extern com::numInstances;

	/* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
	* @brief	Retrieve a factory instance for any co-class
	* @param[in]	clsId	Implementation ID
	* @param[in]	iid		Interface ID
	* @param[out]	ppv		Upon return, points to the instance
	*/
	template <meta::CoreCoClass CoClass>
	::HRESULT 
	getClassObject(Guid clsId, ::IID const& iid, void** ppv)
	{
		if (clsId != __uuidof(CoClass))
			return CLASS_E_CLASSNOTAVAILABLE;

		shared_ptr<::IClassFactory> factory = make_shared<::IClassFactory,coclass_factory_t<CoClass>>();
		return factory->QueryInterface(iid,ppv);
	}

	/* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
	* @brief	Query whether lifetime of all objects constructed by this module have expired
	*/
	::HRESULT 
	canUnloadNow()
	{
		return Boolean{com::numInstances == 0};
	}

	/* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
	* @brief	Populate all registry entries for @p CoClass co-classes
	*/
	template <meta::CoreCoClass CoClass, typename Traits = coclass_traits<CoClass>>
	::HRESULT
	registerServer(::HANDLE hModule) 
	try {
		using namespace core::win;

		// Get module path
		wchar_t modulePath[MAX_PATH] {};
		::GetModuleFileNameW(reinterpret_cast<::HMODULE>(hModule), modulePath, MAX_PATH);

		// Insert class-id registration
		RegistryKey CLSID{win::ClassesRoot, L"CLSID", KeyRight::All};
		RegistryKey ourClassId = CLSID.subkey(create_new, Traits::class_guid.wstr());
		ourClassId[use_default] = Traits::class_name.wstr();
		RegistryKey ourServerPath = ourClassId.subkey(create_new, L"InProcServer32");
		ourServerPath[use_default] = std::wstring_view{modulePath};
		switch (Traits::apartment) {
		case ThreadingModel::Isolated: ourServerPath[L"ThreadingModel"] = std::wstring_view{L"Apartment"}; break;
		case ThreadingModel::Shared:   ourServerPath[L"ThreadingModel"] = std::wstring_view{L"Free"};      break;
		case ThreadingModel::Any:      ourServerPath[L"ThreadingModel"] = std::wstring_view{L"Both"};      break;
		}
		
		// Insert program-id registration
		RegistryKey ourProgId{create_new, win::ClassesRoot, Traits::program_id.wstr(), KeyRight::All};
		ourProgId[use_default] = Traits::class_name.wstr();
		
		// Link the two ids
		RegistryKey{create_new, ourClassId, L"ProgId", KeyRight::All}[use_default] = Traits::program_id.wstr();
		RegistryKey{create_new, ourProgId, L"CLSID", KeyRight::All}[use_default] = Traits::class_guid.wstr();

		return S_OK;
	}
	catch (std::exception const& /*e*/)
	{
		return E_FAIL;
	}

	/* ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` ` */ /*!
	* @brief	Remove all registry entries for @p CoClass co-classes
	*/
	template <meta::CoreCoClass CoClass, typename Traits = coclass_traits<CoClass>>
	::HRESULT
	unregisterServer() 
	try {
		using namespace core::win;

		// Remove class-id registration
		RegistryKey CLSID{win::ClassesRoot, L"CLSID", KeyRight::All};
		{
			auto ourClassId = CLSID.subkey(Traits::class_guid.wstr());
			ourClassId.removeKey(L"ProgId");
			ourClassId.removeKey(L"InProcServer32");
		}
		CLSID.removeKey(Traits::class_guid.wstr());
		
		// Remove program-id registration
		RegistryKey allClasses{win::ClassesRoot, KeyRight::All};
		{
			RegistryKey ourProgId{allClasses, Traits::program_id.wstr(), KeyRight::All};
			ourProgId.removeKey(L"CLSID");
		}
		allClasses.removeKey(Traits::program_id.wstr());
		return S_OK;
	}
	catch (std::exception const& /*e*/)
	{
		return E_FAIL;
	}
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o