/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#ifndef LIB_PARSER_JSON_H_
#define LIB_PARSER_JSON_H_

#include <string>
#include <vector>
#include <utility>
#include "lib/core/core_type_factory.h"

using sharpen_core::TypeFactory;
using sharpen_core::Map;
using sharpen_core::Array;
using sharpen_type::TypeRoot;

namespace sharpen_parser {

std::string strip_outer_quotes(std::string str);
int is_bracket(char c, const std::vector<char const*> &bracks, int index = 0);
// [boottleneck];
std::vector<std::string> split_RSJ_array(std::string str);

class RSJresource {
    std::string data;
    TypeRoot* n;

 public:
    explicit RSJresource(std::string str) : data(str) {}
    explicit RSJresource(const char* str) : RSJresource(std::string(str)) {}

    ~RSJresource() {}

    TypeRoot* parseAll(void);
    TypeRoot* parse(const std::string& data);
};

}  // namespace sharpen_parser

#endif  // LIB_PARSER_JSON_H_
