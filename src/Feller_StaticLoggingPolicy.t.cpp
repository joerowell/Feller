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

#include "Feller_StaticLoggingPolicy.hpp"
#include "Feller_LoggingMode.hpp"
#include "gtest/gtest.h"

TEST(StaticLoggingPolicy, testShouldLog)
{
  Feller::StaticLoggingPolicy<Feller::LoggingMode::EVERYTHING> ret_t;
  Feller::StaticLoggingPolicy<Feller::LoggingMode::NOTHING> ret_f;

  const auto size = static_cast<unsigned>(Feller::LoggingMode::SIZE);

  for (unsigned i = 0; i < size; i++)
  {
    const auto level = static_cast<Feller::LoggingMode>(i);
    EXPECT_EQ(ret_t.shouldLog(level), true);
    EXPECT_EQ(ret_f.shouldLog(level), false);
  }
}

TEST(StaticLoggingPolicy, testSwitchMode)
{
  Feller::StaticLoggingPolicy<Feller::LoggingMode::EVERYTHING> ret_t;
  Feller::StaticLoggingPolicy<Feller::LoggingMode::NOTHING> ret_f;

  ASSERT_EQ(ret_t.mode(), Feller::LoggingMode::EVERYTHING);
  ASSERT_EQ(ret_f.mode(), Feller::LoggingMode::NOTHING);

  const auto size = static_cast<unsigned>(Feller::LoggingMode::SIZE);

  for (unsigned i = 0; i < size; i++)
  {
    const auto level = static_cast<Feller::LoggingMode>(i);
    ret_t.switchMode(level);
    EXPECT_EQ(ret_t.mode(), Feller::LoggingMode::EVERYTHING);
    ret_f.switchMode(level);
    EXPECT_EQ(ret_f.mode(), Feller::LoggingMode::NOTHING);
  }
}
