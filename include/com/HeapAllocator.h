#pragma once
#include "comFramework.h"

namespace com 
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

    //! @brief  Standard-library equivalent name
    template <class T>
    using allocator = HeapAllocator<T>;

} // namespace com
