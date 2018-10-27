/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#ifndef LIB_PARSER_JSON_H_
#define LIB_PARSER_JSON_H_

#include <string>
#include <vector>
#include <utility>
#include <memory>
#include "lib/core/core_type_factory.h"

using sharpen_core::TypeFactory;
using sharpen_core::Map;
using sharpen_core::Array;
using sharpen_type::TypeRoot;

namespace sharpen_parser {

class Json {
    std::string data;
    std::shared_ptr<TypeRoot> n;

    std::string stripQuotes(std::string str);
    int isBracket(char c, const std::vector<char const*> &bracks, int index = 0);
    std::vector<std::string> splitRSJArray(std::string str);
    std::shared_ptr<TypeRoot> parse(const std::string& data);

 public:
    explicit Json(std::string str) : data(str) {}
    explicit Json(const char* str) : Json(std::string(str)) {}
    ~Json() = default;
    // expose interface;
    std::shared_ptr<TypeRoot> parseAll(void);
};

}  // namespace sharpen_parser

#endif  // LIB_PARSER_JSON_H_
