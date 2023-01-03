#pragma once
#include "comFramework.h"

namespace win
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
		BOOL() const 
		{
			return this->value ? TRUE : FALSE;
		}
	};
}
