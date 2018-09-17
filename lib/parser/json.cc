/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#include "lib/parser/json.h"
#include "lib/core/core_util.h"

using sharpen_core::Util;

namespace sharpen_parser {

char const* RSJobjectbrackets = "{}";
char const* RSJarraybrackets = "[]";
char RSJobjectassignment = ':';
char RSJarraydelimiter = ',';

std::vector<char const*> RSJbrackets = {
    RSJobjectbrackets,
    RSJarraybrackets
};
std::vector<char const*> RSJstringquotes = {"\"\"", "''"};
char RSJcharescape = '\\';

std::string strip_outer_quotes(std::string str) {
    str = Util::strtrim(str);

    std::string ret = Util::strtrim(str, "\"");
    if (ret == str) {
        ret = Util::strtrim(str, "'");
    }

    return (ret);
}


int is_bracket(char c, const std::vector<const char*> &bracks, int index) {
    for (int b = 0; b < bracks.size(); ++b)
        if (c == bracks[b][index])
            return (b);
    return (-1);
}

// [boottleneck];
std::vector<std::string> split_RSJ_array(std::string str) {
    std::vector<std::string> ret;
    std::string current;
    std::vector<int> bracket_stack;
    std::vector<int> quote_stack;
    bool escape_active = false;
    int bi;

    str = Util::strtrim(str);
    for (int a = 0; a < str.length(); ++a) {
        // delimiter;
        if (bracket_stack.size() == 0 &&
            quote_stack.size() == 0 &&
            str[a] == RSJarraydelimiter) {
            ret.push_back(current);
            current.clear();
            bracket_stack.clear();
            quote_stack.clear();
            escape_active = false;
            continue;  // to *
        }

        // checks for string;
        if (quote_stack.size() > 0) {
            if (str[a] == RSJcharescape) {
                escape_active = !escape_active;
            } else if (!escape_active &&
                str[a] == RSJstringquotes[quote_stack.back()][1]) {
                quote_stack.pop_back();
                escape_active = false;
            } else {
                escape_active = false;
            }

            current.push_back(str[a]);
            continue;
        }

        if (quote_stack.size() == 0) {
            if ((bi = is_bracket(str[a], RSJstringquotes)) >= 0) {
                quote_stack.push_back(bi);
                current.push_back(str[a]);
                continue;
            }
        }

        // checks for brackets;
        if (bracket_stack.size() > 0 && str[a] == RSJbrackets[bracket_stack.back()][1]) {
            bracket_stack.pop_back();
            current.push_back(str[a]);
            continue;
        }

        if ((bi = is_bracket(str[a], RSJbrackets)) >= 0) {
            bracket_stack.push_back(bi);
            current.push_back(str[a]);
            continue;
        }

        // otherwise;
        current.push_back(str[a]);
    }

    if (current.length() > 0) {
        ret.push_back(current);
    }

    // fix "object";
    std::vector<std::string> fixedRet;
    std::string _s;
    for (auto e : ret) {
        if (!_s.empty() || (e.back() != '}' && e.find(":{") != std::string::npos)) {
            _s += (e + ",");
            if (e.back() == '}' && e.find(":{") == std::string::npos) {
                e = _s.substr(0, -1);
                _s = "";
            }
        }
        if (_s.empty()) {
            fixedRet.push_back(e);
        }
    }

    return (fixedRet);
}

TypeRoot* RSJresource::parse(const std::string& data) {
    TypeRoot* _t;
    std::string content = Util::strtrim(data);

    // parse as object;
    content = Util::strtrim(content, " {", "l");
    content = Util::strtrim(content, " }", "r");
    if (content.length() != data.length()) {
        _t = TypeFactory::buildMap();
        std::vector<std::string> nvPairs = split_RSJ_array(content);
        for (int a = 0; a < nvPairs.size(); ++a) {
            std::size_t assignmentPos = nvPairs[a].find(RSJobjectassignment);
            static_cast<Map*>(_t)->addItem(
                strip_outer_quotes(nvPairs[a].substr(0, assignmentPos)),
                this->parse(Util::strtrim(nvPairs[a].substr(assignmentPos + 1))));
        }
        return _t;
    }

    // parse as array;
    content = Util::strtrim(content, " [", "l");
    content = Util::strtrim(content, " ]", "r");
    if (content.length() != data.length()) {
        _t = TypeFactory::buildArray();
        std::vector<std::string> nvPairs = split_RSJ_array(content);
        for (int a = 0; a < nvPairs.size(); ++a) {
            static_cast<Array*>(_t)->addItem(this->parse(Util::strtrim(nvPairs[a])));
        }
        return _t;
    }

    // parse as string;
    return TypeFactory::buildString(strip_outer_quotes(data));
}

TypeRoot* RSJresource::parseAll(void) {
    return this->parse(this->data);
}

}  // namespace sharpen_parser
