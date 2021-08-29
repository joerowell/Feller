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
