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
#include "com/BStrAllocator.h"
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
	 * @brief	Wide-character basic string supporting attach/detach operations and different allocators
	 * 
	 * @tparam  Allocator  [optional] Custom allocator
	*/
	template <typename Allocator = HeapAllocator<wchar_t>>
	class basic_string
	{
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		using type = basic_string<Allocator>;
		using base = void;
		
		using allocator_t = typename Allocator::template rebind<wchar_t>::other;

		template <typename Self, typename T, typename F>
		using if_const_then_t = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>, T, F>; 
		
    public:
		using allocator_type = allocator_t;
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
		allocator_type Alloc;
		gsl::wzstring  Buffer = nullptr;
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		/**
		 * @brief	Construct @e null string
		*/
		constexpr 
		basic_string() noexcept = default;
		
		//! @brief	Unnecessary; use the default-constructor to create @e null string
		constexpr 
		basic_string(nullptr_t) noexcept = delete;
		
		/**
		 * @brief	Construct with elements copied from a (possibly empty) input range
		 * @details	If both @p pos and @p end specify an empty range then a @e null string is created
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
		constexpr
		basic_string(BeginIterator pos, EndIterator end)
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
		 * @details	If @p src is @c nullptr then a @e null string is created
		 * 
		 * @param str	[optional] Null-terminated source buffer
		 * 
		 * @throws std::bad_alloc      Out of memory
		*/
		constexpr explicit
		basic_string(gsl::cwzstring str)
        {
			if (str)
				this->assign(str, str + basic_string::measure(str));
		}
		
		/**
		 * @brief	Take ownership of elements within a null-terminated buffer
		 * @details	If @p src is @c nullptr then a @e null string is created
		 * 
		 * @param str	[optional] Null-terminated source buffer
		*/
		constexpr
		basic_string(meta::adopt_t, gsl::wzstring str) noexcept
		  : Buffer{str}
        {
		}
		
		/**
		 * @brief	Take ownership of elements within a null-terminated buffer
		 * @details	If @p src is @c nullptr then a @e null string is created
		 * 
		 * @param str	[optional] Null-terminated source buffer
		*/
		constexpr
		basic_string(gsl::wzstring str, meta::adopt_t) noexcept
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
		explicit basic_string(ATL::CSimpleStringT<wchar_t> const& str) 
			: basic_string(str.GetString(), str.GetString() + str.GetLength())
		{
		}

		/**
		 * @brief	Prevent moving elements from ATL string
		*/
		constexpr 
		basic_string(ATL::CSimpleStringT<wchar_t>&&) noexcept = delete;
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
		basic_string(std::basic_string<C,T,A> const& str)
          : basic_string(str.begin(), str.end())
        {
		}
		
		//! @brief	De-allocate string buffer
		constexpr 
		~basic_string() noexcept
		{
			if (this->initialized())
			{
				this->reset();
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
		constexpr
		basic_string(type const& r)
          : basic_string{r.begin(), r.end()}
		{
		}
		
		/**
		 * @brief	Construct with elements copied from a binary string with a different allocator
		 * 
		 * @param r		Other binary string
		 * 
		 * @throws std::bad_alloc		Out of memory
		*/
		template <typename Other>
		constexpr explicit
		basic_string(basic_string<Other> const& r)
          : basic_string{r.begin(), r.end()}
		{
		}

		/**
		 * @brief	Construct with buffer moved from another string
		 * 
		 * @param r		Other string
		*/
		constexpr 
		basic_string(type&& r) noexcept
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
		 * @brief	Overwrite with elements copied from a binary string with a different allocator
		 * 
		 * @param r		Other binary string
		 * 
		 * @throws std::bad_alloc		Out of memory
		*/
		template <typename Other>
		type constexpr&
		operator=(basic_string<Other> const& r)
		{
			this->assign(r.begin(), r.end());
			return *this;
		}
		
		/**
		 * @brief	Overwrite with elements copied from a null-terminated C-string 
		 * @details	If @p r is @c nullptr then a @e null string is created
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
		
		/**
		 * @brief	Overwrite with buffer moved from another binary string
		 * 
		 * @param r		Other binary string
		*/
		type constexpr& 
		operator=(type&& r) noexcept
		{
			r.swap(*this);
			r.reset();
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
		bool constexpr
		contains(Parameter right) const noexcept {
			return this->wsv().contains(right);
		}

		/**
		 * @brief	Query whether string ends with a character/substring
		*/
		template <nstd::AnyCvOf<wchar_t, gsl::cwzstring, std::wstring_view> Parameter>
		bool constexpr
		ends_with(Parameter right) const noexcept {
			return this->wsv().ends_with(right);
		}

		/**
		 * @brief	Query whether string is @e null or @e empty
		*/
		bool constexpr
		empty() const noexcept
        {
			return !this->Buffer || this->Buffer[0] == basic_string::null;
		}
		
		/**
		 * @brief	Retrieve allocator
		*/
		allocator_type constexpr
		get_allocator() const noexcept
		{
			return this->Alloc;
		}
		
		/**
		 * @brief	Query current length, in characters
		*/
		size_type constexpr
		size() const noexcept
        {
			if (!this->Buffer)
				return 0u;

			// Since @c BSTR can contain embedded nulls we mustn't rely on @c wcslen()
			if constexpr (std::is_same_v<allocator_type, BStrAllocator<wchar_t>>)
				return ::SysStringLen(this->Buffer);
			else
				return type::measure(this->Buffer);  // std::wcslen() isn't constexpr
		}

		/**
		 * @brief	Query whether string starts with a character/substring
		*/
		template <nstd::AnyCvOf<wchar_t, gsl::cwzstring, std::wstring_view> Parameter>
		bool constexpr
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
		
		/**
		 * @brief	Convert to wide-character string
		*/
		implicit constexpr 
		operator std::wstring() const noexcept
		{
			return std::wstring{this->wsv()};
		}
		
		/**
		 * @brief	Compare against another instance
		*/
		bool constexpr
		operator==(type const& rhs) const noexcept {
			return this->wsv() == rhs.wsv();
		}
		
		/**
		 * @brief	Compare against string with different allocator
		*/
		template <typename Other>
		bool constexpr
		operator==(basic_string<Other> const& rhs) const noexcept {
			return this->wsv() == rhs.wsv();
		}
		
		/**
		 * @brief	Compare against raw string
		*/
		bool constexpr
		operator==(gsl::cwzstring rhs) const noexcept {
			return this->wsv() == rhs;
		}
		
		/**
		 * @brief	Compare against string view
		*/
		bool constexpr
		operator==(std::wstring_view rhs) const noexcept {
			return this->wsv() == rhs;
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
		 * @brief	Query whether a non-null string
		 * 
		 * @remarks	String is uninitialized when default-constructed, detached, reset, or moved-from
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
		 * @details	If @p pos and @p end specify an empty range then a null string is created
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
		void constexpr
		assign(BeginIterator pos, EndIterator end)
        {
			if constexpr (std::is_pointer_v<BeginIterator> && std::is_pointer_v<EndIterator>) {
				if (!pos != !end) {
					ThrowIfNull(pos);
					ThrowIfNull(end);
				}
				// [DOUBLE-NULL] Create @e null string rather than @e empty string
				if (!pos && !end) {
					this->reset();
					return;
				}
			}
			ThrowIf(end, !detail::is_valid_range(pos, end));

			// Allocate an empty buffer with space for additional null terminator
			auto const nChars = std::distance(pos, end);
			wchar_t* newBuffer = this->Alloc.allocate(nstd::sizeof_n<wchar_t>(nChars + 1));
			if (!newBuffer)
				throw std::bad_alloc{};

			// Replace existing buffer, if any
			if (this->Buffer)
				this->Alloc.deallocate(std::exchange(this->Buffer, newBuffer));
			else
				this->Buffer = newBuffer;
			
			// Populate and null-terminate
			std::copy(pos, end, this->Buffer);
			this->Buffer[nChars] = null;
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
		 * @brief	Detach and return the internal character buffer, leaving a null string
		 * 
		 * @return	nullptr when uninitialized
		*/
		gsl::wzstring constexpr
		detach() noexcept
		{
			return std::exchange(this->Buffer, nullptr);
		}
		
		/**
		 * @brief	Release the internal character buffer, leaving a null string
		*/
		void constexpr
		reset() noexcept 
		{	
			if (this->Buffer)
				this->Alloc.deallocate(std::exchange(this->Buffer, nullptr));
		}

		/**
		 * @brief	Release existing string, if any, and replace with copy of @p wstr
		*/
		void constexpr
		reset(std::wstring_view wstr) noexcept 
		{	
			this->reset();
			this->assign(wstr);
		}
		
		/**
		 * @brief	Release existing string, if any, and adopt @p wstr instead
		*/
		void constexpr
		reset(gsl::wzstring str, meta::adopt_t) noexcept 
		{	
			this->reset();
			this->Buffer = str;
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
	
	//! @brief	Binary string (ie. @c BSTR) allocated using the @c SysAllocString() function
	using bstr = basic_string<BStrAllocator<wchar_t>>;
	
	//! @brief	Wide-character string-literal which is never deallocated
	using noopstring = basic_string<NoopAllocator<wchar_t>>;

	//! @brief	Wide-character string allocated on the COM heap using the @c CoTaskMemAlloc() function
	using wstring = basic_string<HeapAllocator<wchar_t>>;
}
namespace std 
{
	template <typename Alloc>
	struct formatter<core::com::basic_string<Alloc>, wchar_t> 
    {
		auto constexpr
		parse(basic_format_parse_context<wchar_t>& ctx) {
            return ranges::find(ctx, '}');
        }

		auto 
		format(core::com::basic_string<Alloc> const& s, wformat_context& ctx) const {
			return format_to(ctx.out(), L"{}", std::wstring_view(s));
		}
	};
	
	template <typename Alloc, typename T>
	struct _Pointer_of_or_helper<core::com::basic_string<Alloc>, T> {
		using type = T;
	};
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com
{
	//! @brief  Query whether @c basic_string is @e empty
	template <typename Alloc>
	bool
	empty(basic_string<Alloc> const& str) {
		return str.empty();
	}
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com 
{
	inline namespace literals
	{
		inline namespace string_literals
		{
			/**
			 * @brief	Construct @p com::noopstring from string literal
			 * 
			 * @param ws	String literal
			*/
			noopstring constexpr
			operator""_ns(gsl::cwzstring ws, size_t) noexcept
			{
				return {adopt, const_cast<gsl::wzstring>(ws)};
			}
			
			/**
			 * @brief	Construct @p com::wstring at runtime
			*/
			wstring
			inline operator""_s(gsl::cwzstring ws, size_t) noexcept
			{
				return wstring{ws};
			}
		}
	}
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-~o Test Code o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com::testing
{
	//! @test  Verify @c core::com::wstring is default-constructible at compile-time
	static_assert(wstring{} == std::wstring_view{L""});
	
	//! @test  Verify @c core::com::wstring converts to @c std::wstring_view
	static_assert(std::convertible_to<wstring, std::wstring_view>);
	
	//! @test  Verify @c core::com::wstring can be constructed from @c std::string
	static_assert(std::constructible_from<wstring, std::string>);

	//! @test  Verify @c core::com::wstring can be constructed from @c std::wstring
	static_assert(std::constructible_from<wstring, std::wstring>);
	
	//! @test  Verify @c core::com::wstring models @c meta::EmptyCompatible
	static_assert(meta::EmptyCompatible<wstring>);
	
	//! @test  Verify @c core::com::basic_string can be constructed at compile-time
	static_assert(noopstring{}.empty());
	static_assert(noopstring{adopt, (wchar_t*)L"abc"}.data());
	
	//! @test  Verify @c core::com::operator""_ns constructs object at compile-time
	using namespace string_literals;
	using namespace std::string_literals;
	using namespace std::string_view_literals;
	static_assert(L""_ns.empty());
	static_assert(!L"abc"_ns.empty());
	static_assert(L""_ns.size() == 0);
	static_assert(L"abc"_ns.size() == 3);
	static_assert(L"abc"_ns == L"abc"_ns);
	static_assert(L"abc"_ns == L"abc"sv);
	static_assert(L"abc"_ns == L"abc"s);
	static_assert(L"abc"_ns == L"abc");
	static_assert(L"abcde"_ns.contains(L"bcd"));
	static_assert(L"abcde"_ns.starts_with(L"ab"));
	static_assert(L"abcde"_ns.ends_with(L"de"));
	static_assert(L"abc"_ns.front() == L'a');
	static_assert(L"abc"_ns.back() == L'c');
	static_assert(L"abc"_ns.c_str() != nullptr);
	static_assert(L"abc"_ns.data() != nullptr);
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o