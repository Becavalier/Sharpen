/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#include "lib/core/core_util.h"

std::string sharpen_core::Util::toStr(const std::string &s) {
    return s;
}

std::string sharpen_core::Util::toStr(int i) {
    return std::to_string(i);
}

std::string sharpen_core::Util::strtrim(std::string str, std::string chars, std::string opts) {
    if (str.empty()) return(str);

    if (opts.find('l') != std::string::npos) {
        int p;
        for (p = 0; p < str.length(); ++p)
            if (chars.find(str[p]) == std::string::npos) break;
            else if (p == 1 && chars == " {") break;
        str.erase(0, p);
    }

    if (opts.find('r') != std::string::npos) {
        int q, strlenm = str.length() - 1;
        for (q = 0; q < str.length(); ++q)
            if (chars.find(str[strlenm - q]) == std::string::npos) break;
            else if (q == 1 && chars == " }") break;
        str.erase(str.length() - q, q);
    }

    return (str);
}
