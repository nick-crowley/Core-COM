#pragma once
#include "library/core.COM.h"
#include "com/BinaryString.h"

namespace core::com
{
	class Guid
	{
	private:
		using type = Guid;
		using reference = type&;

	public:
		::GUID  m_value = GUID_NULL;

		constexpr 
		explicit 
		Guid(::GUID const& g) noexcept;
		
		satisfies(Guid,
			constexpr IsDefaultConstructible noexcept,
			constexpr IsCopyConstructible noexcept,
			constexpr IsCopyAssignable noexcept,
			constexpr IsDestructible noexcept,
			constexpr NotSortable
		);
		
		reference constexpr
		operator=(::GUID const& g) noexcept {
			// FIXME: Consuming modules receive 'LNK2001 unresolved external symbol' unless defined here
			this->m_value = g;
			return *this;
		}

		Guid 
		static FromProgId(std::wstring_view str);

		Guid 
		static FromString(std::wstring_view str);

		Guid 
		static Generate();

		wstring 
		str() const;
		
		constexpr 
		implicit operator 
		::GUID const&() const noexcept { 
			// FIXME: Consuming modules receive 'LNK2001 unresolved external symbol' unless defined here
			return this->m_value; 
		}

		bool 
		operator==(type const& r) const noexcept;
		
		bool 
		operator!=(type const& r) const noexcept;
	};
	
	std::wstring
	to_wstring(Guid const& g);

	template <typename T>
	Guid const
	inline guid_v { __uuidof(T) };

	namespace literals
	{		
		com::Guid
		operator""_guid(wchar_t const* guid, size_t count);
	}
}

bool 
operator==(::GUID const& l, core::com::Guid const& r) noexcept;

bool 
operator!=(::GUID const& l, core::com::Guid const& r) noexcept;

bool 
operator==(core::com::Guid const& l, ::GUID const& r) noexcept;

bool 
operator!=(core::com::Guid const& l, ::GUID const& r) noexcept;
