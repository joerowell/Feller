#include "Feller_TestData.hpp"

Feller::TestData::~TestData() = default;
bool Feller::TestData::operator==(const Data & /*other*/) const noexcept { return true; }
std::string Feller::TestData::to_string() const { return "None"; }
std::unique_ptr<Feller::Data> Feller::TestData::copy() const
{
  return std::make_unique<Feller::TestData>();
}
