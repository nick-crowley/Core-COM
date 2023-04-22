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
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Name Imports o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Forward Declarations o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Macro Definitions o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Constants & Enumerations o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Class Declarations o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com 
{
    //! @brief  Allocator which constructs/destructs but nothing when asked to perform allocation/deallocation
    template <typename T>
    class HeapAllocator
    {
        static_assert(!std::is_const_v<T>, "The C++ Standard forbids containers and allocators of const elements");
        using type = HeapAllocator<T>;

    public:
        using value_type      = T;
        using pointer         = T*;
        using const_pointer   = T const*;
        using reference       = T&;
        using const_reference = T const&;
        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;
        using propagate_on_container_move_assignment = std::true_type;
        using is_always_equal = std::true_type;
        
        template <typename U>
        struct rebind {
            using other = HeapAllocator<U>;
        };

    public:
        satisfies(HeapAllocator,
            constexpr IsDefaultConstructible noexcept,
            constexpr IsCopyConstructible noexcept,
            constexpr IsCopyAssignable noexcept,
            constexpr IsDestructible noexcept
        );

        template <class U>
        constexpr 
        implicit
        HeapAllocator(const HeapAllocator<U>&) noexcept 
        {}

    public:
        [[nodiscard]] 
        pointer constexpr 
        address(reference obj) const noexcept 
        {
            return std::addressof(obj);
        }

        [[nodiscard]] 
        const_pointer constexpr 
        address(const_reference obj) const noexcept 
        {
            return std::addressof(obj);
        }
    
        [[nodiscard]]
        pointer 
        __declspec(allocator) allocate(size_type const count) const noexcept 
        {
            return static_cast<pointer>(::CoTaskMemAlloc(count));
        }
        
        [[nodiscard]]
        pointer 
        __declspec(allocator) allocate(size_type const count, void const*) const noexcept 
        {
            return static_cast<pointer>(::CoTaskMemAlloc(count));
        }

        void 
        deallocate(pointer const address, size_type const = (size_type)-1) const noexcept 
        {
            ::CoTaskMemFree(address);
        }

        template <typename... Params>
        void constexpr 
        construct(pointer const address, Params&&... args) const noexcept 
        {
            ::new (address) value_type(std::forward<Params>(args)...);
        }

        template <typename U>
        void constexpr 
        destroy(U* const obj) const noexcept 
        {
            obj->~U();
        }

        [[nodiscard]] 
        size_type constexpr 
        max_size() const noexcept 
        {
            return size_type(-1) / sizeof(value_type);
        }
    };
    
    
    template <>
    class HeapAllocator<void> 
    {
        using type = HeapAllocator<void>;

    public:
        using value_type      = void;
        using pointer         = void*;
        using const_pointer   = void const*;
        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;
        using propagate_on_container_move_assignment = std::true_type;
        using is_always_equal = std::true_type;
        
        template <typename U>
        struct rebind {
            using other = HeapAllocator<U>;
        };
        
    public:
        satisfies(HeapAllocator,
            constexpr IsDefaultConstructible noexcept,
            constexpr IsCopyConstructible noexcept,
            constexpr IsCopyAssignable noexcept,
            constexpr IsDestructible noexcept
        );

        template <class U>
        constexpr 
        implicit
        HeapAllocator(HeapAllocator<U> const& r) noexcept
        {}

    public:
        pointer constexpr 
        allocate(size_type const) const noexcept = delete;

        void 
        deallocate(pointer const address, size_type const = (size_type)-1) const noexcept 
        {
            ::CoTaskMemFree(address);
        }
    };
    

    //! @brief  Standard-library equivalent name
    template <class T>
    using allocator = HeapAllocator<T>;
}
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-o Non-member Methods o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o

// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~o Global Functions o~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
namespace core::com 
{
    template <typename T, typename U>    
    bool constexpr 
    operator==(HeapAllocator<T> const&, HeapAllocator<U> const&) noexcept
    {
        return true;
    }
    
    template <typename T, typename U>    
    bool constexpr 
    operator!=(HeapAllocator<T> const&, HeapAllocator<U> const&) noexcept
    {
        return false;
    }

    //! @brief  Convenience variable for COM allocator
    HeapAllocator<void> constexpr
    inline allocCom;

} // namespace core::com
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=-o End of File o-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o