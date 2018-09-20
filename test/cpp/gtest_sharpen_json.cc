#include <string>
#include "gtest/gtest.h"
#include "lib/core/core_type_factory.h"
#include "lib/parser/json.h"

using sharpen_parser::RSJresource;
using sharpen_core::Number;
using sharpen_core::Bool;
using sharpen_core::String;

TEST(Sharpen, Json) {
    // test "Json" parser;
    std::string json = "{\"id\":1,\"name\":\"YHSPY\",\"winner\":true}";
    auto o = std::make_shared<RSJresource>(json)->parseAll()->as<Map>();

    EXPECT_STREQ("1", o->getValue("id")->as<String>()->getNativeData().c_str());
    EXPECT_STREQ("true", o->getValue("winner")->as<String>()->getNativeData().c_str());
    EXPECT_STREQ("YHSPY", o->getValue("name")->as<String>()->getNativeData().c_str());
}   
