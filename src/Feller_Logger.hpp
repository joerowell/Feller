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
#ifndef INCLUDED_TREE_LOGGER
#define INCLUDED_TREE_LOGGER

#include "Feller_Feller.hpp"

namespace Feller
{

/**
   Logger. This is the primary template class for the Feller library.
   This class can be viewed as providing a generic interface that allows
   specifying a few different aspects of how the class works and how it deals
   with it's own internals. Note that whilst the parameters are named as
 policies, this class is not a traditional implementation of policy-based design
 as it does not inherit from its template parameters. Instead, this class
 operates by composition.

   \tparam LogType. This parameter describes what type of log to be used in this
 logger. Since there are potentially a variety of events that can occur in a
 system, it does not necessarily make sense to prescribe a single log type for
 many scenarios. To fix this problem, we provide the ability to specify this
 depending on the use case. An example policy for this can be found in
 EventLog.hpp.

   \tparam StoragePolicy. This parameter describes how the logs are stored
 inside this class. The reason for this specification is because it may be
 reasonable (useful, even) to optimise different storage layouts for distinct
 scenarios. For example, one may choose to have an unordered map if lookup speed
 is vital: alternatively, it may be desirable or useful to provide contiguous
   data access. We provide example policies for this in (for example)
 ContiguousLogStorage.hpp.

   \tparam LockPolicy. This parameter describes how the Logger deals with
 concurrency. Since it may be desirable to share this logger across many
 threads, we provide the ability to configure how the log store is locked
 depending on this template parameter. This has the additional benefit of
 allowing no locking to take place in single-threaded situations. An example
 locking policy can be found in both NoLock.hpp and MutexLock.hpp. Note that the
 use of this policy does not prevent further inner locking. For example, either
 the StoragePolicy or the LogType may possibly implement their own locking
 schemes, and these may or may not interfere with this locking policy. This
 locking policy solely defines protections for the outer access to the log
 store.
 **/

template <typename LogType, template <typename> class StoragePolicy, typename LockPolicy>
class Logger
{
private:
  /** m_store. This member variable contains all of the records that have been
  passed into this logger. This class solely acts as a wrapper around this
  store: most of the real logic occurs inside the store.
  **/
  StoragePolicy<LogType> m_store{};

  /** m_lock. This member variable simply instantiates the locking policy for
  this class. The reason for instantiating this as a member is so that the
  lifetime of the lock is directly tied to the object, as well as not providing
  external access to the lock.
  **/
  LockPolicy m_lock{};

public:
  /**
     log_type. This type exposes the LogType parameter to the outside world.
     This may be useful for other constructions.
  **/
  using log_type = LogType;

  /**
     storage_policy. This type exposes the StoragePolicy parameter to the
  outside world. This may be useful for other constructions.
  **/
  using storage_policy = StoragePolicy<LogType>;

  /**
     lock_policy. This type exposes the LockPolicy parameter to the outside
  world. This may be useful for other constructions.
  **/
  using lock_type = LockPolicy;

  /** size_type. This type is used to represent the return type for size methods
  in the m_store method. This can equally be obtained by taking the type of the
  StoragePolicy externally.
  **/
  using size_type = typename StoragePolicy<LogType>::size_type;
  /** const_iterator. This type is used to represent the iterators in the store
  of this object. We allow direct access to the store via these iterators, but
  only for read access. In other words, we do not provide non-const iterators
  here. This is because non-const iterators may allow violating the locking
  policy (intentionally or otherwise).
  **/
  using const_iterator = typename StoragePolicy<LogType>::const_iterator;

  // INLINE ACCESSORS

  /** cbegin. This function returns a const iterator to the first element of the
      store. This function does not throw and does not allow
      modifications to this object. This function is undefined if invoked on an
      invalid object.
      \return a const iterator to the first element of the log store.      .
  **/
  inline const_iterator cbegin() const noexcept;

