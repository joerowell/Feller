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
