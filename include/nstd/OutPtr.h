#pragma once
#include "comFramework.h"

namespace nstd
{	
    namespace detail
    {
        struct use_default_t 
        {
            satisfies(use_default_t,
		        constexpr IsDefaultConstructible_noexcept
	        );
        } 
        constexpr inline 
        use_default;
    }

    template <typename Input, typename Output>
    class out_ptr_t
    {
        static_assert(std::is_default_constructible_v<Input>);

        Output& m_curValue;
        Input   m_newValue {};

    public:
        constexpr 
        explicit out_ptr_t(Output& result) noexcept : m_curValue(result)
        {
            static_assert(std::is_void_v<std::void_t<decltype(result = static_cast<Output>(0))>>, 
                "No conversion possible between in/out types");
            static_assert(!std::is_const_v<Output>, "Cannot overwrite a constant pointer address");
        }

        constexpr 
        ~out_ptr_t() noexcept 
        {
            if (this->m_newValue)
            {
                if constexpr (std::is_convertible_v<Input,Output>)
                {
                    this->m_curValue = this->m_newValue;
                }
                else 
                {
                    #pragma warning (suppress : 26466) // Don't use static_cast downcasts
                    this->m_curValue = static_cast<Output>(this->m_newValue);
                }
            }
        }

        template <typename T>
        constexpr 
        operator T*() && noexcept
        {
            if constexpr (std::is_constructible_v<T*,Input*>)
                return static_cast<T*>(&this->m_newValue);
            else
                return reinterpret_cast<T*>(&this->m_newValue);
        }
    };

    /**
     * @brief   Construct wrapper to handle receiving a new pointer value
     * 
     * @tparam  Input     [optional] Type of value addressed by the pointer
     * 
     * @param   ptr       [in] Destination pointer
     * 
     * @return  Temporary wrapper; do not save
    */
    template <typename Input = detail::use_default_t, 
              typename Output = void, 
              typename _Input = std::conditional_t<std::is_same_v<Input,detail::use_default_t>,Output,Input>>
    out_ptr_t<_Input,Output> constexpr
    inline out_ptr(Output& ptr) 
    { 
        return out_ptr_t<_Input,Output>{ptr};
    }

} // namespace nstd
