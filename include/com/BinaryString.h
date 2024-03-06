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
		template <std::forward_iterator ForwardIterator>
		bool constexpr
		is_valid_range(ForwardIterator first, ForwardIterator end)
        {
			if constexpr (std::is_pointer_v<ForwardIterator>) {
				return first && end && first <= end;
			}
			else if constexpr (std::random_access_iterator<ForwardIterator>) {
				return first <= end;
			}
            else {
				return true;
			}
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
		 * @brief	Construct around @c nullptr (ie. uninitialized)
		*/
		constexpr 
		BinaryString() noexcept
		{}
		
		/**
		 * @brief	Construct with elements copied from input range
		 * 
		 * @param first		First element in range
		 * @param end		Immediately beyond last element in range
		 * 
		 * @throws std::bad_alloc		Out of memory
		*/
		template <std::forward_iterator ForwardIterator>
			requires nstd::IndirectlyConvertibleTo<ForwardIterator, wchar_t>
		BinaryString(ForwardIterator first, ForwardIterator end)
        {
			this->assign(first,end);
		}
		
		//! @brief	Prevent construction from null pointer literals
		constexpr 
		BinaryString(nullptr_t, nullptr_t) noexcept = delete;

		/**
		 * @brief	Construct with elements copied from a null-terminated C-string
		 * 
		 * @param str		Null-terminated wide-character array
		 * 
		 * @throws std::bad_alloc		Out of memory
		*/
		explicit 
		BinaryString(gsl::cwzstring const str) 
		  : BinaryString{str, str + BinaryString::measure(str)}
        {
		}
		
		/**
		 * @brief	Take ownership of elements within a null-terminated buffer
		 * 
		 * @param str		Null-terminated wide-character array
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
		 * @brief	Construct with elements copied from a std::basic_string
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
		 * @brief	Construct with elements copied from another string
		 * 
		 * @param r		Other string
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
		 * @brief	Overwrite with elements copied from a null-terminated C-string 
		 * 
		 * @param r		Other string
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
		 * @brief	Overwrite with elements copied from another string 
		 * 
		 * @param r		Other string
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
		 * @brief	Overwrite with buffer moved from another string 
		 * 
		 * @param r		Other string
		 * 
		 * @remarks	If @p r has a different allocator to @p this, a copy is performed
		*/
		type constexpr& 
		operator=(type&& r) noexcept
		{
			r.swap(*this);
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

		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Static Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		/**
		 * @brief	Retrieve reference to back character
		 * 
		 * @remarks	Undefined behaviour when the string is empty
		*/
		template <typename Self>
		if_const_then_t<Self, const_reference, reference> constexpr
		back(this Self&& self) noexcept
		{
			return self.Buffer[self.size()-1];
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
		 * @remarks	Undefined behaviour when the string is empty
		*/
		template <typename Self>
		if_const_then_t<Self, const_reference, reference> constexpr
		front(this Self&& self) noexcept
		{
			return self.Buffer[0];
		}

	protected:
		/**
		 * @brief	Measure string, in characters
		 * 
		 * @param	str		Pointer to first element, possibly @c nullptr
		*/
		size_type constexpr
		static measure(gsl::cwzstring str) noexcept {
			return static_cast<size_type>(ZString<wchar_t>::measure(str));
		}
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		/**
		 * @brief	Retrieve pointer to internal character buffer
		*/
		gsl::cwzstring constexpr
		c_str() const noexcept
		{
			return this->initialized() ? &this->Buffer[0] : L"";
		}

		/**
		 * @brief	Query whether string is empty
		 * 
		 * @return	true iff empty
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
		 * @brief	Convert to wide-character string-view
		*/
		implicit constexpr 
		operator std::wstring_view() const noexcept
		{
			return {this->Buffer, this->Buffer + this->size()};
		}
		
		bool constexpr
		operator==(gsl::cwzstring rhs) noexcept {
			return (std::wstring_view)*this == rhs;
		}

		bool constexpr
		friend operator==(gsl::cwzstring lhs, type const& rhs) noexcept {
			return lhs == (std::wstring_view)rhs;
		}
		
		bool constexpr
		operator==(std::wstring_view rhs) noexcept {
			return (std::wstring_view)*this == rhs;
		}

		bool constexpr
		friend operator==(std::wstring_view lhs, type const& rhs) noexcept {
			return lhs == (std::wstring_view)rhs;
		}

		// Disable accidental comparisons between narrow-char and @c core::com::wstring
		bool constexpr friend operator==(gsl::czstring,    type const&) noexcept = delete;
		bool constexpr friend operator==(std::string_view, type const&) noexcept = delete;
		bool constexpr friend operator==(std::string,      type const&) noexcept = delete;
		bool constexpr operator==(gsl::czstring) noexcept = delete;
		bool constexpr operator==(std::string_view) noexcept = delete;
		bool constexpr operator==(std::string) noexcept = delete;

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
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Mutator Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		/**
		 * @brief	Overwrite with elements copied from the range [first, end]
		 * 
		 * @param pos		First element in range
		 * @param end		One past last element in range
		*/
		template <std::forward_iterator ForwardIterator>
			requires nstd::IndirectlyConvertibleTo<ForwardIterator, wchar_t>
		void 
		assign(ForwardIterator pos, ForwardIterator end)
        {
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
		gsl::wzstring
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

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-~o Test Code o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com::testing
{
	//! @test  Verify @c core::com::wstring is default-constructible at compile-time
	static_assert(wstring{} == std::wstring_view{L""});
	
	//! @test  Verify @c core::com::wstring converts to @c std::wstring_view
	static_assert(std::convertible_to<BinaryString, std::wstring_view>);
}
// o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Separator o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o