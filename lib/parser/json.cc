/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#include "lib/core/core_util.h"
#include "lib/parser/json.h"

using sharpen_core::Util;

namespace sharpen_parser {

constexpr char _PARSE_CHAR_COLON_ = ':';
constexpr char _PARSE_CHAR_LBRACE_ = '{';
constexpr char _PARSE_CHAR_RBRACE_ = '}';
constexpr char _PARSE_CHAR_COMMA_ = ',';
constexpr char _PARSE_CHAR_SQUARE_LBRACE_ = '[';
constexpr char _PARSE_CHAR_SQUARE_RBRACE_ = ']';
constexpr char _PARSE_CHAR_DOUBLE_QUOTATION_ = '"';
constexpr char _PARSE_CHAR_SPACE_ = ' ';
constexpr char _PARSE_CHAR_N_ = '\n';
constexpr char _PARSE_CHAR_R_ = '\r';

std::shared_ptr<TypeRoot> Json::fastParse(
    parseState state,
    parseType type,
    int level) {
    bool hasQuote = false;
    std::string word, strKey;
    std::shared_ptr<TypeRoot> head, key, value;

    switch (type) {
        case parseType::_PARSE_ARRAY_:
            head = TypeFactory::buildArray();
            break;
        case parseType::_PARSE_MAP_:
            head = TypeFactory::buildMap();
            break;
        default:
            break;
    }

    for (; step < src.length(); ++step) {
        char current = src[step];
        if (current == _PARSE_CHAR_SPACE_ || current == _PARSE_CHAR_N_ || current == _PARSE_CHAR_R_) {
            continue;
        } else if (current == _PARSE_CHAR_DOUBLE_QUOTATION_) {
            hasQuote = !hasQuote;
            if (!hasQuote) {
                if (state == parseState::_PARSE_VALUE_) {
                    value = TypeFactory::buildString(word);
                } else {
                    if (type == parseType::_PARSE_MAP_) {
                        strKey = word;
                    } else {
                        key = TypeFactory::buildString(word);
                    }
                }
                // reset;
                word.clear();
            }
        } else if (!hasQuote && current == _PARSE_CHAR_COLON_) {
            state = parseState::_PARSE_VALUE_;
        } else if (!hasQuote && current == _PARSE_CHAR_LBRACE_) {
            // recursion;
            step++;
            if (state == parseState::_PARSE_VALUE_) {
                value = fastParse(
                    parseState::_PARSE_KEY_,
                    parseType::_PARSE_MAP_,
                    ++level);
            } else {
                key = fastParse(
                    parseState::_PARSE_KEY_,
                    parseType::_PARSE_MAP_,
                    ++level);
            }

        } else if (!hasQuote && current == _PARSE_CHAR_SQUARE_LBRACE_) {
            // recursion;
            step++;
            if (state == parseState::_PARSE_VALUE_) {
                value = fastParse(
                    parseState::_PARSE_KEY_,
                    parseType::_PARSE_ARRAY_,
                    ++level);
            } else {
                key = fastParse(
                    parseState::_PARSE_KEY_,
                    parseType::_PARSE_ARRAY_,
                    ++level);
            }
        } else if (!hasQuote && (
            current == _PARSE_CHAR_COMMA_ ||
            current == _PARSE_CHAR_RBRACE_ ||
            current == _PARSE_CHAR_SQUARE_RBRACE_)) {
            // not eliminated by last step (none-string type);
            if (word.length() > 0) {
                // map first;
                if (state == parseState::_PARSE_VALUE_) {
                    if (word != "true" && word != "false") {
                        value = TypeFactory::buildString(word);
                    } else {
                        value = TypeFactory::buildBool(convertStrToBool(word));
                    }
                // then array;
                } else {
                    if (word != "true" && word != "false") {
                        key = TypeFactory::buildString(word);
                    } else {
                        key = TypeFactory::buildBool(convertStrToBool(word));
                    }
                }
                word.clear();
            }
            // wrap structure;
            if (type == parseType::_PARSE_MAP_) {
                std::static_pointer_cast<Map>(head)->addItem(strKey, value);
            } else {
                std::static_pointer_cast<Array>(head)->addItem(key);
            }
            state = parseState::_PARSE_KEY_;

            if (current != _PARSE_CHAR_COMMA_) {
                // the end;
                return head;
            }
        } else {
            word.push_back(current);
        }
    }
    // unreacable code, optimizing soon;
    return nullptr;
}

}  // namespace sharpen_parser
