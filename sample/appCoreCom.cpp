#include "library/core.COM.h"
#include "com/Annotations.h"
#include "com/SharedPtr.h"
#include "core/LiteralString.h"
#include <comutil.h>
using namespace core;
using namespace com::literals;

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
	testComSharedPtr();
}
