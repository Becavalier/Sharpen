#include "gtest/gtest.h"
#include "lib/core/core_type_factory.h"

using sharpen_core::TypeFactory;

TEST(SharpenTypeSystem, Basic) {
  EXPECT_EQ(10, TypeFactory::buildNumber(10)->getNativeData());
}