  /** cend.
      This function returns a const iterator to the end of the parameters
      vector. Note that in keeping with the C++ standard library this is, in
  fact, one after the last valid element. \return a const iterator to the end
  element of the parameter vector.
  **/
  inline const_iterator cend() const noexcept;

  /** size.
      This function returns the number of elements in the store.
      This function is well-defined when called on a valid object. This
      function does not throw and does not allow direct modifications.
      @return the number of elements in the store.
  **/
  inline size_type size() const noexcept;

  /**
     capacity. This method returns the capacity of the store as a
     variable of type ``size_type``. This method does not throw and does not
  modify this object. \return: the capacity of the store.
  **/
  inline size_type capacity() const noexcept;

  /**
     insert. This method inserts a `log` into the store.
     Note that this method may throw due to std::bad_alloc,
     and this function will modify this object.
     \param log: the log to be moved into the store.
  **/
  void insert(LogType &&log);
  /**
     insert. This method copies the `log` parameter into the store.
     Note that this method may throw due to std::bad_alloc,
     and this function will modify this object.
     \param log: the log to be copied into the store
   **/
  void insert(const LogType &log);

  /**
     operator<<. Prints a string representation of this object to the
     specified Ostream ``os`. This method may throw.
     \tparam Ostream: the type of stream. This type must implement <<.
     \return the os parameter..
  **/
  template <typename Ostream> inline Ostream &operator<<(Ostream &os);

  /**
     clear. This method clears the store, resetting the size to 0.
     Note that this operation is not guaranteed to free the memory associated
  with the store. This method does not throw.
  **/
  inline void clear() noexcept;
};

/// INLINE FUNCTIONS

template <typename LogType, template <typename> class StoragePolicy, typename LockPolicy>
inline typename Feller::Logger<LogType, StoragePolicy, LockPolicy>::const_iterator
Feller::Logger<LogType, StoragePolicy, LockPolicy>::cbegin() const noexcept
{
  return m_store.cbegin();
}

template <typename LogType, template <typename> class StoragePolicy, typename LockPolicy>
inline typename Feller::Logger<LogType, StoragePolicy, LockPolicy>::const_iterator
Feller::Logger<LogType, StoragePolicy, LockPolicy>::cend() const noexcept
{
  return m_store.cend();
}

template <typename LogType, template <typename> class StoragePolicy, typename LockPolicy>
inline typename Feller::Logger<LogType, StoragePolicy, LockPolicy>::size_type
Feller::Logger<LogType, StoragePolicy, LockPolicy>::size() const noexcept
{
  return m_store.size();
}

template <typename LogType, template <typename> class StoragePolicy, typename LockPolicy>
inline typename Feller::Logger<LogType, StoragePolicy, LockPolicy>::size_type
Feller::Logger<LogType, StoragePolicy, LockPolicy>::capacity() const noexcept
{
  return m_store.capacity();
}

template <typename LogType, template <typename> class StoragePolicy, typename LockPolicy>
inline void Feller::Logger<LogType, StoragePolicy, LockPolicy>::insert(LogType &&log)
{
  typename LockPolicy::LockType lock(m_lock.getLock());
  m_store.insert(log);
}

template <typename LogType, template <typename> class StoragePolicy, typename LockPolicy>
inline void Feller::Logger<LogType, StoragePolicy, LockPolicy>::insert(const LogType &log)
{
  typename LockPolicy::LockType lock(m_lock.getLock());
  m_store.insert(log);
}

template <typename LogType, template <typename> class StoragePolicy, typename LockPolicy>
inline void Feller::Logger<LogType, StoragePolicy, LockPolicy>::clear() noexcept
{
  typename LockPolicy::LockType lock(m_lock.getLock());
  m_store.clear();
}

template <typename LogType, template <typename> class StoragePolicy, typename LockPolicy>
template <typename Ostream>
inline Ostream &Feller::Logger<LogType, StoragePolicy, LockPolicy>::operator<<(Ostream &os)
{
  // All of the state is kept in the store, so we'll just use that.
  os << m_store;
  return os;
}
}  // namespace Feller

#endif
