#include "library/core.COM.h"
#include "com/Annotations.h"
#include "com/Implements.h"
#include "com/EntryPoints.h"
#include "com/SetLastError.h"
using namespace core;
using namespace com::literals;

// Auto-generate IDL from the source file
[module(unspecified, name="InProcServerLib", version="1.0", uuid="A10C8092-3549-4C2E-95D7-F264286720B9")];

[object, uuid("9E66A290-4365-11D2-A997-00C04FA37DDB")]
__interface IInProcServer : IUnknown
{
	::HRESULT 
	Method1(long idx);

	::HRESULT 
	Method2(long idx, com::retval_t<long> out);
};

[uuid("E46E39ED-E221-4F71-8E7A-6FBF30FA7692")]
class InProcServer : public com::implements<IInProcServer,::IUnknown>
{
public:
	struct library_type {
		// COM classes must have a member type named `library_type` but we must explicitly
		// override the library name which would otherwise be obtained via reflection
		LiteralString constexpr
		static library_name {"InProcServerLib"};

		// COM attributes aren't supported on nested classes so this is required
		com::Guid constexpr
		static library_guid = "A10C8092-3549-4C2E-95D7-F264286720B9"_guid;
	};
	
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

extern "C"
::HRESULT 
COMAPI DllGetClassObject(::CLSID const& clsId, ::IID const& iid, void** ppv)
{
	clog.attach(std::cout);
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
	return com::registerServer<InProcServer>(
		::GetModuleHandleW(nullptr)
	);
}

extern "C"
::HRESULT 
COMAPI DllUnregisterServer()
{
	return com::unregisterServer<InProcServer>();
}
