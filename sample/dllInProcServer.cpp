#include "library/core.COM.h"
#include "com/Annotations.h"
#include "com/Implements.h"
#include "com/EntryPoints.h"
#include "com/SetLastError.h"
#include "win/Module.h"
using namespace core;
using namespace com::literals;

// Define program meta-data
metadata std::string_view meta::Settings<program_name> = "In-process COM server demo";
metadata std::string_view meta::Settings<program_version> = "1.0";

// Disable code injection
[no_injected_text(true)];

// Inserts @c library block into IDL with specified @c name and @c uuid
//  NB: CORE properties must supply same values as [attributes] or type-library content will differ from code
[
	module(unspecified, name="InProcServerLib", uuid="A10C8092-3549-4C2E-95D7-F264286720B9"),
	uuid="A10C8092-3549-4C2E-95D7-F264286720B9"
]
struct InProcServerLib
{
	// [optional] Library version of v1.0 is defined implicitly, by omission  (see com::library_traits)
	// [mandatory] For any version other than v1.0
};


// Interfaces must be declared using non-standard `__interface` keyword
// Inserts @c interface block into IDL with specified @c uuid
[
	object,
	uuid("9E66A290-4365-11D2-A997-00C04FA37DDB")
]
__interface IInProcServer : IUnknown
{
	::HRESULT 
	Method1(long idx);

	::HRESULT 
	Method2(long idx, [out, retval] long* out);
};


// Inserts @c coclass block into IDL with specified @c uuid and @c default
//  NB: CORE properties must supply same values as [attributes] or type-library content will differ from code
[
	coclass, 
	default(IInProcServer),
	uuid("E46E39ED-E221-4F71-8E7A-6FBF30FA7692")
]
class InProcServer : public com::implements<IInProcServer,::IUnknown>
{
public:
	// [mandatory] Library containing this coclass
	using library_type = InProcServerLib;

	// [optional] Coclass name of 'InProcServer' is obtained via reflection  (see com::coclass_traits)

	// [optional] Coclass threading-model of 'Apartment' is defined implicitly, by omission  (see com::coclass_traits)

	// [optional] Coclass GUID is obtained from the COM attributes  (see com::coclass_traits)

	// [optional] Coclass version of v1.0 is defined implicitly, by omission  (see com::coclass_traits)
	// [mandatory] For any version other than v1.0

	// [NOTE] Stock ::IClassFactory implementation provided by `ClassFactory<InProcServer>`  (see com::ClassFactory)
	
public:
	::HRESULT 
	COMAPI Method1(long idx) override
	{
		clog << Verbose{"InProcServer::Method1({}) => E_FAIL", idx};

		return com::SetLastError<InProcServer>(E_FAIL, L"Failed for demonstration purposes");
	}

	::HRESULT 
	COMAPI Method2(long idx, com::retval_t<long> out) override
	{
		clog << Verbose{"InProcServer::Method2({}) => S_OK", idx};
		
		*out = 42;
		return S_OK;
	}
};


extern "C"
::HRESULT 
COMAPI DllCanUnloadNow()
{
	return com::canUnloadNow();
}

void
intern InitializeLogFile() {
	std::once_flag f;
	std::call_once(f, []{ 
		clog.createLogFile("InProcServer.log"); 
		startupBanner();
	});
}

extern "C"
::HRESULT 
COMAPI DllGetClassObject(::CLSID const& clsId, ::IID const& iid, void** ppv)
{
	InitializeLogFile();
	return com::getClassObject<InProcServer>(com::Guid{clsId}, iid, ppv);
}

extern "C"
BOOL
WINAPI DllMain(::HINSTANCE hinstDll, DWORD dwReason, void *)
{
    return TRUE;
}

extern "C"
::HRESULT 
COMAPI DllRegisterServer()
{
	InitializeLogFile();
	using enum com::ServerLocation;
	return com::registerServer<InProcServer,InProcess>(win::DllModule.path().native());
}

extern "C"
::HRESULT 
COMAPI DllUnregisterServer()
{
	InitializeLogFile();
	using enum com::ServerLocation;
	return com::unregisterServer<InProcServer,InProcess>();
}
