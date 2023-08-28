#include "library/core.COM.h"
#include "com/Annotations.h"
#include "com/Implements.h"
#include "com/EntryPoints.h"
#include "com/SetLastError.h"
using namespace core;

// Manually specify IDL imports (prevents errors from unwanted IDLs being dragged in)
[emitidl(true, defaultimports=false)];
[import(docobj.idl)];

// Disable all ATL code injection (allows library class to share same name as `[module(...)]` directive)
[no_injected_text(true)];

[uuid("A10C8092-3549-4C2E-95D7-F264286720B9")]
struct BasicServerLib
{
};

// Auto-generate IDL from the source file (must follow declaration of 'BasicServerLib')
[module(unspecified, name="BasicServerLib", version="1.0", uuid="A10C8092-3549-4C2E-95D7-F264286720B9")];

[object, uuid("9E66A290-4365-11D2-A997-00C04FA37DDB")]
__interface IBasicServer : IUnknown
{
	::HRESULT 
	Method1(long idx);

	::HRESULT 
	Method2(long idx, com::retval_t<long> out);
};

[uuid("E46E39ED-E221-4F71-8E7A-6FBF30FA7692")]
class BasicServer : public com::implements<IBasicServer,::IUnknown>
{
public:
	using library_type = BasicServerLib;
	
public:
	::HRESULT 
	COMAPI Method1(long idx) override
	{
		clog << Verbose{"BasicServer::Method1({}) => E_FAIL", idx};

		return com::SetLastError<BasicServer>(E_FAIL, L"Example exception message");
	}

	::HRESULT 
	COMAPI Method2(long idx, com::retval_t<long> out) override
	{
		clog << Verbose{"BasicServer::Method2({}) => S_OK", idx};
		
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
	return com::getClassObject<BasicServer>(com::Guid{clsId}, iid, ppv);
}

extern "C"
::HRESULT 
COMAPI DllRegisterServer()
{
	return com::registerServer<BasicServer>(
		::GetModuleHandleW(nullptr)
	);
}

extern "C"
::HRESULT 
COMAPI DllUnregisterServer()
{
	return com::unregisterServer<BasicServer>();
}

#if defined _M_IX86
#pragma comment (linker, "/EXPORT:DllRegisterServer=_DllRegisterServer@0,PRIVATE")
#pragma comment (linker, "/EXPORT:DllUnregisterServer=_DllUnregisterServer@0,PRIVATE")
#pragma comment (linker, "/EXPORT:DllGetClassObject=_DllGetClassObject@12,PRIVATE")
#pragma comment (linker, "/EXPORT:DllCanUnloadNow=_DllCanUnloadNow@0,PRIVATE")

#elif defined _M_X64
#pragma comment (linker, "/EXPORT:DllRegisterServer,PRIVATE")
#pragma comment (linker, "/EXPORT:DllUnregisterServer,PRIVATE")
#pragma comment (linker, "/EXPORT:DllGetClassObject,PRIVATE")
#pragma comment (linker, "/EXPORT:DllCanUnloadNow,PRIVATE")
#endif
