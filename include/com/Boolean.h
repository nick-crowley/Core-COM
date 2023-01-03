#pragma once
#include "comFramework.h"

namespace com
{
	class Boolean
	{
		bool value;
		
	public:
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
