/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#ifndef LIB_PARSER_JSON_H_
#define LIB_PARSER_JSON_H_

#define DEFAULT_INDEX 1

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
    enum class parseType {
        _PARSE_STRING_,
        _PARSE_BOOL_,
        _PARSE_ARRAY_,
        _PARSE_MAP_
    };

    enum class parseState {
        _PARSE_KEY_,
        _PARSE_VALUE_
    };

    int step;
    std::string src;

    inline bool convertStrToBool(const std::string &str) const {
        return str == "true";
    }

 public:
    explicit Json(std::string str) : src(str), step(DEFAULT_INDEX) {}
    explicit Json(const char* str) : Json(std::string(str)) {}
    ~Json() = default;

    std::shared_ptr<TypeRoot> fastParse(
        // initial state;
        parseState state = parseState::_PARSE_KEY_,
        // initial structure;
        parseType type = parseType::_PARSE_MAP_,
        // process level(depth);
        int level = 0);
};

}  // namespace sharpen_parser

#endif  // LIB_PARSER_JSON_H_
