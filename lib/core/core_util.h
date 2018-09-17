/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#ifndef LIB_CORE_CORE_UTIL_H_
#define LIB_CORE_CORE_UTIL_H_

#include "lib/core/core_type.h"
#include <type_traits>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>

using sharpen_type::JSTypes;
using LDMovementResult = std::vector<std::map<std::string, int>>;

namespace sharpen_core {

struct Util {
    enum LD_OPS {
        _LD_STEP_ADD_ = 0,
        _LD_STEP_DEL_,
        _LD_STEP_REP_,
        _LD_STEP_VOID_
    };

    template<typename T>
    static JSTypes getNumberType(T) {
        return std::is_integral<T>::value ?
            JSTypes::JSTYPE_INTEGER :
            JSTypes::JSTYPE_FLOAT;
    }

    static std::string toStr(const std::string &s);
    static std::string toStr(int i);

    static std::string strtrim(
        std::string str,
        std::string chars = " \t\n\r",
        std::string opts = "lr");

    template<typename C>
    static void print(const C& cont) {
        std::ostream_iterator<typename C::value_type> outIter(std::cout, " ");
        std::copy(
            begin(cont),
            end(cont),
            outIter);
        std::cout << std::endl;
    }

    static LDMovementResult findLevenshteinDistancePath(
        const std::vector<std::string>&,
        const std::vector<std::string>&);

    static void applyLDResult(
        std::vector<std::string>&,
        std::vector<std::string>&,
        LDMovementResult&);
};

}  // namespace sharpen_core

#endif  // LIB_CORE_CORE_UTIL_H_
