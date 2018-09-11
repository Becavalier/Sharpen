/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#ifndef LIB_CORE_CORE_ERROR_H_
#define LIB_CORE_CORE_ERROR_H_

#include <string>
#include <map>
#include <iostream>

using stringMapType = std::map<std::string, std::string>;

namespace sharpen_core {

class Error {
    static stringMapType entity;

 public:
    Error() = default;
    static void say(std::string key) {
        std::cerr << Error::entity.find(key)->second << std::endl;
    }
};

}  // namespace sharpen_core

#endif  // LIB_CORE_CORE_ERROR_H_
