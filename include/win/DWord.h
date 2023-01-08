#pragma once
#ifndef ComFramework_h_included
#	error Including this header directly may cause a circular dependency; include <comFramework.h> directly
#endif
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
