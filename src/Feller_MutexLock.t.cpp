/***\
*
*   Copyright (C) Joe Rowell
*
*   This file is part of Feller. Feller is free software:
*   you can redistribute it and/or modify it under the terms of the
*   GNU General Public License as published by the Free Software Foundation,
*   either version 2 of the License, or (at your option) any later version.
*
*   Feller is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with Feller. If not, see <http://www.gnu.org/licenses/>.
*
****/
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
