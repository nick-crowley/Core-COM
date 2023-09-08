#include "library/core.COM.h"
#include "com/Annotations.h"
#include "com/Implements.h"
#include "com/EntryPoints.h"
#include "com/SetLastError.h"
#include "win/Module.h"
using namespace core;

// Manually specify IDL imports (prevents errors from unwanted IDLs being dragged in)
[emitidl(true, defaultimports=false)];
[import(docobj.idl)];

// Disable all ATL code injection (allows library class to share same name as `[module(...)]` directive)
[no_injected_text(true)];

[uuid("A10C8092-3549-4C2E-95D7-F264286720B9")]
struct AttributesLib
{
};

// Auto-generate IDL from the source file (must be placed after declaration of 'AttributesLib')
[module(unspecified, name="AttributesLib", version="1.0", uuid="A10C8092-3549-4C2E-95D7-F264286720B9")];

[object, uuid("9E66A290-4365-11D2-A997-00C04FA37DDB")]
__interface IAttributes : IUnknown
{
	::HRESULT 
	Method1(long idx);

	::HRESULT 
	Method2(long idx, com::retval_t<long> out);
};

[uuid("E46E39ED-E221-4F71-8E7A-6FBF30FA7692")]
class Attributes : public com::implements<IAttributes,::IUnknown>
{
public:
	using library_type = AttributesLib;
	
public:
	::HRESULT 
	COMAPI Method1(long idx) override
	{
		clog << Verbose{"Attributes::Method1({}) => E_FAIL", idx};

		return com::SetLastError<Attributes>(E_FAIL, L"Example exception message");
	}

	::HRESULT 
	COMAPI Method2(long idx, com::retval_t<long> out) override
	{
		clog << Verbose{"Attributes::Method2({}) => S_OK", idx};
		
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
	return com::getClassObject<Attributes>(com::Guid{clsId}, iid, ppv);
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
	return com::registerServer<Attributes>(win::DllModule.path().native());
}

extern "C"
::HRESULT 
COMAPI DllUnregisterServer()
{
	return com::unregisterServer<Attributes>();
}
