#include "Feller_MutexLock.hpp"
#include "gtest/gtest.h"

TEST(Lock, testInit)
{
  Feller::MutexLock l{};
  // Note: any failure here may be explainable by the behaviour of
  // std::mutex::try_lock, since the function may fail spuriously.
  EXPECT_EQ(l.getLock().try_lock(), true);
}

TEST(Lock, getLock)
{
  Feller::MutexLock l{};
  auto &lock1 = l.getLock();
  auto &lock2 = l.getLock();
  ASSERT_EQ(&lock1, &lock2);
}
