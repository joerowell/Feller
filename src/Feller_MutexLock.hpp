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
#ifndef INCLUDED_FELLER_MUTEX_LOCK
#define INCLUDED_FELLER_MUTEX_LOCK

#include <mutex>

#include "Feller_Feller.hpp"

namespace Feller
{

/**
 MutexLock. This policy class provides a simple, mutex based solution to
locking. Briefly, this class contains a std::mutex that represents a lock. This
mutex can be used externally to this class by calling the getLock method.
Further, this class defines a LockType that defines how this lock should be
used.

 As an example, let's suppose you wished to use the lock provided by this
class. This can be achieved using the following segment:

 MutexLock my_lock;
 MutexLock::LockType lock(my_lock.getLock());

 Since this class provides a mutex, the constructor for the LockType will
block until the mutex is no longer locked. This enables a rather primitive form
of mutual exclusion for the class using this policy.
**/
class MutexLock
{
private:
  /**
     lock. This is the mutex associated with this class.
     This mutex is owned exclusively by this class: this class will,
     however, distribute access to this lock via the ``getLock`` method.
  **/
  std::mutex lock{};

public:
  /**
     LockType. This defines the wrapper that should be used for the lock
  associated with this class. Given that lock is a std::mutex, then the
  LockType is a std::lock_guard<std::mutex>.
  **/
  using LockType = std::lock_guard<std::mutex>;

  /**
     getLock. This class returns a reference
     to the mutex associated with this Lock. This function cannot
     be const, since the mutex may be modified externally. This function
     does not throw. This function is well-defined for all well-formed locks.
     Note that this function does not block if the mutex is in use.
     \return the mutex contained in this Lock.
  **/
  inline std::mutex &getLock() noexcept;
};

/// INLINE FUNCTIONS
inline std::mutex &MutexLock::getLock() noexcept { return this->lock; }

}  // namespace Feller
#endif
