#ifndef __CORE_UTIL__
#define __CORE_UTIL__

#include "core_type.h"
#include <type_traits>
#include <string>

using namespace sharpen_type;

namespace sharpen_core {

    struct Util {
        template<typename T>
        static JSTypes getNumberType (T) {
            return std::is_integral<T>::value ? JSTYPE_INTEGER : JSTYPE_FLOAT;
        };

        static std::string toStr (const std::string &s);
        static std::string toStr (int i);
    };   
}

#endif