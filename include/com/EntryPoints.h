#pragma once
#include "comFramework.h"
#include "com/CoClassTraits.h"
#include "win/RegistryKey.h"
#include "com/Boolean.h"
#include "com/Function.h"

namespace com
{
	extern std::atomic_long g_numInstances;

	template <meta::CoClass CoClass>
	HRESULT 
	getClassObject(Guid clsId, ::IID const& iid, void** ppv)
	{
		static typename coclass_traits<CoClass>::factory_type factory;

		if (clsId != __uuidof(CoClass))
			return CLASS_E_CLASSNOTAVAILABLE;

		return factory.CreateInstance(nullptr,iid,ppv);
	}

	HRESULT 
	canUnloadNow()
	{
		return Boolean{g_numInstances == 0};
	}

	template <meta::CoClass CoClass>
	HRESULT
	registerServer(HANDLE hModule) 
	try {
		using traits = coclass_traits<CoClass>;
		using namespace win;

		// Get module path
		wchar_t modulePath[MAX_PATH] {};
		::GetModuleFileNameW(reinterpret_cast<HMODULE>(hModule), modulePath, MAX_PATH);

		// Insert class-id registration
		RegistryKey CLSID{win::ClassesRoot, L"CLSID", KeyRight::All};
		RegistryKey ourClassId = CLSID.subkey(create_new, traits::class_guid.str());
		ourClassId[use_default] = traits::class_name;
		RegistryKey ourServerPath = ourClassId.subkey(create_new, L"InProcServer32");
		ourServerPath[use_default] = std::wstring_view{modulePath};
		ourServerPath[L"ThreadingModel"] = std::wstring_view{L"Apartment"};

		// Insert program-id registration
		RegistryKey ourProgId{create_new, win::ClassesRoot, traits::program_id, KeyRight::All};
		ourProgId[use_default] = traits::class_name;
		
		// Link the two ids
		RegistryKey{create_new, ourClassId, L"ProgId", KeyRight::All}[use_default] = traits::program_id;
		RegistryKey{create_new, ourProgId, L"CLSID", KeyRight::All}[use_default] = traits::class_guid.str();

		return S_OK;
	}
	catch (std::exception const& /*e*/)
	{
		return E_FAIL;
	}
	
	template <meta::CoClass CoClass>
	HRESULT
	unregisterServer() 
	try {
		using traits = coclass_traits<CoClass>;
		using namespace win;

		// Remove class-id registration
		RegistryKey CLSID{win::ClassesRoot, L"CLSID", KeyRight::All};
		{
			auto ourClassId = CLSID.subkey(traits::class_guid.str());
			ourClassId.removeKey(L"ProgId");
			ourClassId.removeKey(L"InProcServer32");
		}
		CLSID.removeKey(traits::class_guid.str());
		
		// Remove program-id registration
		RegistryKey allClasses{win::ClassesRoot, KeyRight::All};
		{
			RegistryKey ourProgId{allClasses, traits::program_id, KeyRight::All};
			ourProgId.removeKey(L"CLSID");
		}
		allClasses.removeKey(traits::program_id);
		return S_OK;
	}
	catch (std::exception const& /*e*/)
	{
		return E_FAIL;
	}
}
