#include <string>
#include "gtest/gtest.h"
#include "lib/core/core_util.h"

using sharpen_core::Util;

TEST(Sharpen, LD) {
    // test LD algorithm;
    std::vector<std::string> v1{"d", "e", "m", "o", "c", "r", "a", "t"};
    std::vector<std::string> v2{"r", "e", "p", "u", "b", "l", "i", "c", "a", "n"};
    auto diff = Util::findLevenshteinDistancePath(v1, v2);
    Util::applyLDResult(v1, v2, diff);
    EXPECT_EQ(v1, v2);
}
