#include "library/core.COM.h"
#include "com/Function.h"
#include "com/Implements.h"
#include "win/RegistryKey.h"
#include "com/ClassFactory.h"
#include "com/Dispatch.h"
#include "com/EntryPoints.h"
#include "com/BinaryString.h"
#include "com/Apartments.h"
#include "com/SetLastError.h"
#include "com/LibraryTraits.h"
#include "core/LiteralString.h"
#include "CustomLib.h"
#include <comutil.h>
using namespace std::string_view_literals;
using namespace core;

std::atomic_long 
com::g_numInstances = 0;

[module(unspecified, name="CustomLib", version="1.0", uuid="A10C8092-3549-4C2E-95D7-F264286720B9")];

[object, uuid("9E66A290-4365-11D2-A997-00C04FA37DDB")]
__interface ICustom : IDispatch
{
	HRESULT Method1(com::in_t<wchar_t const> a);
	HRESULT Method2(com::in_t<wchar_t const> a, long b, com::retval_t<long> out);
};

[object, uuid("EB234EE2-A28F-4BE8-88B5-0802A812FB3A")]
__interface ICustom2 : IDispatch
{
   HRESULT Method3(com::in_t<wchar_t const> a, long b, com::out_t<float> c, com::retval_t<wchar_t*> out);
};

//#define DISPATCH_SUPPORT_MULTIPLE_INTERFACES 1 

[uuid("E46E39ED-E221-4F71-8E7A-6FBF30FA7692")]
class CoCustom : public com::Dispatch<ICustom>
{
	using base = com::Dispatch<ICustom>;

public:
	using library_type = CustomLib_t;
	
	com::ThreadingModel constexpr 
	static apartment = com::ThreadingModel::Any;

	auto constexpr 
	static class_name = LiteralString{L"CoCustom"};

	auto constexpr 
	static class_version = com::Version{1,0};

public:
	CoCustom() 
	  : base{this}
	{}

public:
	HRESULT 
	__stdcall Method1(com::in_t<wchar_t const> a) override
	{
		std::wcout << "CoCustom::Method1() => E_NOTIMPL " << std::endl;
		return E_NOTIMPL;
	}

	HRESULT 
	__stdcall Method2(com::in_t<wchar_t const> a, long b, com::retval_t<long> out) override
	{
		*out = 42;

		if (b == 12)
			return com::SetLastError<CoCustom>(E_FAIL, L"Example exception message");

		std::wcout << "CoCustom::Method2(" << a << ", " << b << ") => S_OK (" << (long)*out << ")" << std::endl;
		return S_OK;
	}

#if DISPATCH_SUPPORT_MULTIPLE_INTERFACES
	HRESULT
	__stdcall Method3(com::in_t<wchar_t const> a, long b, com::out_t<float> out1, com::retval_t<wchar_t*> out2) override
	{
		*out1 = 1.25f;
		*out2 = L"hello world";
		std::wcout << "CoCustom::Method3(" << a << ", " << b << ") => S_OK (" << *out1 << ", '" << *out2 << "')" << std::endl;
		return S_OK;
	}
#endif // DISPATCH_SUPPORT_MULTIPLE_INTERFACES

};

extern "C"
HRESULT 
__stdcall DllCanUnloadNow()
{
	return com::canUnloadNow();
}

extern "C"
HRESULT 
__stdcall DllGetClassObject(CLSID const& clsId, IID const& iid, void** ppv)
{
	return com::getClassObject<CoCustom>(com::Guid{clsId}, iid, ppv);
}

extern "C"
HRESULT 
__stdcall DllRegisterServer()
{
	return com::registerServer<CoCustom>(
		GetModuleHandleW(nullptr)
	);
}

extern "C"
HRESULT 
__stdcall DllUnregisterServer()
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

class CoCustomWrapper
{
	CoCustom Object;
	
public:
	void 
	method1(com::in_t<wchar_t const> a) 
	{
		auto fx = com::method(&ICustom::Method1);
		return fx(this->Object,a);
	}
	
	long
	method2(com::in_t<wchar_t const> a, long b) 
	{
		auto fx = com::method<1>(&ICustom::Method2);
		return fx(this->Object,a,b);
	}

#if DISPATCH_SUPPORT_MULTIPLE_INTERFACES
	auto /*std::tuple<float,wchar_t*> */
	method3(com::in_t<wchar_t const> a, long b) 
	{
		auto fx = com::method<2>(&ICustom2::Method3);
		return fx(this->Object,a,b);
	}
#endif
};

// 
int main()
{
	com::IsolatedApartment apartment;

	// Test coclass wrapper
	CoCustomWrapper wrapper;
	wrapper.method1(L"Test");
	long result = wrapper.method2(L"Hello", 42);
	result = wrapper.method2(L"Error", 12);

#if DISPATCH_SUPPORT_MULTIPLE_INTERFACES
	auto [a,b] = wrapper.method3(L"World", 5);
#endif

	// Test Bstring wrapper
	com::wstring s{ L"hello"};
	s = L"hello";

	//_variant_t t;
	//_bstr_t b;

	// Test constexpr string
	wchar_t buf[10] {L"abcdfeg"};
	LiteralString cs{buf};

	wchar_t constexpr lhs[10] {L"abcdfeg"};
	wchar_t constexpr rhs[10] {L"abcdfeg"};
	auto constexpr cs2 = LiteralString{lhs} + LiteralString{rhs};
	auto constexpr cs3 = LiteralString{lhs} + L'x';

	// Test make_com() func
	auto ptr = com::make_shared<CoCustom,ICustom>();
}
