#include <string>
#include "gtest/gtest.h"
#include "lib/core/core_util.h"

using sharpen_core::Util;

TEST(Sharpen, LD) {
    // test LD algorithm;
    std::vector<std::string> v1{"d"};
    std::vector<std::string> v2{"c", "r", "a", "t"};
    std::vector<std::string> v3{"r", "e", "p", "a", "n"};
    std::vector<std::string> v4{"r", "e", "p", "u", "b", "l", "i", "c", "a", "n"};
    std::vector<std::string> vt{"r", "e", "p", "u", "b", "l", "i", "c", "a", "n"};
    auto diff1 = Util::findLevenshteinDistancePath(v1, vt);
    auto diff2 = Util::findLevenshteinDistancePath(v2, vt);
    auto diff3 = Util::findLevenshteinDistancePath(v3, vt);
    auto diff4 = Util::findLevenshteinDistancePath(v4, vt);
    Util::applyLDResult(v1, vt, diff1);
    Util::applyLDResult(v2, vt, diff2);
    Util::applyLDResult(v3, vt, diff3);
    Util::applyLDResult(v4, vt, diff4);
    EXPECT_EQ(v1, vt);
    EXPECT_EQ(v2, vt);
    EXPECT_EQ(v3, vt);
    EXPECT_EQ(v4, vt);
}
