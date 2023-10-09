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

//                 __                      .__                             .___ 
//   _____ _____  |  | __ ____        _____|  |__ _____ _______   ____   __| _/ 
//  /     \\__  \ |  |/ // __ \      /  ___/  |  \\__  \\_  __ \_/ __ \ / __ |  
// |  Y Y  \/ __ \|    <\  ___/      \___ \|   Y  \/ __ \|  | \/\  ___// /_/ |  
// |__|_|  (____  /__|_ \\___  >____/____  >___|  (____  /__|    \___  >____ |  
//       \/     \/     \/    \/_____/    \/     \/     \/            \/     \/  
//
/////////////////////////////////////////////////////////////////////////////////

[emitidl(restricted)];

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
	testComSharedPtr();
}
