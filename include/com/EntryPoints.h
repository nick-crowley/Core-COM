#pragma once
#include "library/core.COM.h"
#include "com/CoClassTraits.h"
#include "win/RegistryKey.h"
#include "com/Boolean.h"
#include "com/Function.h"
#include "com/SharedPtr.h"

namespace core::com
{
	std::atomic_long 
	extern com::numInstances;


	template <meta::CoClass CoClass>
	::HRESULT 
	getClassObject(Guid clsId, ::IID const& iid, void** ppv)
	{
		if (clsId != __uuidof(CoClass))
			return CLASS_E_CLASSNOTAVAILABLE;

		shared_ptr<::IClassFactory> factory{new coclass_factory_t<CoClass>{}};
		return factory->CreateInstance(nullptr,iid,ppv);
	}

	::HRESULT 
	canUnloadNow()
	{
		return Boolean{com::numInstances == 0};
	}

	template <meta::CoClass CoClass, typename Traits = coclass_traits<CoClass>>
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
		ourClassId[use_default] = Traits::class_name;
		RegistryKey ourServerPath = ourClassId.subkey(create_new, L"InProcServer32");
		ourServerPath[use_default] = std::wstring_view{modulePath};
		switch (Traits::apartment) {
		case ThreadingModel::Isolated: ourServerPath[L"ThreadingModel"] = std::wstring_view{L"Apartment"}; break;
		case ThreadingModel::Shared:   ourServerPath[L"ThreadingModel"] = std::wstring_view{L"Free"};      break;
		case ThreadingModel::Any:      ourServerPath[L"ThreadingModel"] = std::wstring_view{L"Both"};      break;
		}
		
		// Insert program-id registration
		RegistryKey ourProgId{create_new, win::ClassesRoot, Traits::program_id, KeyRight::All};
		ourProgId[use_default] = Traits::class_name;
		
		// Link the two ids
		RegistryKey{create_new, ourClassId, L"ProgId", KeyRight::All}[use_default] = Traits::program_id;
		RegistryKey{create_new, ourProgId, L"CLSID", KeyRight::All}[use_default] = Traits::class_guid.wstr();

		return S_OK;
	}
	catch (std::exception const& /*e*/)
	{
		return E_FAIL;
	}
	
	template <meta::CoClass CoClass, typename Traits = coclass_traits<CoClass>>
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
			RegistryKey ourProgId{allClasses, Traits::program_id, KeyRight::All};
			ourProgId.removeKey(L"CLSID");
		}
		allClasses.removeKey(Traits::program_id);
		return S_OK;
	}
	catch (std::exception const& /*e*/)
	{
		return E_FAIL;
	}
}
