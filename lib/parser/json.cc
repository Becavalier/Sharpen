/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#include "lib/core/core_util.h"
#include "lib/parser/json.h"

using sharpen_core::Util;

namespace sharpen_parser {

char const* RSJobjectbrackets = "{}";
char const* RSJarraybrackets = "[]";
constexpr char RSJobjectassignment = ':';
constexpr char RSJarraydelimiter = ',';

std::vector<char const*> RSJbrackets = {
    RSJobjectbrackets,
    RSJarraybrackets
};
std::vector<char const*> RSJstringquotes = {"\"\"", "''"};
char RSJcharescape = '\\';

std::string Json::stripQuotes(std::string str) {
    return Util::strTrim(str, " \"'");
}

int Json::isBracket(char c, const std::vector<const char*> &bracks, int index) {
    for (int b = 0; b < bracks.size(); ++b)
        if (c == bracks[b][index])
            return (b);
    return (-1);
}

// [boottleneck];
std::vector<std::string> Json::splitRSJArray(std::string str) {
    std::vector<std::string> ret;
    std::string current;
    std::vector<int> bracketStack;
    std::vector<int> quoteStack;
    bool escapeActive = false;
    int bi;

    str = Util::strTrim(str);
    for (int a = 0; a < str.length(); ++a) {
        // delimiter;
        if (bracketStack.size() == 0 &&
            quoteStack.size() == 0 &&
            str[a] == RSJarraydelimiter) {
            ret.push_back(current);
            current.clear();
            escapeActive = false;
            continue;  // to *
        }

        // checks for string;
        if (quoteStack.size() > 0) {
            if (str[a] == RSJcharescape) {
                escapeActive = !escapeActive;
            } else if (!escapeActive &&
                str[a] == RSJstringquotes[quoteStack.back()][1]) {
                quoteStack.pop_back();
                escapeActive = false;
            } else {
                escapeActive = false;
            }

            current.push_back(str[a]);
            continue;
        }

        if (quoteStack.size() == 0) {
            if ((bi = isBracket(str[a], RSJstringquotes)) >= 0) {
                quoteStack.push_back(bi);
                current.push_back(str[a]);
                continue;
            }
        }

        // checks for brackets;
        if (bracketStack.size() > 0 && str[a] == RSJbrackets[bracketStack.back()][1]) {
            bracketStack.pop_back();
            current.push_back(str[a]);
            continue;
        }

        if ((bi = isBracket(str[a], RSJbrackets)) >= 0) {
            bracketStack.push_back(bi);
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

std::shared_ptr<TypeRoot> Json::parse(const std::string& data) {
    std::shared_ptr<TypeRoot> t;
    std::string content = Util::strTrim(data);

    // parse as object;
    content = Util::strTrim(content, " {", Util::strTrimOpts::_STRTRIM_LEFT_);
    content = Util::strTrim(content, " }", Util::strTrimOpts::_STRTRIM_RIHGT_);
    if (content.length() != data.length()) {
        t = TypeFactory::buildMap();
        std::vector<std::string> nvPairs = splitRSJArray(content);
        for (int a = 0; a < nvPairs.size(); ++a) {
            std::size_t assignmentPos = nvPairs[a].find(RSJobjectassignment);
            std::static_pointer_cast<Map>(t)->addItem(
                stripQuotes(nvPairs[a].substr(0, assignmentPos)),
                this->parse(Util::strTrim(nvPairs[a].substr(assignmentPos + 1))));
        }
        return t;
    }

    // parse as array;
    content = Util::strTrim(content, " [", Util::strTrimOpts::_STRTRIM_LEFT_);
    content = Util::strTrim(content, " ]", Util::strTrimOpts::_STRTRIM_RIHGT_);
    if (content.length() != data.length()) {
        t = TypeFactory::buildArray();
        std::vector<std::string> nvPairs = splitRSJArray(content);
        for (int a = 0; a < nvPairs.size(); ++a) {
            std::static_pointer_cast<Array>(t)->addItem(this->parse(Util::strTrim(nvPairs[a])));
        }
        return t;
    }

    // parse as string;
    return TypeFactory::buildString(stripQuotes(data));
}

std::shared_ptr<TypeRoot> Json::parseAll(void) {
    return this->parse(this->data);
}

}  // namespace sharpen_parser
