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

#include "Feller_ConditionalLoggingPolicy.hpp"
#include "Feller_LoggingMode.hpp"
#include "gtest/gtest.h"

TEST(ConditionalLoggingPolicy, testInit)
{
  Feller::ConditionalLoggingPolicy policy{};
  EXPECT_EQ(policy.mode(), Feller::LoggingMode::EVERYTHING);
}

TEST(ConditionalLoggingPolicy, testShouldLog)
{
  Feller::ConditionalLoggingPolicy policy{};
  const auto size = static_cast<unsigned>(Feller::LoggingMode::SIZE);

  for (unsigned i = 0; i < size; i++)
  {
    const auto level = static_cast<Feller::LoggingMode>(i);
    policy.switchMode(level);
    for (unsigned j = 0; j < size; j++)
    {
      EXPECT_EQ(policy.shouldLog(static_cast<Feller::LoggingMode>(j)), (i != 0 & j <= i));
    }
  }
}

TEST(ConditionalLoggingPolicy, testSwitchMode)
{
  Feller::ConditionalLoggingPolicy policy{};
  const auto size = static_cast<unsigned>(Feller::LoggingMode::SIZE);
  for (unsigned i = 0; i < size; i++)
  {
    const auto level = static_cast<Feller::LoggingMode>(i);
    policy.switchMode(level);
    EXPECT_EQ(policy.mode(), level);
  }
}
