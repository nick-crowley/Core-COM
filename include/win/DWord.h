#pragma once
//#include "comFramework.h"       // Warning: To avoid circular dependency, include <comFramework.h> directly
#include "../src/PlatformSdk.h"
#include "meta/Concepts.h"
#include "nstd/experimental/implicit.h"

namespace win
{
	template <meta::ExplicitlyConvertible<::DWORD> ValueType> 
	class DWord
	{
		ValueType const& m_value;

	public:
		constexpr 
		explicit DWord(ValueType const& val)
		  : m_value{val}
		{}

	public:
		constexpr 
		implicit operator 
		::DWORD() const
		{
			return static_cast<::DWORD>(this->m_value);
		}
	};
}
