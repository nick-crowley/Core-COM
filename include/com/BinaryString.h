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
		struct BStr
		{
			uint32_t  Length;
			wchar_t*  Buffer;
		};

		template <std::forward_iterator ForwardIterator>
		bool constexpr
		static is_valid_range(ForwardIterator first, ForwardIterator end)
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
	 * @brief	Character-based sequence container convertible to COM 'binary-string' (ie. BSTR)
	 * 
	 * @tparam	Allocator	Any allocator type
	*/
	template <typename Allocator = allocator<wchar_t>>
	class BinaryString : private detail::BStr
	{
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Types & Constants o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		using type = BinaryString<Allocator>;
		using base = detail::BStr;

		using character_t = wchar_t;
		using char_allocator_t = typename Allocator::template rebind<character_t>::other;
		
    public:
		using allocator_type = Allocator;
		using iterator = character_t*;
		using const_iterator = character_t const*;
		using size_type = uint32_t;
		using value_type = character_t;
		using difference_type = ptrdiff_t;
		using reference = character_t&;
		using const_reference = character_t const&;
		
	private:
		character_t constexpr
		inline static null = L'\0';
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=o Representation o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	private:
		char_allocator_t Alloc;
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Construction & Destruction o=~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		/**
		 * @brief	Construct uninitialized
		*/
		constexpr 
		BinaryString() noexcept : base{}
		{}
		
		/**
		 * @brief	Construct uninitialized
		 * 
		 * @param alloc		Allocator used to allocate character buffer
		*/
		explicit constexpr
		BinaryString(Allocator alloc) noexcept
		  : base{}, Alloc{alloc}
		{
		}

		/**
		 * @brief	Construct with elements copied from input range
		 * 
		 * @tparam	ForwardIterator		Referant type must be convertible to @p character_t
		 * 
		 * @param first		First element in range
		 * @param end		Immediately beyond last element in range
		 * @param alloc		[optional] Allocator used to allocate character buffer
		*/
		template <std::forward_iterator ForwardIterator>
		BinaryString(ForwardIterator first, ForwardIterator end, Allocator alloc = Allocator{}) noexcept
          : base{}, Alloc{alloc}
        {	
			static_assert(std::is_convertible_v<decltype(*first),character_t>);
			this->assign(first,end);
		}
		
		//! @brief	Prevent construction from null pointers
		constexpr 
		BinaryString(nullptr_t, nullptr_t, Allocator = Allocator{}) noexcept = delete;

		/**
		 * @brief	Construct with elements copied from a null-terminated C-string
		 * 
		 * @param str		Null-terminated wide-character array
		 * @param alloc		[optional] Allocator used to allocate character buffer
		*/
		explicit 
		BinaryString(gsl::cwzstring const str, Allocator alloc = Allocator{}) noexcept
		  : BinaryString{str, str+type::measure(str), alloc}
        {
		}
		
		/**
		 * @brief	Take ownership of elements within a null-terminated buffer
		 * 
		 * @param str		Null-terminated wide-character array
		 * @param alloc		[optional] Allocator used to allocate character buffer
		*/
		constexpr explicit 
		BinaryString(meta::adopt_t, gsl::wzstring const str, Allocator alloc = Allocator{}) noexcept
		  : base{static_cast<uint32_t>(type::measure(str)), str}, Alloc{alloc}
        {
		}
		
#if SUPPORT_ATL_STRING
		/**
		 * @brief	Construct with elements copied from an ATL string
		 *
		 * @param str		ATL string
		 * @param alloc		[optional] Allocator used to allocate character buffer
		*/
		constexpr 
		explicit BinaryString(ATL::CSimpleStringT<wchar_t> const& str, Allocator alloc = Allocator{}) noexcept
			: BinaryString(str.GetString(), str.GetString() + str.GetLength(), alloc)
		{
		}

		/**
		 * @brief	Prevent moving from ATL string
		*/
		constexpr 
		BinaryString(ATL::CSimpleStringT<wchar_t>&&, Allocator = Allocator{}) noexcept = delete;
#endif // 0

		/**
		 * @brief	Construct with elements copied from a std::basic_string
		 * 
		 * @param str		Standard library string
		 * @param alloc		[optional] Allocator used to allocate character buffer
		*/
		template <typename C, typename T, typename A>
		constexpr explicit 
		BinaryString(std::basic_string<C,T,A> const& str, Allocator alloc = Allocator{}) noexcept
          : BinaryString(str.begin(), str.end(), alloc)
        {
		}
		
		/**
		 * @brief	Prevent moving from std::basic_string
		*/
		template <typename C, typename T, typename A>
		constexpr 
		BinaryString(std::basic_string<C,T,A>&&, Allocator = Allocator{}) noexcept = delete;
				
		/**
		 * @brief	Construct with elements copied from another string
		 * 
		 * @param r		Other string
		*/
		constexpr 
		BinaryString(type const& r) noexcept
          : BinaryString{r.begin(), r.end(), r.get_allocator()}
		{
		}

		/**
		 * @brief	Construct with elements copied from another string into memory allocated using @p alloc
		 * 
		 * @param r			Other string
		 * @param alloc		Allocator used to allocate character buffer
		*/
		constexpr 
		BinaryString(type const& r, Allocator alloc) noexcept
		  : BinaryString{r.begin(), r.end(), alloc}
		{
		}
			
		/**
		 * @brief	Construct with elements copied from a string using a different allocator
		 * 
		 * @param r		Other string
		*/
		template <typename Other>
		constexpr 
		BinaryString(BinaryString<Other> const& r) noexcept
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
		  : base{}, Alloc{r.get_allocator()}
		{
			r.swap(*this);
		}
		
		/**
		 * @brief	Best effort to construct with buffer moved from another string (otherwise copied)
		 * 
		 * @param r			Other string
		 * @param alloc		Allocator used to allocate character buffer
		 * 
		 * @remarks	@p r is copied if elements of @p r were not allocated using @p alloc
		*/
		constexpr 
		BinaryString(type&& r, Allocator alloc) noexcept
		  : base{}, Alloc{alloc}
		{
			if (r.get_allocator() == alloc)
			{
				r.swap(*this);
				std::swap(this->Alloc, r.Alloc);
			}
			else
			{
				type{r.begin(), r.end(), alloc}.swap(*this);
			}
		}
		
		/**
		 * @brief	Overwrite with elements copied from a null-terminated C-string 
		 * 
		 * @param r		Other string
		*/
		type constexpr& 
		operator=(gsl::cwzstring const str) noexcept
		{
			type{str}.swap(*this);
			return *this;
		}
		
		/**
		 * @brief	Overwrite with elements copied from another string 
		 * 
		 * @param r		Other string
		*/
		type constexpr&
		operator=(type const& r) noexcept
		{
			type{r}.swap(*this);
			return *this;
		}
		
		/**
		 * @brief	Overwrite with elements copied from a string using a different allocator
		 * 
		 * @param r		Other string
		*/
		template <typename Other>
		type constexpr&
		operator=(BinaryString<Other> const& r) noexcept
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
			if constexpr (Allocator::propagate_on_container_move_assignment())
            {
				r.swap(*this);
			}
			else if (r.get_allocator() == this->get_allocator())
            {
				r.swap(*this);
				std::swap(this->Alloc, r.Alloc);
			}
			else 
			{
				type{r.begin(), r.end(), this->Alloc}.swap(*this);
				std::swap(this->Alloc, r.Alloc);
			} 
			return *this;
		}
		
		//! @brief	De-allocate string buffer
		constexpr 
		~BinaryString() noexcept
		{
			if (this->initialized())
            {
				this->Alloc.deallocate(this->Buffer);
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
		auto constexpr&
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
		auto constexpr
		begin(this Self&& self) noexcept
        {
			return self.initialized() ? &self.Buffer[0] : &type::null;
		}

		/**
		 * @brief	Retrieve pointer to internal character buffer
		*/
		template <typename Self>
		auto constexpr*
		c_str(this Self&& self) noexcept
		{
			return self.initialized() ? &self.Buffer[0] : &type::null;
		}

		/**
		 * @brief	Retrieve pointer to internal character buffer
		*/
		template <typename Self>
		auto constexpr*
		data(this Self&& self) noexcept
		{
			return self.initialized() ? &self.Buffer[0] : &type::null;
		}

		/**
		 * @brief	Retrieve iterator positioned immediately beyond last character
		 * 
		 * @return	Equals @c begin() when string is empty
		*/
		template <typename Self>
		auto constexpr
		end(this Self&& self) noexcept
        {
			return self.initialized() ? &self.Buffer[self.size()] : &type::null;
		}

		/**
		 * @brief	Retrieve reference to front character
		 * 
		 * @remarks	Undefined behaviour when the string is empty
		*/
		template <typename Self>
		auto constexpr&
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
		static measure(gsl::cwzstring str) {
			return static_cast<size_type>(ZString<character_t>::measure(str));
		}
		
		// o~=~-~=~-~=~-~=~-~=~-~=~-~=~o Observer Methods & Operators o~-~=~-~=~-~=~-~=~-~=~-~=~-~o
	public:
		/**
		 * @brief	Query whether string is empty
		 * 
		 * @return	true iff empty
		*/
		bool constexpr
		empty() const noexcept
        {
			return this->Length == 0;
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
			return this->Length;
		}
		
		/**
		 * @brief	Construct string view of buffer
		*/
		implicit constexpr 
		operator std::wstring_view() const noexcept
		{
			return {this->Buffer, this->Buffer+this->Length};
		}
		
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
		 * @tparam	ForwardIterator		Iterator whose referent is convertible to @c wchar_t
		 * 
		 * @param first		First element in range
		 * @param end		One past last element in range
		*/
		template <std::forward_iterator ForwardIterator>
		void 
		assign(ForwardIterator first, ForwardIterator end) noexcept
        {	
			static_assert(std::is_convertible_v<decltype(*first),character_t>);

			if (difference_type const nChars = std::distance(first,end); 
				nChars < static_cast<size_type>(-1) && detail::is_valid_range(first,end))
			{	
				if (auto* newBuffer = this->Alloc.allocate(nstd::sizeof_n<wchar_t>(nChars+1)); newBuffer)
                {
					if (this->Buffer) 
						this->Alloc.deallocate(this->Buffer);
					
					this->Buffer = newBuffer;
					this->Length = static_cast<size_type>(nChars);
					std::copy(first, end, this->Buffer);
					this->Buffer[nChars] = null;
				}
			}
		}

		/**
		 * @brief	Detach and return the internal character buffer, leaving this object uninitialized
		 * 
		 * @return	nullptr when uninitialized
		*/
		character_t constexpr* 
		detach() noexcept
		{
			this->Length = 0;
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
			std::swap(this->Alloc, r.Alloc);
			std::swap(this->Buffer, r.Buffer);
			std::swap(this->Length, r.Length);
		}
	};
	
	template <uint32_t N>
	explicit BinaryString(wchar_t const (&&)[N]) -> BinaryString<NoopAllocator<wchar_t>>;
	
	//! @brief	Wide-character BSTR allocated on the COM heap
	using wstring = BinaryString<allocator<wchar_t>>;
	
	extern template wstring;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
bool constexpr
operator==(gsl::cwzstring lhs, core::com::wstring const& rhs) noexcept {
	return lhs == (std::wstring_view)rhs;
}

bool constexpr
operator==(core::com::wstring const& lhs, gsl::cwzstring rhs) noexcept {
	return (std::wstring_view)lhs == rhs;
}

bool constexpr operator==(gsl::czstring, core::com::wstring const&) noexcept = delete;
bool constexpr operator==(core::com::wstring const&, gsl::czstring) noexcept = delete;

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com 
{
	inline namespace literals
	{
		inline namespace string_literals
		{
			/**
			 * @brief	Construct @p com::BinaryString from string literal
			 * 
			 * @param ws	String literal
			*/
			auto consteval
			operator""_bstr(gsl::cwzstring ws, size_t) noexcept
			{
				return BinaryString<NoopAllocator<wchar_t>>{adopt, const_cast<gsl::wzstring>(ws)};
			}
		}
	}
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-~o Test Code o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com::testing
{
	using namespace com::string_literals;

	//! @test  Verify @c core::com::wstring is default-constructible at compile-time
	static_assert(wstring{} == std::wstring_view{L""});
	
	//! @test  Verify @c core::com::wstring converts to @c std::wstring_view
	static_assert(std::convertible_to<decltype(L"abc"_bstr),std::wstring_view>);

	//! @test  Verify @c core::com::wstring can be compared against @c std::wstring_view
	//static_assert(L"abc"_bstr == L"abc"sv);

	//! @test  Verify @c core::com::wstring::font() returns the first character
	static_assert(L"abc"_bstr.front() == 'a');

	//! @test  Verify @c core::com::wstring::back() returns the last character
	static_assert(L"abc"_bstr.back() == 'c');

	//! @test  Verify @c core::com::wstring::size() returns the size in characters
	static_assert(L"abc"_bstr.size() == 3);

	//! @test  Verify @c core::com::wstring::empty() returns false when non-empty
	static_assert(!L"abc"_bstr.empty());
	
	//! @test  Verify @c core::com::wstring::c_str() returns valid character-pointer
	static_assert(L"abc"_bstr.c_str() == L"abc"sv);
}
// o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Separator o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o