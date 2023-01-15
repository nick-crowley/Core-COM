#pragma once
#include "library/core.COM.h"

namespace core::com
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
