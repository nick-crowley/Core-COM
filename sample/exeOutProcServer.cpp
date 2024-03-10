#include "library/core.COM.h"
#include "com/Apartments.h"
#include "com/ClassObject.h"
#include "com/Implements.h"
#include "win/ManualResetEvent.h"
using namespace core;
using namespace com::literals;

// Define program meta-data
metadata std::string_view meta::Settings<program_name> = "Out-of-process COM server demo";
metadata std::string_view meta::Settings<program_version> = "1.0";

// Auto-generate IDL from the source file
[module(unspecified, name="OutProcServerLib", version="1.0", uuid="DD45780C-AFD6-45B6-8028-F22A7C8EECCD")];

// Prevent code-injection into attributed types
[no_injected_text(true)];

// Define COM interface in-line
[object, uuid("AB4CB84C-CDCC-4525-AFB0-0B0D5798078D")]
__interface IOutProcServer : IUnknown
{
	::HRESULT 
	Method();
};

// Provide a mechanism by which single-use object can communicate the expiration
//  of its lifetime back to the server (so it knows when to quit)
win::ManualResetEvent
intern g_InstancesDestroyed{false};

// Implementation of our interface
[coclass, default(IOutProcServer), uuid("04944F50-6F92-4F77-B405-E04BEED469F8")]
class OutProcServer : public com::implements<IOutProcServer,::IUnknown>
{
public:
	struct library_type {
		LiteralString constexpr
		static library_name {"OutProcServerLib"};

		com::Guid constexpr
		static library_guid = "DD45780C-AFD6-45B6-8028-F22A7C8EECCD"_guid;
	};
	
public:
	~OutProcServer() {
		g_InstancesDestroyed.signal();
	}

public:
	::HRESULT 
	COMAPI Method() override
	{
		clog << Important{"OutProcServer::Method() executing => S_OK"};
		return S_OK;
	}
};

// Out-of-process server
int16_t
intern run(std::wstring_view const cmdLine)
try {
	clog.createLogFile("outProcServer.log");
	startupBanner();

	com::SharedApartment apartment;
	com::coclass_factory_t<OutProcServer> factory;

	// Register/execute the class object
	com::ClassObject<OutProcServer> obj{factory, CLSCTX_LOCAL_SERVER, REGCLS_SINGLEUSE|REGCLS_SUSPENDED};
	com::resumeClassObjects();

	// Wait for the object it created to be destroyed
	win::waitFor(*g_InstancesDestroyed.handle());
	return 0;
}
catch (std::exception const& e)
{
	clog << e;
	return -1;
}

int 
WINAPI wWinMain(::HINSTANCE hInstance, ::HINSTANCE, wchar_t* szCmdLine, int nCmdShow) {
	auto const r = ::run(szCmdLine);
	clog << Important{"Process terminating"};
	return r;
}