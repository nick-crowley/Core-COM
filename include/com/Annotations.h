#pragma once
#include "comFramework.h"

namespace com
{
    template <typename ValueType>
    using retval_t = std::add_pointer_t<ValueType>;
    
    template <typename ValueType>
    using in_t = std::add_pointer_t<ValueType>;

    template <typename ValueType>
    using out_t = std::add_pointer_t<ValueType>;
}
