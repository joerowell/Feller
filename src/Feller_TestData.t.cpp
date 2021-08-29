#include "Feller_TestData.hpp"
#include "gtest/gtest.h"

TEST(TestData, testEquality)
{
  Feller::TestData d1;
  Feller::TestData d2;
  ASSERT_EQ(d1, d2);
}

TEST(TestData, testToString)
{
  Feller::TestData d1;
  ASSERT_EQ(d1.to_string(), "None");
}
