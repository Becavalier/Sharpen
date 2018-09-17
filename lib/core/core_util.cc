/*
#  Copyright (c) 2018 YHSPY. All rights reserved.
*/

#include "lib/core/core_util.h"
#include <numeric>
#include <algorithm>
#include <iterator>
#include <map>

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

LDMovementResult sharpen_core::Util::findLevenshteinDistancePath(
    const std::vector<std::string> &vl,
    const std::vector<std::string> &vr) {
    LDMovementResult movementResult;
    LDMovementResult container;
    int counter = -1;

    auto _CONTAINER_ADD = [&container, &counter](int parent, int op, int x, int y) mutable -> void {
        container.push_back({
            {"parent", parent},
            {"op", op},
            {"x", x},
            {"y", y}
        });
        counter++;
    };

    int vlLen = vl.size();
    int vrLen = vr.size();

    auto columnStart = (decltype(vlLen))1;
    auto column = std::vector<decltype(vlLen)>(vlLen + 1, 0);

    std::iota(column.begin(), column.end(), columnStart - 1);
    _CONTAINER_ADD(-1, _LD_STEP_VOID_, 0, 0);
    for (int i = 1; i < (vl.size() + 1); i++) {
        _CONTAINER_ADD(0, _LD_STEP_DEL_, i, 0);
    }

    // "x" index;
    for (auto x = columnStart; x <= vrLen; x++) {
        column[0] = x;
        auto lastDiagonal = x - columnStart;
         _CONTAINER_ADD(0, _LD_STEP_ADD_, 0, x);
        for (auto y = columnStart; y <= vlLen; y++) {
            auto oldDiagonal = column[y];
            auto possibilities = {
                lastDiagonal + (vl[y - 1] == vr[x - 1] ? 0 : 1),  // diagonal (default);
                column[y] + 1,  // top;
                column[y - 1] + 1  // left;
            };
            auto pos = std::distance(
                possibilities.begin(),
                std::min_element(possibilities.begin(), possibilities.end()));

            column[y] = std::min(possibilities);
            lastDiagonal = oldDiagonal;

            LD_OPS op;
            int index;
            if (pos == 0) {
                op = _LD_STEP_REP_;
                index = counter - vlLen - 1;
            } else if (pos == 1) {
                op = _LD_STEP_ADD_;
                index = counter - vlLen;
            } else {
                op = _LD_STEP_DEL_;
                index = counter;
            }

            _CONTAINER_ADD(index, op, y, x);
        }
    }

    // auto result = column[vlLen];

    // back recursion to find the path;
    std::function<void(int)> _BACK_RECURSION =
        [&_BACK_RECURSION, &container, &movementResult](int i) mutable -> void {
            auto e = container.at(i);
            auto p = e.find("parent")->second;
            if (p != -1) {
                movementResult.push_back(e);
                _BACK_RECURSION(p);
            }
        };
    _BACK_RECURSION(container.size() - 1);

    return movementResult;
}

void sharpen_core::Util::applyLDResult(
    std::vector<std::string> &vl,
    std::vector<std::string> &vr,
    LDMovementResult &result) {
     for (auto e : result) {
        auto op = e.find("op")->second, x = e.find("x")->second, y = e.find("y")->second;
        if (op == _LD_STEP_DEL_) {
            if (y == 0) {
                vl.erase(vl.begin(), vl.begin() + (x - 1));
            } else {
                vl.erase(vl.begin() + (x - 1));
            }
        } else if (op == _LD_STEP_REP_) {
            vl[x - 1] = vr[y - 1];
        } else if (op == _LD_STEP_ADD_) {
            if (y == 0) {
                vl.insert(vl.begin(), vr.begin(), vr.begin() + (y - 1));
            } else {
                vl.insert(vl.begin() + x, vr[y - 1]);
            }
        }
    }
}
