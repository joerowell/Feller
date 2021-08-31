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
#ifndef INCLUDED_FELLER_LOGGER
#define INCLUDED_FELLER_LOGGER

#include "Feller_Feller.hpp"
#include "Feller_LoggingMode.hpp"

namespace Feller
{

/**
   Logger. This is the primary template class for the Feller library.
   This class can be viewed as providing a generic interface that allows
   specifying a few different aspects of how the class works and how it deals
   with it's own internals. Note that whilst the parameters are named as
 policies, this class is not a traditional implementation of policy-based design
 as it does not inherit from its all of its template parameters. Instead, this
 class accepts the first parameter as an argument for the other template parameters and inherits
 from all of the rest.

   \tparam LogType. This parameter describes what type of log to be used in this
 logger. Since there are potentially a variety of events that can occur in a
 system, it does not necessarily make sense to prescribe a single log type for
 many scenarios. To fix this problem, we provide the ability to specify this
 depending on the use case. An example policy for this can be found in
 Feller_EventLog.hpp.

   \tparam KeyType. This parameter describes a key if the log is written as a key/value
   pair. For example, if the storage policy is an unordered-map, then this parameter
   acts as the key type for that map. By default this is set to be a char type, as
   this is not necessarily always used.

   \tparam StoragePolicy. This parameter describes how the logs are stored
 inside this class. The reason for this specification is because it may be
 reasonable (useful, even) to optimise different storage layouts for distinct
 scenarios. For example, one may choose to have an unordered map if lookup speed
 is vital: alternatively, it may be desirable or useful to provide contiguous
   data access. We provide example policies for this in (for example)
 Feller_ContiguousLogStorage.hpp. By default we allocate contiguously using the ContiguousLogStorage
 class.

   \tparam LockPolicy. This parameter describes how the Logger deals with
 concurrency. Since it may be desirable to share this logger across many
 threads, we provide the ability to configure how the log store is locked
 depending on this template parameter. This has the additional benefit of
 allowing no locking to take place in single-threaded situations. Example
 locking policies can be found in both Feller_NoLock.hpp and Feller_MutexLock.hpp. Note that the
 use of this policy does not prevent further inner locking. For example, either
 the StoragePolicy or the LogType may possibly implement their own locking
 schemes, and these may or may not interfere with this locking policy. This
 locking policy solely defines protections for the outer access to the log
 store. By defauly we using no locking via NoLock.

  \tparam LoggingPolicy. This parameter determines how the logger processes logs. Briefly,
  it is sometimes advantageous to log everything (i.e in situations where full monitoring is
 needed). Similarly, it is sometimes necessary to turn off all logging (due to e.g performance
 reasons). Since any sufficiently advanced logger needs to have a way of dealing with these
 decisions, this template parameter allows one to customise this detail. Example logging policies
 can be found in Feller_LogEverything.hpp and Feller_LogNothing.hpp. By default we use the
 ConditionalLoggingPolicy.
 **/

template <typename LogType, typename KeyType = char,
          template <typename, typename> class StoragePolicy = ContiguousLogStorage,
          typename LockPolicy = NoLock, typename LoggingPolicy = ConditionalLoggingPolicy>
class Logger : public StoragePolicy<LogType, KeyType>, public LockPolicy, public LoggingPolicy
{
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
  using storage_policy = StoragePolicy<LogType, KeyType>;

  /**
     lock_policy. This type exposes the LockPolicy parameter to the outside
  world. This may be useful for other constructions.
  **/
  using lock_type = LockPolicy;

  /**
     logging_type. This type exposes the LoggingPolicy parameter to the outside world.
     This may be useful for other constructions.
  **/
  using logging_type = LoggingPolicy;

  /** size_type. This type is used to represent the return type for size methods
  in the m_store method. This can equally be obtained by taking the type of the
  StoragePolicy externally.
  **/
  using size_type = typename StoragePolicy<LogType, KeyType>::size_type;
  /** const_iterator. This type is used to represent the iterators in the store
  of this object. We allow direct access to the store via these iterators, but
  only for read access. In other words, we do not provide non-const iterators
  here. This is because non-const iterators may allow violating the locking
  policy (intentionally or otherwise).
  **/
  using const_iterator = typename StoragePolicy<LogType, KeyType>::const_iterator;

  // INLINE MODIFIERS

  /**
     insert. This method inserts a `log` into the store.
     Note that this method may throw due to std::bad_alloc,
     and this function will modify this object.
     \param log: the log to be moved into the store.
     \param priority: the priority of the log. This determines whether the log will be inserted.
  **/
  inline void insert(LogType &&log, Feller::LoggingMode priority = Feller::LoggingMode::EVERYTHING);
  /**
     insert. This method copies the `log` parameter into the store.
     Note that this method may throw due to std::bad_alloc,
     and this function will modify this object.
     \param log: the log to be copied into the store
     \param priority: the priority of the log. This determines whether the log will be inserted.
   **/
  inline void insert(const LogType &log,
                     const Feller::LoggingMode priority = Feller::LoggingMode::EVERYTHING);

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

template <typename LogType, typename KeyType, template <typename, typename> class StoragePolicy,
          typename LockPolicy, typename LoggingPolicy>
inline void Feller::Logger<LogType, KeyType, StoragePolicy, LockPolicy, LoggingPolicy>::insert(
    LogType &&log, const Feller::LoggingMode priority)
{
  if (!this->shouldLog(priority))
    return;
  auto lock = this->getWorkingLock();
  StoragePolicy<LogType, KeyType>::insert(log);
}

template <typename LogType, typename KeyType, template <typename, typename> class StoragePolicy,
          typename LockPolicy, typename LoggingPolicy>
inline void Feller::Logger<LogType, KeyType, StoragePolicy, LockPolicy, LoggingPolicy>::insert(
    const LogType &log, const Feller::LoggingMode priority)
{
  if (!this->shouldLog(priority))
    return;
  auto lock = this->getWorkingLock();
  StoragePolicy<LogType, KeyType>::insert(log);
}
template <typename LogType, typename KeyType, template <typename, typename> class StoragePolicy,
          typename LockPolicy, typename LoggingPolicy>
inline void
Feller::Logger<LogType, KeyType, StoragePolicy, LockPolicy, LoggingPolicy>::clear() noexcept
{
  auto lock = this->getWorkingLock();
  this->clear();
}

}  // namespace Feller

#endif
