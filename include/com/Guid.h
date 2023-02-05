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
		static fromProgId(std::wstring_view str);

		Guid 
		static fromString(std::wstring_view str);

		Guid 
		static generate();
		
		wstring 
		wstr() const;
		
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
	
	std::string
	to_string(Guid const& g);

	std::wstring
	to_wstring(Guid const&);

	template <typename T>
	Guid const
	inline guid_v { __uuidof(T) };

	namespace literals
	{
		com::Guid
		operator""_guid(gsl::cwzstring guid, size_t count);
	}
}

std::string 
to_string(::GUID const& g);

std::wstring 
to_wstring(::GUID const& g);

bool 
operator==(::GUID const& l, core::com::Guid const& r) noexcept;

bool 
operator!=(::GUID const& l, core::com::Guid const& r) noexcept;

bool 
operator==(core::com::Guid const& l, ::GUID const& r) noexcept;

bool 
operator!=(core::com::Guid const& l, ::GUID const& r) noexcept;
