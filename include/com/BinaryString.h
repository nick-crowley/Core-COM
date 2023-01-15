#pragma once
#include "library/core.COM.h"
#if ATL_STRING_SUPPORT
#	include <atlstr.h>
#endif
#include "com/HeapAllocator.h"
#include "com/NoopAllocator.h"

namespace core::com 
{
	namespace detail 
	{
		template <typename T, typename... U>
		bool constexpr 
		static is_any_of_v = std::disjunction_v<std::is_same<T,U>...>;
		
		struct com_string
		{
			uint32_t  Length;
			wchar_t*  Buffer;
		};

		template <typename InputIterator>
		bool 
		static is_valid_range(InputIterator first, InputIterator end)
        {
			if constexpr (std::is_pointer_v<InputIterator>) {
				return first && end && first <= end;
			}
			else if constexpr (std::_Is_random_iter_v<InputIterator>) {
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
	class BinaryString;

	inline namespace literals
	{
		inline namespace string_literals
		{
			constexpr BinaryString<NoopAllocator<wchar_t>> 
			operator""_bstr(const wchar_t*, size_t) noexcept;
		}
	}

	template <typename Allocator /*= allocator<wchar_t>*/>
	class BinaryString : private detail::com_string
	{
		using type = BinaryString<Allocator>;
		using base = detail::com_string;

		using character_t = wchar_t;
		using char_alloc_t = typename Allocator::template rebind<wchar_t>::other;
		
		constexpr character_t 
		static inline null = L'\0';

    public:
		using allocator_type = Allocator;
		using iterator = character_t*;
		using const_iterator = character_t const*;
		using size_type = uint32_t;
		using value_type = character_t;
		using difference_type = ptrdiff_t;
		using reference = character_t&;
		using const_reference = character_t const&;
		
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
		constexpr 
		explicit BinaryString(Allocator alloc) noexcept
		  : base{}, m_alloc{alloc}
		{
		}

		/**
		 * @brief	Construct with elements copied from built-in array
		 * 
		 * @tparam InputCharacter	Any type convertible to @p character_t 
		 * 
		 * @param src		Built-in array of elements (convertible to @p InputCharacter)
		 * @param alloc		[optional] Allocator used to allocate character buffer
		*/
		template <typename InputCharacter, size_type N>
		constexpr 
		explicit BinaryString(InputCharacter (&src)[N], Allocator alloc = Allocator{}) noexcept
          : BinaryString(std::begin(src), std::prev(std::end(src)), alloc)
        {
		}
		
		/**
		 * @brief	Construct around string literal buffer
		 * 
		 * @tparam InputCharacter	Any type convertible to @p character_t 
		 * 
		 * @param src		String literal 
		 * 
		 * @remarks	Requires @p Allocator be the no-op allocator
		*/
		template <typename InputCharacter, size_type N>
		constexpr 
		explicit BinaryString(InputCharacter (&&src)[N]) noexcept
		  : base{N-1, N, const_cast<character_t*>(static_cast<character_t const* const>(src))}
        {
			static_assert(std::is_const_v<InputCharacter>,"String literal buffer must be const");

			static_assert(std::is_same_v<Allocator,NoopAllocator<character_t>>, 
				"Constructing from string literal requires that you use NoopAllocator<..>");

			static_assert(std::is_convertible_v<InputCharacter*,character_t const*>,
				"Cannot construct from character literals of supplied type");
		}

		//! @brief	Cannot construct from null pointers
		constexpr 
		BinaryString(nullptr_t, nullptr_t, Allocator = Allocator{}) noexcept = delete;

		/**
		 * @brief	Construct with elements copied from input range
		 * 
		 * @tparam InputIterator	Any type convertible to @p character_t
		 * 
		 * @param first		First element in range
		 * @param end		Immediately beyond last element in range
		 * @param alloc		[optional] Allocator used to allocate character buffer
		*/
		template <typename InputIterator>
		constexpr 
		BinaryString(InputIterator first, InputIterator end, Allocator alloc = Allocator{}) noexcept
          : base{}, m_alloc{alloc}
        {	
			static_assert(std::is_convertible_v<decltype(*first),wchar_t>);
			this->assign(first,end);
		}
		
		/**
		 * @brief	Construct with elements copied from a null-terminated C-string
		 * 
		 * @param str		Null-terminated character array
		 * @param alloc		[optional] Allocator used to allocate character buffer
		*/
		constexpr 
		explicit BinaryString(character_t const* str, Allocator alloc = Allocator{}) noexcept
		  : BinaryString{str, str+wcslen(str), alloc}
        {
		}
		
		/**
		 * @brief	Take ownership of elements in a null-terminated C-string
		 * 
		 * @param str		Null-terminated character array
		 * @param alloc		[optional] Allocator used to allocate character buffer
		*/
		constexpr 
		explicit BinaryString(meta::adopt_t, character_t const* str, Allocator alloc = Allocator{}) noexcept
		  : base{static_cast<uint32_t>(wcslen(str)),const_cast<character_t*>(str)}, m_alloc{alloc}
        {
		}
		
		/**
		 * @brief	Overwrite with elements copied from a null-terminated C-string 
		 * 
		 * @param r		Other string
		*/
		constexpr type& 
		operator=(character_t const* const str) noexcept
		{
			type{str}.swap(*this);
			return *this;
		}
		
#if ATL_STRING_SUPPORT
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
		constexpr 
		explicit BinaryString(std::basic_string<C,T,A> const& str, Allocator alloc = Allocator{}) noexcept
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
		 * @brief	Construct with buffer moved from another string
		 * 
		 * @param r		Other string
		*/
		constexpr 
		BinaryString(type&& r) noexcept
		  : base{}, m_alloc{r.get_allocator()}
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
		  : base{}, m_alloc{alloc}
		{
			if (r.get_allocator() == alloc)
			{
				r.swap(*this);
				std::swap(this->m_alloc, r.m_alloc);
			}
			else
			{
				type{r.begin(), r.end(), alloc}.swap(*this);
			}
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
				std::swap(this->m_alloc, r.m_alloc);
			}
			else 
			{
				type{r.begin(), r.end(), this->m_alloc}.swap(*this);
				std::swap(this->m_alloc, r.m_alloc);
			} 
			return *this;
		}
		
		//! @brief	De-allocate string buffer
		constexpr 
		~BinaryString() noexcept
		{
			if (this->initialized())
            {
				this->m_alloc.deallocate(this->Buffer);
				this->Buffer = nullptr;
			}
		}
		
		/**
		 * @brief	Overwrite with elements copied from the range [first, end]
		 * 
		 * @tparam	InputIterator	Any input iterator
		 * 
		 * @param first		First element in range
		 * @param end		One past last element in range
		*/
		template <typename InputIterator>
		void constexpr
		assign(InputIterator first, InputIterator end) noexcept
        {	
			static_assert(std::is_convertible_v<decltype(*first),character_t>);
			if (ptrdiff_t const nChars = std::distance(first,end); 
				nChars < static_cast<size_type>(-1) && detail::is_valid_range(first,end))
			{	
				if (auto* newBuffer = this->m_alloc.allocate(nChars+1); newBuffer)
                {
					if (this->Buffer) 
					{
						this->m_alloc.deallocate(this->Buffer);
					}
					this->Buffer = newBuffer;
					this->Length = static_cast<size_type>(nChars) * sizeof(character_t);
					std::copy(first, end, this->Buffer);
					this->Buffer[nChars] = null;
				}
			}
		}

		/**
		 * @brief	Retrieve const-reference to back character
		 * 
		 * @remarks	Undefined behaviour when the string is empty or uninitialized
		*/
		const_reference constexpr
		back() const noexcept
		{
			return this->Buffer[this->size()-1];
		}

		/**
		 * @brief	Retrieve reference to back character
		 * 
		 * @remarks	Undefined behaviour when the string is empty or uninitialized
		*/
		reference constexpr
		back() noexcept
		{
			return this->Buffer[this->size()-1];
		}

		/**
		 * @brief	Retrieve const-iterator positioned at first character
		 * 
		 * @return	nullptr when uninitialized
		*/
		const_iterator constexpr
		begin() const noexcept
        {
			return this->initialized() ? &this->Buffer[0] : nullptr;
		}

		/**
		 * @brief	Retrieve iterator positioned at first character
		 * 
		 * @return	nullptr when uninitialized
		*/
		iterator constexpr 
		begin() noexcept
        {
			return this->initialized() ? &this->Buffer[0] : nullptr;
		}

		/**
		 * @brief	Retrieve const-pointer to internal character buffer
		 * 
		 * @return	nullptr when uninitialized
		*/
		character_t const constexpr* 
		c_str() const & noexcept
		{
			return this->data();
		}

		/**
		 * @brief	Retrieve pointer to internal character buffer
		 * 
		 * @return	nullptr when uninitialized
		*/
		character_t constexpr* 
		c_str() & noexcept
		{
			return this->data();
		}

		/**
		 * @brief	Retrieve const-pointer to internal character buffer
		 * 
		 * @return	nullptr when uninitialized
		*/
		character_t const constexpr* 
		data() const & noexcept
		{
			return this->initialized() ? &this->Buffer[0] : nullptr;
		}

		/**
		 * @brief	Retrieve pointer to internal character buffer
		 * 
		 * @return	nullptr when uninitialized
		*/
		character_t constexpr* 
		data() & noexcept
		{
			return this->initialized() ? &this->Buffer[0] : nullptr;
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
		 * @brief	Retrieve const-iterator positioned immediately beyond last character
		 * 
		 * @return	nullptr when uninitialized
		*/
		const_iterator constexpr
		end() const noexcept
        {
			return this->initialized() ? &this->Buffer[this->size()] : nullptr;
		}

		/**
		 * @brief	Retrieve iterator positioned immediately beyond last character
		 * 
		 * @return	nullptr when uninitialized
		*/
		iterator constexpr
		end() noexcept
        {
			return this->initialized() ? &this->Buffer[this->size()] : nullptr;
		}
		
		/**
		 * @brief	Retrieve const-reference to front character
		 * 
		 * @remarks	Undefined behaviour when the string is empty or uninitialized
		*/
		const_reference constexpr
		front() const noexcept
		{
			return this->Buffer[0];
		}

		/**
		 * @brief	Retrieve reference to front character
		 * 
		 * @remarks	Undefined behaviour when the string is empty or uninitialized
		*/
		reference constexpr
		front() noexcept
		{
			return this->Buffer[0];
		}

		/**
		 * @brief	Query whether string is empty
		 * 
		 * @return	true iff empty or uninitialized
		*/
		bool constexpr
		empty() const noexcept
        {
			return this->Length == 0;
		}

		/**
		 * @brief	Query whether initialized
		*/
		allocator_type constexpr
		get_allocator() const noexcept
		{
			return this->m_alloc;
		}
		
		/**
		 * @brief	Query whether initialized
		 * 
		 * @remarks	String is uninitialized when default-constructed
		*/
		bool constexpr
		initialized() const noexcept
        {
			return this->Buffer != nullptr;
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
		 * @brief	Swap with another string
		 * 
		 * @param r	Other string
		*/
		void constexpr
		swap(type& r) noexcept
        {
			std::swap(this->m_alloc, r.m_alloc);
			std::swap(this->Buffer, r.Buffer);
			std::swap(this->Length, r.Length);
		}
		
		/**
		 * @brief	Convert into a string view
		*/
		implicit operator 
		std::wstring_view() const noexcept
		{
			return {this->Buffer, this->Buffer+this->Length};
		}
		
	private:
		BinaryString<NoopAllocator<wchar_t>> constexpr 
		friend literals::string_literals::operator""_bstr(const wchar_t*, size_t) noexcept;
		
		//! @brief	Private constructor for literal operators
		constexpr 
		BinaryString(meta::hidden_t, character_t const* str, size_type len) noexcept
		  : BinaryString{str, str+len}
		{
		}

		char_alloc_t m_alloc;
	};
	
	template <uint32_t N>
	explicit BinaryString(wchar_t const (&&)[N]) -> BinaryString<NoopAllocator<wchar_t>>;
	
	inline namespace literals
	{
		inline namespace string_literals
		{
			constexpr BinaryString<NoopAllocator<wchar_t>> 
			operator""_bstr(wchar_t const* ws, size_t n) noexcept
			{
				return {meta::hidden, ws, static_cast<uint32_t>(n)};
			}
		}
	}

	extern template BinaryString<allocator<wchar_t>>;

	using wstring = BinaryString<allocator<wchar_t>>;
}
