/* o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o */ /*!
* @copyright	Copyright (c) 2023, Nick Crowley. All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted
* provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions
*    and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of
*    conditions and the following disclaimer in the documentation and/or other materials provided
*    with the distribution.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
* FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
* WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
* The views and conclusions contained in the software and documentation are those of the author 
* and should not be interpreted as representing official policies, either expressed or implied, of
* the projects which contain it.
*/
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Preprocessor Directives o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#pragma once
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Header Files o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#include "library/core.COM.h"
#include "com/HeapAllocator.h"
#include "com/NoopAllocator.h"
#include "core/ZString.h"
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com 
{
	namespace detail 
	{
		template <std::forward_iterator BeginIterator, std::forward_iterator EndIterator>
			requires nstd::IndirectlyConvertibleTo<BeginIterator, wchar_t>
			      && nstd::IndirectlyConvertibleTo<EndIterator, wchar_t>
			      && requires(BeginIterator a, EndIterator b) { a <= b; }
		bool constexpr
		is_valid_range(BeginIterator pos, EndIterator end)
        {
			if constexpr (std::is_pointer_v<BeginIterator> 
			           && std::is_pointer_v<EndIterator>)
				return static_cast<void const* const>(pos) <= static_cast<void const* const>(end);
			
			else if constexpr (std::random_access_iterator<BeginIterator>
			                && std::random_access_iterator<EndIterator>)
				return pos <= end;
			
            else
				return true;
		}
	}
	
	/**
	 * @brief	Binary string (ie. BSTR) with character-based sequence container interface
	*/
	class BinaryString
	{
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		using type = BinaryString;
		using base = void;
		
		template <typename Self, typename T, typename F>
		using if_const_then_t = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>, T, F>; 

    public:
		using allocator_type = void;
		using iterator = gsl::wzstring;
		using const_iterator = gsl::cwzstring;
		using size_type = uint32_t;
		using value_type = wchar_t;
		using difference_type = ptrdiff_t;
		using reference = wchar_t&;
		using const_reference = wchar_t const&;
		
	private:
		wchar_t constexpr
		inline static null = L'\0';
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		gsl::wzstring Buffer = nullptr;
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		/**
		 * @brief	Construct null @c BSTR
		*/
		constexpr 
		BinaryString() noexcept = default;
		
		//! @brief	Unnecessary; use the default-constructor to create null @c BSTR
		constexpr 
		BinaryString(nullptr_t) noexcept = delete;

		/**
		 * @brief	Construct with elements copied from a (possibly empty) input range
		 * @details	If both @p pos and @p end are @c nullptr then a null @c BSTR is created
		 * 
		 * @param pos		First element in range
		 * @param end		Immediately beyond last element in range
		 * 
		 * @throws std::bad_alloc          Out of memory
		 * @throws std::invalid_argument   Missing either argument (but not both)
		 * @throws std::invalid_argument   @p pos and @p end specify an invalid range
		*/
		template <std::forward_iterator BeginIterator, std::forward_iterator EndIterator>
			requires nstd::IndirectlyConvertibleTo<BeginIterator, wchar_t>
			      && nstd::IndirectlyConvertibleTo<EndIterator, wchar_t>
		BinaryString(BeginIterator pos, EndIterator end)
        {
			if constexpr (std::is_pointer_v<BeginIterator> && std::is_pointer_v<EndIterator>) {
				if (!pos != !end) {
					ThrowIfNull(pos);
					ThrowIfNull(end);
				}
			}

			this->assign(pos,end);
		}
		
		/**
		 * @brief	Construct with elements copied from a null-terminated C-string
		 * @details	If @p src is @c nullptr then a null @c BSTR is created
		 * 
		 * @param str	[optional] Null-terminated source buffer
		 * 
		 * @throws std::bad_alloc      Out of memory
		*/
		explicit 
		BinaryString(gsl::cwzstring const str)
        {
			if (str)
				this->assign(str, str + BinaryString::measure(str));
		}
		
		/**
		 * @brief	Take ownership of elements within a null-terminated buffer
		 * @details	If @p src is @c nullptr then a null @c BSTR is created
		 * 
		 * @param str	[optional] Null-terminated source buffer
		*/
		constexpr
		BinaryString(meta::adopt_t, gsl::wzstring const str) noexcept
		  : Buffer{str}
        {
		}
		
#if SUPPORT_ATL_STRING
		/**
		 * @brief	Construct with elements copied from an ATL string
		 *
		 * @param str		ATL string
		 * 
		 * @throws std::bad_alloc		Out of memory
		*/
		explicit BinaryString(ATL::CSimpleStringT<wchar_t> const& str) 
			: BinaryString(str.GetString(), str.GetString() + str.GetLength())
		{
		}

		/**
		 * @brief	Prevent moving elements from ATL string
		*/
		constexpr 
		BinaryString(ATL::CSimpleStringT<wchar_t>&&) noexcept = delete;
#endif // 0

		/**
		 * @brief	Construct with elements copied from a @c std::basic_string
		 * 
		 * @param str		Standard library string
		 * 
		 * @throws std::bad_alloc		Out of memory
		*/
		template <typename C, typename T, typename A>
		explicit constexpr 
		BinaryString(std::basic_string<C,T,A> const& str)
          : BinaryString(str.begin(), str.end())
        {
		}
		
		/**
		 * @brief	Overwrite with elements copied from a null-terminated C-string 
		 * @details	If @p r is @c nullptr then a null @c BSTR is created
		 * 
		 * @param r		Source buffer
		 * 
		 * @throws std::bad_alloc		Out of memory
		*/
		type constexpr& 
		operator=(gsl::cwzstring const str)
		{
			type{str}.swap(*this);
			return *this;
		}
		
		//! @brief	De-allocate string buffer
		constexpr 
		~BinaryString() noexcept
		{
			if (this->initialized())
			{
				::SysFreeString(this->Buffer);
				this->Buffer = nullptr;
			}
		}	
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Copy & Move Semantics o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		/**
		 * @brief	Construct with elements copied from another binary string
		 * 
		 * @param r		Other binary string
		 * 
		 * @throws std::bad_alloc		Out of memory
		*/
		BinaryString(type const& r)
          : BinaryString{r.begin(), r.end()}
		{
		}

		/**
		 * @brief	Construct with buffer moved from another string
		 * 
		 * @param r		Other string
		*/
		constexpr 
		BinaryString(type&& r) noexcept
		{
			r.swap(*this);
		}
		
		/**
		 * @brief	Overwrite with elements copied from another binary string
		 * 
		 * @param r		Other binary string
		 * 
		 * @throws std::bad_alloc		Out of memory
		*/
		type constexpr&
		operator=(type const& r)
		{
			type{r}.swap(*this);
			return *this;
		}
		
		/**
		 * @brief	Overwrite with buffer moved from another binary string
		 * 
		 * @param r		Other binary string
		*/
		type constexpr& 
		operator=(type&& r) noexcept
		{
			r.swap(*this);
			return *this;
		}
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		/**
		 * @brief	Retrieve reference to back character
		 * 
		 * @throws  std::logic_error  string is empty
		*/
		template <typename Self>
		if_const_then_t<Self, const_reference, reference> constexpr
		back(this Self&& self)
		{
			if (self.empty())
				throw logic_error{"String is empty"};
			return self.Buffer[self.size() - 1];
		}

		/**
		 * @brief	Retrieve iterator positioned at first character
		 * 
		 * @return	Equals @c end() when string is empty
		*/
		template <typename Self>
		if_const_then_t<Self, const_iterator, iterator> constexpr
		begin(this Self&& self) noexcept
        {
			return self.initialized() ? &self.Buffer[0] : nullptr;
		}

		/**
		 * @brief	Retrieve pointer to internal character buffer
		*/
		template <typename Self>
		if_const_then_t<Self, gsl::cwzstring, gsl::wzstring> constexpr
		data(this Self&& self) noexcept
		{
			return self.initialized() ? &self.Buffer[0] : nullptr;
		}

		/**
		 * @brief	Retrieve iterator positioned immediately beyond last character
		 * 
		 * @return	Equals @c begin() when string is empty
		*/
		template <typename Self>
		if_const_then_t<Self, const_iterator, iterator> constexpr
		end(this Self&& self) noexcept
        {
			return self.initialized() ? &self.Buffer[self.size()] : nullptr;
		}

		/**
		 * @brief	Retrieve reference to front character
		 * 
		 * @throws  std::logic_error  string is empty
		*/
		template <typename Self>
		if_const_then_t<Self, const_reference, reference> constexpr
		front(this Self&& self)
		{
			if (self.empty())
				throw logic_error{"String is empty"};
			return self.Buffer[0];
		}

	protected:
		/**
		 * @brief	Measure string, in characters
		 * 
		 * @param	str		[optional] Pointer to first element
		*/
		size_type constexpr
		static measure(gsl::cwzstring str) noexcept {
			return static_cast<size_type>(ZString<wchar_t>::measure(str));
		}
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		/**
		 * @brief	Retrieve const-pointer to null-terminated string buffer, if any
		*/
		gsl::cwzstring constexpr
		c_str() const noexcept
		{
			return this->initialized() ? &this->Buffer[0] : nullptr;
		}
		
		/**
		 * @brief	Query whether string contains a character/substring
		*/
		template <nstd::AnyCvOf<wchar_t, gsl::cwzstring, std::wstring_view> Parameter>
		bool
		contains(Parameter right) const noexcept {
			return this->wsv().contains(right);
		}

		/**
		 * @brief	Query whether string ends with a character/substring
		*/
		template <nstd::AnyCvOf<wchar_t, gsl::cwzstring, std::wstring_view> Parameter>
		bool
		ends_with(Parameter right) const noexcept {
			return this->wsv().ends_with(right);
		}

		/**
		 * @brief	Query whether string is either a @e null @c BSTR or it's empty
		*/
		bool constexpr
		empty() const noexcept
        {
			return !this->Buffer || this->Buffer[0] == BinaryString::null;
		}

		/**
		 * @brief	Custom allocators are not supported
		*/
		allocator_type constexpr
		get_allocator() const noexcept = delete;
		
		/**
		 * @brief	Query current length, in characters
		*/
		size_type constexpr
		size() const noexcept
        {
			return this->Buffer ? ::SysStringLen(this->Buffer) : 0u;
		}

		/**
		 * @brief	Query whether string starts with a character/substring
		*/
		template <nstd::AnyCvOf<wchar_t, gsl::cwzstring, std::wstring_view> Parameter>
		bool
		starts_with(Parameter right) const noexcept {
			return this->wsv().starts_with(right);
		}
		
		/**
		 * @brief	Convert to wide-character string-view
		*/
		implicit constexpr 
		operator std::wstring_view() const noexcept
		{
			return this->wsv();
		}
		
		bool constexpr
		operator==(type const& rhs) const noexcept {
			return (std::wstring_view)*this == (std::wstring_view)rhs;
		}

		bool constexpr
		operator==(gsl::cwzstring rhs) const noexcept {
			return (std::wstring_view)*this == rhs;
		}

		bool constexpr
		inline friend operator==(gsl::cwzstring lhs, type const& rhs) noexcept {
			return lhs == (std::wstring_view)rhs;
		}
		
		bool constexpr
		operator==(std::wstring_view rhs) const noexcept {
			return (std::wstring_view)*this == rhs;
		}

		bool constexpr
		inline friend operator==(std::wstring_view lhs, type const& rhs) noexcept {
			return lhs == (std::wstring_view)rhs;
		}

		// Disable accidental comparisons between narrow-char and @c core::com::wstring
		bool constexpr friend operator==(gsl::czstring,    type const&) noexcept = delete;
		bool constexpr friend operator==(std::string_view, type const&) noexcept = delete;
		bool constexpr friend operator==(std::string,      type const&) noexcept = delete;
		bool constexpr operator==(gsl::czstring) const noexcept = delete;
		bool constexpr operator==(std::string_view) const noexcept = delete;
		bool constexpr operator==(std::string) const noexcept = delete;

	protected:
		/**
		 * @brief	Query whether initialized
		 * 
		 * @remarks	String is uninitialized when default-constructed, detached, or moved-from
		*/
		bool constexpr
		initialized() const noexcept
        {
			return this->Buffer != nullptr;
		}
		
		/**
		 * @brief	Retrieve string-view of buffer
		*/
		std::wstring_view constexpr 
		wsv() const noexcept
		{
			return {this->Buffer, this->Buffer + this->size()};
		}
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		/**
		 * @brief	Overwrite with elements copied from the range [first, end]
		 * @details	If both @p pos and @p end are @c nullptr then a null @c BSTR is created
		 * 
		 * @param pos		First element in range
		 * @param end		One past last element in range
		 * 
		 * @throws  std::invalid_argument   Missing either argument (but not both)
		 * @throws  std::invalid_argument   @p pos and @p end specify an invalid range
		*/
		template <std::forward_iterator BeginIterator, std::forward_iterator EndIterator>
			requires nstd::IndirectlyConvertibleTo<BeginIterator, wchar_t>
			      && nstd::IndirectlyConvertibleTo<EndIterator, wchar_t>
		void 
		assign(BeginIterator pos, EndIterator end)
        {
			if constexpr (std::is_pointer_v<BeginIterator> && std::is_pointer_v<EndIterator>) {
				if (!pos != !end) {
					ThrowIfNull(pos);
					ThrowIfNull(end);
				}
				else if (!pos && !end) {
					if (this->Buffer)
						::SysFreeString(std::exchange(this->Buffer, nullptr));
					return;
				}
			}
			ThrowIf(end, !detail::is_valid_range(pos, end));

			// Allocate an empty buffer with space for additional null terminator
			auto const nChars = std::distance(pos, end);
			if (wchar_t* newBuffer = ::SysAllocStringByteLen(nullptr, (UINT)nstd::sizeof_n<wchar_t>(nChars)); !newBuffer)
				throw std::bad_alloc{};
			else
			{
				if (this->Buffer)
					::SysFreeString(this->Buffer);
					
				this->Buffer = newBuffer;
				std::copy(pos, end, this->Buffer);
				this->Buffer[nChars] = null;
			}
		}
		
		/**
		 * @brief	Retrieve pointer to internal character buffer
		*/
		gsl::wzstring constexpr
		bstr() noexcept
		{
			return this->Buffer;
		}

		/**
		 * @brief	Detach and return the internal character buffer, leaving this object uninitialized
		 * 
		 * @return	nullptr when uninitialized
		*/
		gsl::wzstring constexpr
		detach() noexcept
		{
			return std::exchange(this->Buffer, nullptr);
		}

		/**
		 * @brief	Swap with another string
		 * 
		 * @param r	Other string
		*/
		void constexpr
		swap(type& r) noexcept
        {
			std::swap(this->Buffer, r.Buffer);
		}
	};
	
	//! @brief	Wide-character BSTR allocated on the COM heap
	using wstring = BinaryString;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com
{
	//! @brief  Query whether @c BinaryString is @e empty
	bool
	empty(BinaryString const& str) {
		return str.empty();
	}
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-~o Test Code o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com::testing
{
	//! @test  Verify @c core::com::wstring is default-constructible at compile-time
	static_assert(wstring{} == std::wstring_view{L""});
	
	//! @test  Verify @c core::com::wstring converts to @c std::wstring_view
	static_assert(std::convertible_to<BinaryString, std::wstring_view>);
	
	//! @test  Verify @c core::com::wstring can be constructed from @c std::string
	static_assert(std::constructible_from<BinaryString, std::string>);

	//! @test  Verify @c core::com::wstring can be constructed from @c std::wstring
	static_assert(std::constructible_from<BinaryString, std::wstring>);
	
	//! @test  Verify @c core::com::wstring models @c meta::EmptyCompatible
	static_assert(meta::EmptyCompatible<wstring>);
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o