/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#ifndef LIB_CORE_CORE_UTIL_H_
#define LIB_CORE_CORE_UTIL_H_

#include "lib/core/core_type.h"
#include <type_traits>
#include <string>

using sharpen_type::JSTypes;

namespace sharpen_core {

struct Util {
    template<typename T>
    static JSTypes getNumberType(T) {
        return std::is_integral<T>::value ? JSTypes::JSTYPE_INTEGER : JSTypes::JSTYPE_FLOAT;
    }

    static std::string toStr(const std::string &s);
    static std::string toStr(int i);
};

}  // namespace sharpen_core

#endif  // LIB_CORE_CORE_UTIL_H_
