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
#include "Feller_EventLog.hpp"
#include "Feller_TestData.hpp"
#include "gtest/gtest.h"

TEST(EventLog, testInit)
{
  Feller::EventLog l{};
  // Firstly, this log is empty: so no size should be set
  EXPECT_EQ(l.size(), 0);
  // Secondly we expect the current time to be ~ the one on insertion
  const auto is_gt = l.time() < std::chrono::system_clock::now();
  EXPECT_EQ(is_gt, true);
  // And, of course, we expect the name to be blank.
  EXPECT_EQ(l.name(), "");
  // And, naturally, this should not be a valid pointer.
  EXPECT_EQ(l.aux(), nullptr);
  // And, since the size is zero, this distance should be 0.
  EXPECT_EQ(std::distance(l.cbegin(), l.cend()), 0);
}

TEST(EventLog, testInitialEqual)
{
  Feller::EventLog l1{};
  const auto &l2 = l1;
  EXPECT_EQ(l1, l2);
  // Now check that these are equal when doing a copy construction
  auto l3 = l1;
  EXPECT_EQ(l3, l1);

  // And, of course, if these are equal if we set two default Data variables to
  // each m_aux
  l3.aux() = std::make_unique<Feller::TestData>();
  l1.aux() = std::make_unique<Feller::TestData>();
  EXPECT_EQ(l3, l1);

  // And now we need to check the copy constructor too
  Feller::EventLog l4{l1};
  EXPECT_EQ(l4, l1);
}

TEST(EventLog, testNotEqual)
{
  Feller::EventLog l1{};
  Feller::EventLog l2{};
  EXPECT_NE(l1, l2);
}

TEST(EventLog, testEmplaceBack)
{
  Feller::EventLog l1{};
  ASSERT_EQ(l1.size(), 0);

  const std::string key = "abc";
  const std::string val = "def";
  l1.emplace_back(key, val);
  std::pair<std::string, std::string> pari(key, val);
  EXPECT_EQ(*(l1.cbegin()), pari);
}

TEST(EventLog, testEmplaceBackPair)
{
  Feller::EventLog l1{};
  ASSERT_EQ(l1.size(), 0);

  const std::string key{"abc"};
  const std::string val{"def"};
  const std::pair<std::string, std::string> pair(key, val);
  l1.emplace_back(pair);
  EXPECT_EQ(l1.size(), 1);
  EXPECT_EQ(*(l1.cbegin()), pair);
}

TEST(EventLog, testCapacity)
{
  Feller::EventLog l1{};
  ASSERT_EQ(l1.size(), 0);
  ASSERT_EQ(l1.capacity(), 0);

  const std::string key   = "abc";
  const std::string value = "def";
  l1.emplace_back(key, value);
  ASSERT_EQ(l1.size(), 1);
  const bool gt = l1.capacity() >= 1;
  EXPECT_EQ(gt, true);
}

TEST(EventLog, testReserve)
{
  Feller::EventLog l1{};
  ASSERT_EQ(l1.size(), 0);
  ASSERT_EQ(l1.capacity(), 0);

  const auto cap = static_cast<unsigned>(rand()) % 4096;
  l1.reserve(cap);
  const bool gt = l1.capacity() >= 1;
  EXPECT_EQ(gt, true);
}

TEST(EventLog, testInitFromVars)
{
  Feller::EventLog l{"Test"};
  EXPECT_EQ(l.name(), "Test");
  EXPECT_NE(l, Feller::EventLog{});
}

TEST(EventLog, testToString)
{
  Feller::EventLog l{};
  std::ostringstream os1;
  os1 << l;
  EXPECT_EQ(os1.str(), l.to_string());

  // Now let's check it if we insert parameters
  l.emplace_back("abc", "def");
  std::ostringstream os2;
  os2 << l;
  EXPECT_NE(os1.str(), os2.str());
  EXPECT_EQ(os2.str(), l.to_string());

  // Now finally set the aux var
  l.aux() = std::make_unique<Feller::TestData>();
  std::ostringstream os3;
  os3 << l;
  // These should be exactly the same, since we return "None" by default
  // for both TestData and when there is no data.
  EXPECT_EQ(os3.str(), os2.str());
  EXPECT_NE(os3.str(), os1.str());
  EXPECT_EQ(os3.str(), l.to_string());
}

TEST(EventLog, testClear)
{
  // Here we reserve a large amount of space, insert a few strings
  // and then check that the capacity is reserved
  const auto size = static_cast<unsigned>(rand() % 4096);

  Feller::EventLog l1{};
  for (unsigned i = 0; i < size; i++)
  {
    l1.emplace_back("abc", "def");
  }

  EXPECT_EQ(l1.size(), size);

  // Now check that the capacity is at least bigger than the size.
  const auto capacity = l1.capacity();
  EXPECT_EQ(capacity >= l1.size(), true);

  // Now we've got that,  we clear and check the capacity is the same as before.
  l1.clear();
  EXPECT_EQ(l1.size(), 0);
  EXPECT_EQ(capacity, l1.capacity());
}

TEST(EventLog, testResize)
{
  // Here we reserve a large amount of space, insert a few strings
  // and then check that the capacity is reserved
  const auto size = static_cast<unsigned>(rand() % 4096);

  Feller::EventLog l1{};
  for (unsigned i = 0; i < size; i++)
  {
    l1.emplace_back("abc", "def");
  }

  EXPECT_EQ(l1.size(), size);
  l1.resize(0);
  EXPECT_EQ(l1.size(), 0);
}

TEST(EventLog, testResizeTo0)
{
  // Here we reserve a large amount of space, insert a few strings
  // and then check that the capacity is reserved
  const auto size = static_cast<unsigned>(rand() % 4096);

  Feller::EventLog l1{};
  for (unsigned i = 0; i < size; i++)
  {
    l1.emplace_back("abc", "def");
  }

  EXPECT_EQ(l1.size(), size);
  l1.set_zero();
  EXPECT_EQ(l1.size(), 0);
  EXPECT_EQ(l1.capacity(), 0);
}

TEST(EventLog, testConstructWithStringParams)
{
  Feller::EventLog l{"Abc", "def"};
  EXPECT_EQ(l.size(), 1);
  EXPECT_EQ(l.name(), "Inserted");
  // Wrap them as a pair
  // Yes, the name is intentional
  std::pair<std::string, std::string> pari(std::string("Abc"), std::string("def"));
  EXPECT_EQ(*(l.cbegin()), pari);
}
