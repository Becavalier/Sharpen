/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#include "lib/core/core_util.h"
#include "lib/parser/json.h"

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

std::string stripQuotes(std::string str) {
    str = Util::strtrim(str);

    std::string ret = Util::strtrim(str, "\"");
    if (ret == str) {
        ret = Util::strtrim(str, "'");
    }

    return (ret);
}

int isBracket(char c, const std::vector<const char*> &bracks, int index) {
    for (int b = 0; b < bracks.size(); ++b)
        if (c == bracks[b][index])
            return (b);
    return (-1);
}

// [boottleneck];
std::vector<std::string> splitRSJArray(std::string str) {
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
            if ((bi = isBracket(str[a], RSJstringquotes)) >= 0) {
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

        if ((bi = isBracket(str[a], RSJbrackets)) >= 0) {
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
    std::string s;
    for (auto e : ret) {
        if (!s.empty() || (e.back() != '}' && e.find(":{") != std::string::npos)) {
            s += (e + ",");
            if (e.back() == '}' && e.find(":{") == std::string::npos) {
                e = s.substr(0, -1);
                s = "";
            }
        }
        if (s.empty()) {
            fixedRet.push_back(e);
        }
    }

    return (fixedRet);
}

std::shared_ptr<TypeRoot> RSJresource::parse(const std::string& data) {
    std::shared_ptr<TypeRoot> t;
    std::string content = Util::strtrim(data);

    // parse as object;
    content = Util::strtrim(content, " {", "l");
    content = Util::strtrim(content, " }", "r");
    if (content.length() != data.length()) {
        t = TypeFactory::buildMap();
        std::vector<std::string> nvPairs = splitRSJArray(content);
        for (int a = 0; a < nvPairs.size(); ++a) {
            std::size_t assignmentPos = nvPairs[a].find(RSJobjectassignment);
            std::static_pointer_cast<Map>(t)->addItem(
                stripQuotes(nvPairs[a].substr(0, assignmentPos)),
                this->parse(Util::strtrim(nvPairs[a].substr(assignmentPos + 1))));
        }
        return t;
    }

    // parse as array;
    content = Util::strtrim(content, " [", "l");
    content = Util::strtrim(content, " ]", "r");
    if (content.length() != data.length()) {
        t = TypeFactory::buildArray();
        std::vector<std::string> nvPairs = splitRSJArray(content);
        for (int a = 0; a < nvPairs.size(); ++a) {
            std::static_pointer_cast<Array>(t)->addItem(this->parse(Util::strtrim(nvPairs[a])));
        }
        return t;
    }

    // parse as string;
    return TypeFactory::buildString(stripQuotes(data));
}

std::shared_ptr<TypeRoot> RSJresource::parseAll(void) {
    return this->parse(this->data);
}

}  // namespace sharpen_parser
