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
		
		satisfies(Boolean,
			NotDefaultConstructible,
            constexpr IsCopyable noexcept,
			constexpr IsEqualityComparable noexcept,
			NotSortable
		);

	public:
		constexpr
		implicit operator 
		::HRESULT() const 
		{
			return this->value ? S_OK : S_FALSE;
		}
	};
}
