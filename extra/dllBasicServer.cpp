#include "library/core.COM.h"
#include "com/Annotations.h"
#include "com/Implements.h"
#include "com/EntryPoints.h"
#include "com/SetLastError.h"
using namespace core;

[module(unspecified, name="CustomLib", version="1.0", uuid="A10C8092-3549-4C2E-95D7-F264286720B9")];

[uuid("A10C8092-3549-4C2E-95D7-F264286720B9")]
struct CustomLibType
{
	// By default, the library name is obtained using reflection but MSVC won't permit us to
	//  re-use `CustomLib` after using it in the `module` definition (above) so we must specify 
	//  the library name explicitly
	auto constexpr 
	static library_name = core::LiteralString{"CustomLib"};
};

[object, uuid("9E66A290-4365-11D2-A997-00C04FA37DDB")]
__interface ICustom : IUnknown
{
	HRESULT Method1(com::in_t<wchar_t const> a);

	HRESULT Method2(com::in_t<wchar_t const> a, long b, com::retval_t<long> out);
};

[uuid("E46E39ED-E221-4F71-8E7A-6FBF30FA7692")]
class CoCustom : public com::implements<ICustom,::IUnknown>
{
public:
	using library_type = CustomLibType;
	
public:
	CoCustom() = default;

public:
	HRESULT 
	COMAPI Method1(com::in_t<wchar_t const> a) override
	{
		std::wcout << "CoCustom::Method1() => E_NOTIMPL " << std::endl;
		return E_NOTIMPL;
	}

	HRESULT 
	COMAPI Method2(com::in_t<wchar_t const> a, long b, com::retval_t<long> out) override
	{
		*out = 42;

		if (b == 12)
			return com::SetLastError<CoCustom>(E_FAIL, L"Example exception message");

		std::wcout << "CoCustom::Method2(" << a << ", " << b << ") => S_OK (" << (long)*out << ")" << std::endl;
		return S_OK;
	}
};

extern "C"
HRESULT 
COMAPI DllCanUnloadNow()
{
	return com::canUnloadNow();
}

extern "C"
HRESULT 
COMAPI DllGetClassObject(CLSID const& clsId, IID const& iid, void** ppv)
{
	return com::getClassObject<CoCustom>(com::Guid{clsId}, iid, ppv);
}

extern "C"
HRESULT 
COMAPI DllRegisterServer()
{
	return com::registerServer<CoCustom>(
		GetModuleHandleW(nullptr)
	);
}

extern "C"
HRESULT 
COMAPI DllUnregisterServer()
{
	return com::unregisterServer<CoCustom>();
}

#if defined _M_IX86
//#pragma comment (linker, "/EXPORT:DllMain=_DllMain@12,PRIVATE")
#pragma comment (linker, "/EXPORT:DllRegisterServer=_DllRegisterServer@0,PRIVATE")
#pragma comment (linker, "/EXPORT:DllUnregisterServer=_DllUnregisterServer@0,PRIVATE")
#pragma comment (linker, "/EXPORT:DllGetClassObject=_DllGetClassObject@12,PRIVATE")
#pragma comment (linker, "/EXPORT:DllCanUnloadNow=_DllCanUnloadNow@0,PRIVATE")
#elif defined _M_X64
//#pragma comment (linker, "/EXPORT:DllMain,PRIVATE")
#pragma comment (linker, "/EXPORT:DllRegisterServer,PRIVATE")
#pragma comment (linker, "/EXPORT:DllUnregisterServer,PRIVATE")
#pragma comment (linker, "/EXPORT:DllGetClassObject,PRIVATE")
#pragma comment (linker, "/EXPORT:DllCanUnloadNow,PRIVATE")
#endif
