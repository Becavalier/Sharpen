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

std::string stripQuotes(std::string str);
int isBracket(char c, const std::vector<char const*> &bracks, int index = 0);
// [boottleneck];
std::vector<std::string> splitRSJArray(std::string str);

class RSJresource {
    std::string data;
    std::shared_ptr<TypeRoot> n;

 public:
    explicit RSJresource(std::string str) : data(str) {}
    explicit RSJresource(const char* str) : RSJresource(std::string(str)) {}
    ~RSJresource() {}
    std::shared_ptr<TypeRoot> parseAll(void);
    std::shared_ptr<TypeRoot> parse(const std::string& data);
};

}  // namespace sharpen_parser

#endif  // LIB_PARSER_JSON_H_
