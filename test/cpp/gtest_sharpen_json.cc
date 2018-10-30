#include <string>
#include "gtest/gtest.h"
#include "lib/core/core_type_factory.h"
#include "lib/parser/json.h"

using sharpen_parser::Json;
using sharpen_core::Number;
using sharpen_core::Bool;
using sharpen_core::String;

TEST(Sharpen, Json) {
    // test "Json" parser;
    std::string json = u8R"({
        "id": 1,
        "name": "YHSPY",
        "female": false,
        "handsome": true 
    })";
    auto o = std::make_shared<Json>(json)->fastParse()->as<Map>();

    EXPECT_STREQ("1", o->getValue("id")->as<String>()->getNativeData().c_str());
    EXPECT_STREQ("YHSPY", o->getValue("name")->as<String>()->getNativeData().c_str());
    EXPECT_EQ(true, o->getValue("handsome")->as<Bool>()->getNativeData());
    EXPECT_EQ(false, o->getValue("female")->as<Bool>()->getNativeData());
}   
