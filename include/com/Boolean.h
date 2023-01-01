#pragma once
#include "comFramework.h"

namespace com
{
	struct Boolean
	{
		bool value;
		
		constexpr 
		explicit 
		Boolean(bool expr) : value{expr}
		{
		}

		implicit operator 
		HRESULT() const 
		{
			return this->value ? S_OK : S_FALSE;
		}
	};
}

namespace win
{
	struct Boolean
	{
		bool value;
		
		constexpr 
		explicit 
		Boolean(bool expr) : value{expr}
		{
		}

		implicit operator 
		BOOL() const 
		{
			return this->value ? TRUE : FALSE;
		}
	};
}
