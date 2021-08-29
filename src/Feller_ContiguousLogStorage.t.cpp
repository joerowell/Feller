#include "Feller_ContiguousLogStorage.hpp"
#include "gtest/gtest.h"

TEST(ContiguousLogStorage, testOstream)
{

  Feller::ContiguousLogStorage<int> log;
  const auto size = static_cast<unsigned>(rand()) % 4096;

  std::string curr;

  for (unsigned i = 0; i < size; i++)
  {
    const auto c = static_cast<unsigned>(rand());
    log.emplace_back(c);
    curr += std::to_string(c);
  }

  std::ostringstream os;
  os << log;
  EXPECT_EQ(os.str(), curr);
}

TEST(ContiguousLogStorage, testInsert)
{
  Feller::ContiguousLogStorage<int> log;
  ASSERT_EQ(log.size(), 0);

  auto c = static_cast<int>(rand());
  log.insert(c);
  EXPECT_EQ(log.size(), 1);
  EXPECT_EQ(log[0], c);
}

TEST(ContiguousLogStorage, testInsertMove)
{
  // Note; this uses string because int is trivially copiable (and thus
  // moving it has no effect).
  Feller::ContiguousLogStorage<std::string> log;
  ASSERT_EQ(log.size(), 0);
  std::string c = "abcdef";
  auto d        = c;
  log.insert(std::move(c));
  EXPECT_EQ(log.size(), 1);
  EXPECT_EQ(log[0], d);
}
