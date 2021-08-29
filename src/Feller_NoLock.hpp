#ifndef INCLUDED_FELLER_NO_LOCK
#define INCLUDED_FELLER_NO_LOCK

#include "Feller_AnyType.hpp"
#include "Feller_Feller.hpp"

namespace Feller
{
/**
 \brief The purpose of this component is to provide a policy class that does
 not provide any locking mechanisms. In particular, this class is designed to
 enable concurrent operations to occur on a \ref Logger without any guarantees
of thread safety.
**/

/**
 NoLock. This class provides a policy class that does not provide any locking
 mechanisms. In particular, this class satisfies the Lock requirements: we
define an alias for the LockType which refers to the \ref AnyType, and simply
return a void from the getLock method.

 This class should be used as a policy class for a \ref Logger that does not
 require any particular notion of thread safety. This may be useful for a
 single-threaded \ref Logger.
**/
class NoLock
{
private:
  // EMPTY: this class has nothing useful.
public:
  /**
     LockType. This defines the type of lock provided by this policy class.
     Here, the type is simply the AnyType.
  **/
  using LockType = Feller::AnyType;
  /**
     getLock. This method returns the lock associated with this policy class.
     This method returns nothing, as there is no associated lock for this class.
     However, to ensure this can be used in a variety of contexts, we actually
  just return 0 from this function.
     @return 0.
  **/
  inline constexpr unsigned char getLock() const noexcept;
};
// INLINE FUNCTIONS
inline constexpr unsigned char NoLock::getLock() const noexcept { return 0; }

}  // namespace Feller
#endif
