#pragma once
#include "comFramework.h"

namespace win
{
    namespace detail
    {   
        auto constexpr
        inline CloseHandlePolicy = [](::HANDLE handle) noexcept {
            ::CloseHandle(handle);
        };

        auto constexpr
        inline RegCloseKeyPolicy = [](::HKEY key) noexcept {
            ::RegCloseKey(key);
        };
        
        auto constexpr
        inline FreeLibraryPolicy = [](::HMODULE handle) noexcept {
            ::FreeLibrary(handle);
        };
        
        template <typename RawHandle>
        using NullHandle = std::integral_constant<RawHandle,nullptr>;
        
        /*template <typename RawHandle>
        RawHandle constexpr
        inline NullHandle = nullptr;*/

        using InvalidHandle = std::integral_constant<::HANDLE,INVALID_HANDLE_VALUE>;

        /*::HANDLE constexpr
        inline InvalidHandle = INVALID_HANDLE_VALUE;*/

    } 

    //! @brief  Smart-handle with shared-ownership semantics
    template <
        typename RawHandle, 
        auto Releaser = detail::CloseHandlePolicy, 
        RawHandle Empty = detail::NullHandle<RawHandle>::value
    >
    class SmartHandle
    {
        using type = SmartHandle<RawHandle,Releaser,Empty>;
        
        auto constexpr 
        inline static Deleter = [](RawHandle* ptr) 
        {
            if (ptr && *ptr != Empty)
            {
                Releaser(*ptr);
            }
        };

    private:
        std::shared_ptr<RawHandle> m_handle;

    public:
        constexpr
        explicit
        SmartHandle(RawHandle handle = Empty) 
          : m_handle{new RawHandle{handle},Deleter}
        {
        }
        
        SmartHandle& 
        operator=(RawHandle handle) 
        {
            type{handle}.swap(*this);
            return *this;
        }

        satisfies(SmartHandle,
            constexpr_IsCopyable_noexcept,
            constexpr_IsMovable_noexcept,
            constexpr IsDestructible_noexcept
        );
        
    public:
        //! @brief  Close the managed handle prematurely thereby invalidating (but not releasing) it
        void
        static close(type& handle) noexcept
        {
            Releaser(*handle);
        }
        
        bool
        empty() const noexcept
        {
            return !this->m_handle ||
                   *this->m_handle == Empty;
        }

        RawHandle*
        get() const noexcept
        {
            return this->m_handle.get();
        }

        void 
        reset(RawHandle handle = Empty) 
        {
            this->m_handle.reset(new RawHandle{handle},Deleter);
        }

        void 
        swap(type& r) noexcept
        {
            this->m_handle.swap(r.m_handle);
        }
        
        RawHandle 
        operator*() const noexcept
        {
            return *this->m_handle.get();
        }

        implicit operator 
        bool() const noexcept
        {
            return !this->empty();
        }
    };
    
    //! @brief  Smart-handle released using ::CloseHandle()
    using SharedHandle = SmartHandle<::HANDLE>;

    //! @brief  Smart-handle released using ::CloseHandle()
    using SharedEvent = SharedHandle;
    
    //! @brief  Smart-handle released using ::CloseHandle()
    using SharedFile = SmartHandle<::HANDLE, detail::CloseHandlePolicy, detail::InvalidHandle::value>;
    
    //! @brief  Smart-handle released using ::RegCloseKey()
    using SharedRegistryKey = SmartHandle<::HKEY, detail::RegCloseKeyPolicy>;

    //! @brief  Smart-handle released using ::FreeLibrary()
    using SharedModule = SmartHandle<::HMODULE, detail::FreeLibraryPolicy>;

} // namespace win