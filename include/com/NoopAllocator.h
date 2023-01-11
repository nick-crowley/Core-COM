#pragma once
#include "comFramework.h"

namespace com 
{
    //! @brief  Allocator which constructs/destructs but nothing when asked to perform allocation/deallocation
    template <typename T>
    class NoopAllocator 
    {
        static_assert(!std::is_const_v<T>, "The C++ Standard forbids containers and allocators of const elements");
        using type = NoopAllocator<T>;

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
            using other = NoopAllocator<U>;
        };

    public:
        satisfies(NoopAllocator,
            constexpr IsDefaultConstructible noexcept,
            constexpr IsCopyConstructible noexcept,
            constexpr IsCopyAssignable noexcept,
            constexpr IsDestructible noexcept
        );

        template <class U>
        constexpr 
        implicit
        NoopAllocator(const NoopAllocator<U>&) noexcept 
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
        pointer constexpr
        __declspec(allocator) allocate(size_type const /*count*/) const noexcept 
        {
            return nullptr;
        }
        
        [[nodiscard]] 
        pointer constexpr
        __declspec(allocator) allocate(size_type const /*count*/, void const*) const noexcept 
        {
            return nullptr;
        }

        void constexpr 
        deallocate(pointer const /*address*/, size_type const = (size_type)-1) const noexcept 
        {
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
    class NoopAllocator<void> 
    {
        using type = NoopAllocator<void>;

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
            using other = NoopAllocator<U>;
        };

    public:
        satisfies(NoopAllocator,
            constexpr IsDefaultConstructible noexcept,
            constexpr IsCopyConstructible noexcept,
            constexpr IsCopyAssignable noexcept,
            constexpr IsDestructible noexcept
        );

        template <class U>
        constexpr 
        implicit
        NoopAllocator(const NoopAllocator<U>& r) noexcept
        {}

        pointer constexpr 
        allocate(size_type const) const noexcept = delete;

        void constexpr 
        deallocate(pointer const /*address*/, size_type const = (size_type)-1) const noexcept 
        {
        }
    };
    
    template <typename T, typename U>    
    bool constexpr 
    operator==(NoopAllocator<T> const&, NoopAllocator<U> const&) noexcept
    {
        return true;
    }
    
    template <typename T, typename U>    
    bool constexpr 
    operator!=(NoopAllocator<T> const&, NoopAllocator<U> const&) noexcept
    {
        return false;
    }

    //! @brief  Convenience variable for all no-op allocators
    NoopAllocator<void> constexpr
    inline allocNoop;

} // namespace com
