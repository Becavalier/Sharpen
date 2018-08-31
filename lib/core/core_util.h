#ifndef __CORE_UTIL__
#define __CORE_UTIL__

#include "core_types.h"

#include <type_traits>

using namespace types;

namespace core {

    struct Util {
        template<typename T>
        static types::JSTypes getNumberType (T) {
            return std::is_integral<T>::value ? JSTYPE_INTEGER : JSTYPE_FLOAT;
        };
    };

}

#endif