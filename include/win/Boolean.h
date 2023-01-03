#pragma once
#include "comFramework.h"

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
