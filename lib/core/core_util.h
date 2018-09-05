#ifndef __CORE_UTIL__
#define __CORE_UTIL__

#include "core_type.h"

#include <type_traits>

using namespace sharpen_type;

namespace sharpen_core {

    struct Util {
        template<typename T>
        static sharpen_type::JSTypes getNumberType (T) {
            return std::is_integral<T>::value ? JSTYPE_INTEGER : JSTYPE_FLOAT;
        };
    };

}

#endif