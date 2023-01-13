#pragma once
#include "comFramework.h"
#include "com/Function.h"

namespace core::com
{
	class InitializeSentry
	{
		static constexpr auto coInitializeEx = function(CoInitializeEx);

	public:
		InitializeSentry(DWORD flags) 
		{
			coInitializeEx(nullptr, flags);
		}

		~InitializeSentry() 
		{
			::CoUninitialize();
		}
	};
}
