#include "gtest/gtest.h"
#include "lib/core/core_type_factory.h"

using sharpen_core::TypeFactory;

TEST(Sharpen, Type) {
  // basic type;
  EXPECT_EQ(1e5, TypeFactory::buildNumber(1e5)->getNativeData());
  EXPECT_EQ(-1e5, TypeFactory::buildNumber(-1e5)->getNativeData());
  EXPECT_EQ(19.53, TypeFactory::buildNumber(19.53)->getNativeData());
  EXPECT_EQ(-19.53, TypeFactory::buildNumber(-19.53)->getNativeData());
  EXPECT_TRUE(TypeFactory::buildBool(true)->getNativeData());
  EXPECT_FALSE(TypeFactory::buildBool(false)->getNativeData());
  EXPECT_STREQ("Sharpen", TypeFactory::buildString("Sharpen")->getNativeData().c_str());
  
  // advanced types;
  auto mapA = TypeFactory::buildMap();
  mapA->addItem("Number", TypeFactory::buildNumber(10));
  mapA->addItem("Bool", TypeFactory::buildBool(true));
  mapA->addItem("String", TypeFactory::buildString("YHSPY"));

  auto mapB = TypeFactory::buildMap();
  mapB->addItem("Number", TypeFactory::buildNumber(10));
  mapB->addItem("Bool", TypeFactory::buildBool(true));
  mapB->addItem("String", TypeFactory::buildString("YHSPY"));

  EXPECT_EQ(3, mapA->getSize());
  EXPECT_EQ(3, mapB->getSize());
  EXPECT_TRUE(TypeFactory::isEqual(mapA, mapB));

  auto arrayA = TypeFactory::buildArray();
  arrayA->addItem(TypeFactory::buildNumber(1e7));
  arrayA->addItem(TypeFactory::buildBool(false));
  arrayA->addItem(TypeFactory::buildString("Sharpen"));
  arrayA->addItem(mapA);

  auto arrayB = TypeFactory::buildArray();
  arrayB->addItem(TypeFactory::buildNumber(1e7));
  arrayB->addItem(TypeFactory::buildBool(false));
  arrayB->addItem(TypeFactory::buildString("Sharpen"));
  arrayB->addItem(mapB);

  EXPECT_EQ(4, arrayA->getSize());
  EXPECT_EQ(4, arrayB->getSize());
  EXPECT_TRUE(TypeFactory::isEqual(arrayA, arrayB));
}
