#ifndef INCLUDED_FELLER_TEST_DATA
#define INCLUDED_FELLER_TEST_DATA

#include "Feller_Data.hpp"
#include "Feller_Feller.hpp"
#include <string>

namespace Feller
{

/**
 \brief TestData. This class defines a simple series of methods to enable
testing to be easier. This class itself also has a test file that enforces the
expected output of this class: note that this is not for the correctness of the
methods. Instead, these tests are defined to detect otherwise silent changes.
**/
struct TestData : public Data
{
  /**
     ~TestData. This is the destructor for this class.
     This destructor does nothing, as this class is empty.
  **/
  ~TestData() override;

  /**
     operator==. This method always returns true, since there's
     no obvious differences between different instantiations
     of this struct.
  **/
  bool operator==(const Data &) const noexcept;

  /**
     to_string. This method simply returns "None", since there is no data
     attached to this object.
  **/
  std::string to_string() const override;

  /**
     copy. This method returns a pointer to a copy of this object.
     Note that this method simply returns a new object pointed to
     by a unique ptr without any data.
     \return a unique ptr to a copy of this object.
  **/
  std::unique_ptr<Data> copy() const override;
};

// This class should be exactly the same size as a pointer
// to a vtable, since we expect there to be nothing else in this class.
static_assert(sizeof(TestData) == sizeof(int *));
}  // namespace Feller
#endif
