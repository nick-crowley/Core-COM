#include "library/core.COM.h"
#include "com/Annotations.h"
#include "com/BinaryString.h"
#include "com/SharedPtr.h"
#include "core/LiteralString.h"
#include <comutil.h>
using namespace core;
using namespace com::literals;

// __________  _________ __         .__                
// \______   \/   _____//  |________|__| ____    ____  
//  |    |  _/\_____  \\   __\_  __ \  |/    \  / ___\ 
//  |    |   \/        \|  |  |  | \/  |   |  \/ /_/  >
//  |______  /_______  /|__|  |__|  |__|___|  /\___  / 
//         \/        \/                     \//_____/  
// 
///////////////////////////////////////////////////////// 

void testComString()
{	
	com::wstring constexpr
	static ss1;

	auto constexpr
	static ss2 = L"abc"_bstr;

	// Test Bstring wrapper
	scoped {
		com::wstring{};
	}

	scoped {
		com::wstring s0 = ss2;
		auto c = s0.front();
	}

	scoped {
		com::wstring s1{L"hello"};
		s1 = L"hello";
		std::wstring_view sv1 = s1;

		auto* tmp = s1.c_str();
		tmp = s1.data();
		auto it = s1.begin();
		it = s1.end();
		auto c = s1.front();
		c = s1.back();
	}

	scoped {
		com::wstring s2{adopt,com::HeapAllocator<wchar_t>{}.allocate(5)};
		s2 = L"hello";
		std::wstring_view sv2 = s2;
	}

	scoped {
		com::wstring s1{L"hello"};
		com::wstring s2 = s1;
		s2 = std::move(s1);
		s1 = std::move(s2);
		com::wstring s3 = std::move(s1);
	}

	std::wstring s3{L"abc"};
	com::wstring s4{s3.begin(),s3.end()};
	s4.assign(s3.begin(),s3.end());

	std::wstring_view sv4 = s4;

	//_variant_t t;
	//_bstr_t b;

}

void testLiteralString()
{	
	// Test constexpr string
	wchar_t buf[10] {L"abcdfeg"};
	LiteralString cs{buf};

	wchar_t constexpr lhs[10] {L"abcdfeg"};
	wchar_t constexpr rhs[10] {L"abcdfeg"};
	auto constexpr cs2 = LiteralString{lhs} + LiteralString{rhs};
	auto constexpr cs3 = LiteralString{lhs} + L'x';
}

//                 __                      .__                             .___ 
//   _____ _____  |  | __ ____        _____|  |__ _____ _______   ____   __| _/ 
//  /     \\__  \ |  |/ // __ \      /  ___/  |  \\__  \\_  __ \_/ __ \ / __ |  
// |  Y Y  \/ __ \|    <\  ___/      \___ \|   Y  \/ __ \|  | \/\  ___// /_/ |  
// |__|_|  (____  /__|_ \\___  >____/____  >___|  (____  /__|    \___  >____ |  
//       \/     \/     \/    \/_____/    \/     \/     \/            \/     \/  
//
/////////////////////////////////////////////////////////////////////////////////

[module(unspecified, name="ExampleLib", version="1.0", uuid="D0DCF441-AA4C-4EAC-ACC1-32EDD2ED16F6")];

[object, uuid("FE2C8ED8-98DA-4BF7-BD5E-0AA1D38FB5E6")]
__interface IExample : IUnknown
{
	HRESULT 
	Method(long idx, com::in_t<wchar_t const> str, com::retval_t<long> out);
};

[uuid("42C386F4-95A0-43A7-B24C-7288D31E98C2")]
class Example : public com::implements<IExample,::IUnknown>
{
public:
	struct library_type {
		auto constexpr 
		static library_name = core::LiteralString{"ExampleLib"};

		auto constexpr 
		static library_guid = "D0DCF441-AA4C-4EAC-ACC1-32EDD2ED16F6"_guid;
	};
	
public:
	Example() = default;

public:
	HRESULT 
	COMAPI Method(long idx, com::in_t<wchar_t const> str, com::retval_t<long> out) override
	{
		return S_OK;
	}
};

void testComSharedPtr()
{
	// Test com::make_shared()
	auto ptr = com::make_shared<IExample,Example>();
}


//    ________ ____ ___.___________          
//   /  _____/|    |   \   \______ \   ______
//  /   \  ___|    |   /   ||    |  \ /  ___/
//  \    \_\  \    |  /|   ||    `   \\___ \ 
//   \______  /______/ |___/_______  /____  >
//          \/                     \/     \/ 
/////////////////////////////////////////////////

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        type constexpr static name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

// "{FE2C8ED8-98DA-4BF7-BD5E-0AA1D38FB5E6}"
MIDL_DEFINE_GUID(GUID, IID_IExample,0xFE2C8ED8,0x98DA,0x4BF7,0xBD,0x5E,0x0A,0xA1,0xD3,0x8F,0xB5,0xE6);

// "{D0DCF441-AA4C-4EAC-ACC1-32EDD2ED16F6}"
MIDL_DEFINE_GUID(GUID, LIBID_ExampleLib,0xD0DCF441,0xAA4C,0x4EAC,0xAC,0xC1,0x32,0xED,0xD2,0xED,0x16,0xF6);

// "{42C386F4-95A0-43A7-B24C-7288D31E98C2}"
MIDL_DEFINE_GUID(GUID, CLSID_Example,0x42C386F4,0x95A0,0x43A7,0xB2,0x4C,0x72,0x88,0xD3,0x1E,0x98,0xC2);

void testComGuid()
{
	constexpr static com::Guid gN;

	auto constexpr static g1 = com::Guid::fromString(L"{FE2C8ED8-98DA-4BF7-BD5E-0AA1D38FB5E6}");
	bool result = g1 == IID_IExample;

	auto constexpr static g2 = com::Guid::fromString(L"{D0DCF441-AA4C-4EAC-ACC1-32EDD2ED16F6}");
	result = g2 == LIBID_ExampleLib;
	
	auto constexpr static g4 = com::Guid::fromString("{FE2C8ED8-98DA-4BF7-BD5E-0AA1D38FB5E6}");
	result = g4 == IID_IExample;

	auto constexpr static g5 = "{42C386F4-95A0-43A7-B24C-7288D31E98C2}"_guid;
	result = g5 == CLSID_Example;

	auto constexpr static r1 = g1 == g2;
	auto constexpr static r2 = g1 == IID_IExample;
	auto constexpr static r3 = IID_IExample == g1;

}


//                .__        
//   _____ _____  |__| ____  
//  /     \\__  \ |  |/    \ 
// |  Y Y  \/ __ \|  |   |  \
// |__|_|  (____  /__|___|  /
//       \/     \/        \/ 
/////////////////////////////// 

int main()
{
	testComString();
	testLiteralString();
	testComSharedPtr();
	testComGuid();
}
