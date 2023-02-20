#pragma once
#include "library/core.COM.h"
#include "com/BinaryString.h"

bool
ComExport operator<(::GUID const& l, ::GUID const& r);

namespace core::com
{
	class ComExport Guid
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

	class ComExport KnownGuidCollection : private std::map<::GUID,std::string_view> {
		using base = std::map<::GUID,std::string_view>;

	public:
		using iterator = typename base::iterator;
		using const_iterator = typename base::const_iterator;
		using value_type = typename base::value_type;
		using reference = typename base::reference;
		using const_reference = typename base::const_reference;
		using difference_type = typename base::difference_type;
		using size_type = typename base::size_type;

	public:
		KnownGuidCollection();

	public:
		template <typename Self>
		auto 
		begin(this Self&& self) {
			return base::begin();
		}

		template <typename Self>
		auto 
		end(this Self&& self) {
			return base::end();
		}

		bool
		contains(::GUID const& g) const {
			return base::contains(g);
		}

		std::string_view
		operator[](::GUID const& g) const {
			return base::at(g);
		}

	public:
		void
		add(::GUID guid, std::string_view name) {
			this->emplace(guid,name);
		}
	};
	
	std::string
	ComExport to_string(Guid const& g);

	std::wstring
	ComExport to_wstring(Guid const&);

	template <typename T>
	Guid const
	inline guid_v { __uuidof(T) };

	namespace literals
	{
		com::Guid
		ComExport operator""_guid(gsl::cwzstring guid, size_t count);
	}

	KnownGuidCollection
	inline ComExport KnownGuids;
}

std::string 
ComExport to_string(::GUID const& g);

std::wstring 
ComExport to_wstring(::GUID const& g);

bool 
ComExport operator==(::GUID const& l, core::com::Guid const& r) noexcept;

bool 
ComExport operator!=(::GUID const& l, core::com::Guid const& r) noexcept;

bool 
ComExport operator==(core::com::Guid const& l, ::GUID const& r) noexcept;

bool 
ComExport operator!=(core::com::Guid const& l, ::GUID const& r) noexcept;

bool
ComExport operator<(::GUID const& l, ::GUID const& r);
	