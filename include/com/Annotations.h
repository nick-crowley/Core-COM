#pragma once
#include "library/core.COM.h"

namespace core::com
{
    template <typename ValueType>
    using retval_t = std::add_pointer_t<ValueType>;
    
    template <typename ValueType>
    using in_t = std::add_pointer_t<ValueType>;

    template <typename ValueType>
    using out_t = std::add_pointer_t<ValueType>;
}
