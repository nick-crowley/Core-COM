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
		using namespace std::literals;

		// Get module path
		wchar_t modulePath[MAX_PATH] {};
		::GetModuleFileNameW(reinterpret_cast<HMODULE>(hModule), modulePath, MAX_PATH);

		// Insert class-id registration
		win::RegistryKey allClassIds{open_existing, win::ClassesRoot, L"CLSID"sv, win::KeyRight::All};
		win::RegistryKey ourClassId{create_new, allClassIds, traits::class_guid.str(), win::KeyRight::All};
		ourClassId.setValue(traits::class_name);
		win::RegistryKey ourClassIdServer{create_new, ourClassId, L"InProcServer32"sv, win::KeyRight::All};
		ourClassIdServer.setValue(std::wstring_view{modulePath});
		ourClassIdServer.setValue(L"ThreadingModel"sv, L"Apartment"sv);

		// Insert program-id registration
		win::RegistryKey allClasses{open_existing, win::ClassesRoot, win::KeyRight::All};
		win::RegistryKey ourProgId{create_new, allClasses, traits::program_id, win::KeyRight::All};
		ourProgId.setValue(traits::class_name);
		
		// Link the two ids
		win::RegistryKey ourClassIdProgId{create_new, ourClassId, L"ProgId"sv, win::KeyRight::All};
		ourClassIdProgId.setValue(traits::program_id);
		win::RegistryKey ourProgIdClassId{create_new, ourProgId, L"CLSID"sv, win::KeyRight::All};
		ourProgIdClassId.setValue(traits::class_guid.str());

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
		using namespace std::literals;

		// Remove class-id registration
		win::RegistryKey allClassIds{open_existing, win::ClassesRoot, L"CLSID"sv, win::KeyRight::All};
		{
			win::RegistryKey ourClassId{open_existing, allClassIds, traits::class_guid.str(), win::KeyRight::All};
			ourClassId.deleteSubKey(L"ProgId"sv);
			ourClassId.deleteSubKey(L"InProcServer32"sv);
		}
		allClassIds.deleteSubKey(traits::class_guid.str());
		
		// Remove program-id registration
		win::RegistryKey allClasses{open_existing, win::ClassesRoot, win::KeyRight::All};
		{
			win::RegistryKey ourProgId{open_existing, allClasses, traits::program_id, win::KeyRight::All};
			ourProgId.deleteSubKey(L"CLSID"sv);
		}
		allClasses.deleteSubKey(traits::program_id);
		return S_OK;
	}
	catch (std::exception const& /*e*/)
	{
		return E_FAIL;
	}
}
