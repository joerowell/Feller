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
#include "Feller_LogEverything.hpp"
#include "Feller_LogNothing.hpp"
#include "Feller_ConditionalLoggingPolicy.hpp"
#include "gtest/gtest.h"

// Note; this file defines a very particular type of
// Logger. The way to test this class is to first test each
// policy separately, and then test the logger in general. This
// separates the interface from the implementation.

using LoggerType = typename Feller::Logger<Feller::EventLog, char, Feller::ContiguousLogStorage,
                                           Feller::MutexLock, Feller::LogEverything>;

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
  std::cout << "inserting" << std::endl;
  l1.insert(l);
  EXPECT_EQ(l1.size(), 1);
  EXPECT_EQ(*(l1.cbegin()), l);
}

TEST(Logger, testInsertWithStaticGuard)
{
  Feller::Logger<Feller::EventLog, char, Feller::ContiguousLogStorage, Feller::MutexLock,
                 Feller::LogNothing>
      logger;
  Feller::EventLog l{"Test"};
  logger.insert(l, Feller::LoggingMode::EVERYTHING);
  EXPECT_EQ(l.size(), 0);
}

TEST(Logger, testInsertWithConditionalGuard)
{
  Feller::Logger<Feller::EventLog, char, Feller::ContiguousLogStorage, Feller::MutexLock,
                 Feller::ConditionalLoggingPolicy>
      logger;
  Feller::EventLog l{"Test"};

  const auto size    = static_cast<unsigned>(Feller::LoggingMode::SIZE);
  unsigned curr_size = 0;
  for (unsigned i = 0; i < size; i++)
  {
    logger.switchMode(static_cast<Feller::LoggingMode>(i));
    for (unsigned j = 0; j < size; j++)
    {
      logger.insert(l, static_cast<Feller::LoggingMode>(j));
      curr_size += static_cast<unsigned>((i != 0) && (j <= i));
      EXPECT_EQ(logger.size(), curr_size);
    }
  }
}
