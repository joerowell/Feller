#include "Feller_Logger.hpp"
#include "Feller_ContiguousLogStorage.hpp"
#include "Feller_EventLog.hpp"
#include "Feller_MutexLock.hpp"

#include "gtest/gtest.h"

// Note; this file defines a very particular type of
// Logger. The way to test this class is to first test each
// policy separately, and then test the logger in general. This
// separates the interface from the implementation.

using LoggerType =
    typename Feller::Logger<Feller::EventLog, Feller::ContiguousLogStorage, Feller::MutexLock>;

TEST(Logger, testInit)
{
  LoggerType l1{};
  EXPECT_EQ(l1.size(), 0);
  EXPECT_EQ(l1.capacity(), 0);
  EXPECT_EQ(l1.cbegin(), l1.cend());
}

TEST(Logger, testInsert)
{
  LoggerType l1{};
  ASSERT_EQ(l1.size(), 0);
  // First of all, check that we can insert a random Log.
  Feller::EventLog l{"Test"};
  l.emplace_back("abc", "def");
  l1.insert(l);
  EXPECT_EQ(l1.size(), 1);
  EXPECT_EQ(*(l1.cbegin()), l);
}
