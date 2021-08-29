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
#ifndef INCLUDED_TREE
#define INCLUDED_TREE

/**
   \brief
   Tree. This is the main namespace for the Tree library.
   This namespace simply forward declares all of the structs that are used
inside this project. For more information, see the top-level readme.
**/

namespace Tree
{

/**
 \brief The purpose of this component is to provide a placeholder type
 for template expansions. In particular, this class should be used to
 consume arbitrary input.
**/
struct AnyType;

/**
  \brief The purpose of this component is to provide a straightforward
  contiguous container for logs of a specified type. Use this if your
application would benefit in some sense from a contiguous representation of your
logs.
**/
template <typename LogType> class ContiguousLogStorage;

/**
 \brief The purpose of this component is to allow you to extend the amount of
 data that is collected in each log.
**/
struct Data;

/**
 \brief The purpose of this class is to enable you to test other classes using
 the \ref Data class without needing to encroach on other classes. Note that
this test case is solely defined to allow you to write simpler unit tests: it
should not replace proper integration tests.
**/
struct TestData;

/**
  \brief The purpose of this component is to provide a simple
  class that represents an event log. Each event log corresponds to a unique
event in the lifetime of the program which may or may not be useful. This
component should be used inside the lifespan of the wider logging system: while
it is a small component, it can only really derive any useful meaning from
broader use.
**/
class EventLog;

/**
 * \brief The purpose of this component is to provide a simple policy class that
 * represents a Lock. This policy class should be used for ensuring that the
 * underlying \ref Logger is thread safe.
 **/
struct MutexLock;

/**
 \brief The purpose of this component is to provide a policy class that does
 not provide any locking mechanisms. In particular, this class is designed to
 enable concurrent operations to occur on a \ref Logger without any guarantees
of thread safety.
**/
struct NoLock;

/** \brief The purpose of this component is to provide a generic logging
  facility for your application. This component can be viewed as the primary
  entry facility for the Tree logging package. Note that this component
  primarily combines other, smaller classes to create a cohesive whole.
**/
template <typename LogType, template <typename> class StoragePolicy, typename LockPolicy>
class Logger;
/**
 \brief The purpose of this namespace is to contain any utility functions that
are associated with the Tree project. Note that this component should not depend
on any other files used in this project other than standard system headers.
**/
namespace Util
{
}
}  // namespace Tree

#endif
#ifndef INCLUDED_ANY_TYPE
#define INCLUDED_ANY_TYPE

#include "Tree_Tree.hpp"

namespace Tree
{
/**
   \brief The purpose of this component is to provide a placeholder type
for template expansions. In particular, this class should be used to
consume arbitrary input.
**/
/**
   AnyType. This is a simple class placeholder. This class
   simply accepts any number of arguments and does nothing with them.
   This is primarily defined so that we may use it when declaring useful
   policy classes later on.
**/
struct AnyType
{
  /**
     AnyType. This is the default constructor for this class. This constructor
     accepts a non-zero number of parameters and does nothing with them.
  **/
  AnyType(...) {}
  /**
     AnyType. This constructor accepts no parameters and does nothing.
  **/
  AnyType() {}
};
}  // namespace Tree
#endif
#include "Tree_AnyType.hpp"
#ifndef INCLUDED_TREE_CONTIGUOUS_LOG_STORAGE
#define INCLUDED_TREE_CONTIGUOUS_LOG_STORAGE

#include <sstream>
#include <string>
#include <vector>

#include "Tree_Tree.hpp"

namespace Tree
{
/**
 ContiguousLogStorage. This class implements a contiguous container that stores
particular logs. This type stores all data contiguously: more specifically, for
some log type LogT, this class inherits from std::vector<LogT> and provides an
additional friend function for serialisation. Note that this class publicly
inherits from std::vector so that maximal functionality can exist for clients of
this particular class.

 \tparam LogType: the type of log to be stored in this class.
**/

template <typename LogType> class ContiguousLogStorage : public std::vector<LogType>
{
public:
  /**
      operator<<. Prints a string representation of the object ``st`` to the
      specified ostream ``os`. This method may throw.
      \tparam LT: the type stored in st.
      \param os: the stream to print the storage object to.
      \param st: the object to be printed.
      \return the os parameter.
   **/
  template <typename LT>
  inline friend std::ostream &operator<<(std::ostream &os, const ContiguousLogStorage<LT> &st);
  /**
     insert. This method implements a wrapper around emplace_back. In
  particular, this function copies the `log` into this object. This function is
  required for use inside other classes, such as \ref Logger. This function may
  throw, is not const, and is well-defined provided that both `log` and `this`
  are well-defined objects. \param log: the log to be copied into this object.
  **/
  inline void insert(const LogType &log);

  /**
     insert. This method implements a wrapper around emplace_back. In
  particular, this function moves the `log` into this object. This function is
  required for use inside other classes, such as \ref Logger. This function may
  throw, is not const, and is well-defined provided that both `log` and `this`
  are well-defined objects. \param log: the log to be copied into this object.
  **/
  inline void insert(LogType &&log);
};
/// INLINE FUNCTIONS
template <typename LogType>
inline std::ostream &operator<<(std::ostream &os, const ContiguousLogStorage<LogType> &st)
{
  for (auto &v : st)
  {
    os << v;
  }
  return os;
}

template <typename LogType>
inline void Tree::ContiguousLogStorage<LogType>::insert(const LogType &log)
{
  this->emplace_back(log);
}

template <typename LogType> inline void Tree::ContiguousLogStorage<LogType>::insert(LogType &&log)
{
  this->emplace_back(std::move(log));
}

}  // namespace Tree

#endif
#include "Tree_ContiguousLogStorage.hpp"
#ifndef INCLUDED_TREE_DATA
#define INCLUDED_TREE_DATA

#include <memory>

#include "Tree_Tree.hpp"

namespace Tree
{

/**
 \brief The purpose of this component is to allow you to extend the amount of
 data that is collected in each log.
**/

/**
 Data. The purpose of this structure is to act as a parent class for
 each of the various data types that are used in the Logger.

 Briefly, there may be some circumstances in which you want to track more data
than the Log class supports natively. Since this is a very legitimate use case
in a distributed application, we provide this class to extend functionality in a
type-safe fashion: if you simply inherit from this type and instantiate the
correct methods, then you can place the associated data inside a log. This class
itself merely declares some useful functions that any child type must
instantiate in order to be considered as a type of `Data`.

 This class does not define any implementation of its methods. A very minimal
 implementation of this class can be found in TestData.hpp
**/

struct Data
{
  /**
     ~Data(). This is a forward declaration that enforces that any child class
     creates an appropriate virtual destructor.
     The default version of this function does nothing.
  **/
  virtual ~Data();

  /**
     operator==. This method compares `this` object with the parameter `other`
  for equality. Note that this operation should be defined semantically: in
  other words, each child type should implement value equality. Note that this
  operator may not modify `this`, and it may not throw.

     @param other: the object to be compared with `this`.
     @return true if the objects are semantically equal, false otherwise.
  **/
  virtual bool operator==(const Data &other) const noexcept = 0;
  /**
     to_string. This method produces a string representation of the data
  associated with `this` object. This method may not modify `this` object. This
  method may throw due to allocation failures.
     @return the string representation of this object.
  **/
  virtual std::string to_string() const = 0;

  /**
     copy. This method returns a std::unique_ptr to a copy of this object.
     This is primarily useful when copying the log class between functions if
  this is ever necessary. Note that this function does not modify this class.
     @return a unique ptr to a copy of this object.
  **/
  virtual std::unique_ptr<Data> copy() const = 0;
};
}  // namespace Tree
#endif
#include "Tree_Data.hpp"
Tree::Data::~Data() = default;
#ifndef INCLUDED_TREE_EVENT_LOG
#define INCLUDED_TREE_EVENT_LOG

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "Tree_Tree.hpp"

#include "Tree_Data.hpp"
#include "Tree_Util.hpp"

namespace Tree
{

/**
    \brief EventLog. This class represents a simple event log.
  Each event log corresponds to an event in the system that has been traced.
  Each event log holds a time (i.e when the event
happened) and some auxiliary data that may or may not be useful (i.e such as the
name of the function that was entered, and the string representation of the
parameters that were passed in). For broad extensibility, we also provide a
pointer to extra \ref Data that may be used (and set externally) in situations
where the basic level of provision given by this class is not sufficient for the
use case. This class provides no direct internal member access. In particular,
while this class exposes constant iterators to the underlying parameter vector,
this class does not provide non-constant iterators. Similarly, this class does
not allow direct access to its internals: these should instead be indirectly
accessed via the provided member functions.
**/
class EventLog
{
  /** m_name. This variable corresponds to the name of the event log.
      The typical usage of this is to give some information about the
      event log. For example, if you entered a function f(),
      you may wish to set the name of the log as "entered f".
  **/
  std::string m_name{};
  /** m_time. This variable corresponds to the time the event log was created.
      The typical use of this variable is to provide some sort of chronology to
      a series of logs. This variable is not externally modifiable.
  **/
  std::chrono::time_point<std::chrono::system_clock> m_time{std::chrono::system_clock::now()};

  /** m_parameters. This variable is used to hold string representations
      of additional data. This variable can be used to represent arguments to a
      function: for example, you may wish to represent the particular value of
  an argument upon entering a function.
  **/
  std::vector<std::pair<std::string, std::string>> m_parameters{};

  /** m_aux.
      In some situations the basic nature of this class may not be enough to
  encode the right level of information. For these cases, we provide a simple
  pointer that can be used to attach auxiliary information. This event log owns
  the auxiliary data: the data is deleted upon destruction of the event log.
  **/
  std::unique_ptr<Data> m_aux{nullptr};

public:
  // Expose the size type for this object too.
  // Here we just expose the size type for the underlying vector of parameters.

  /** size_type. This specifies the return type for the size method.
      Since the size method solely relates to the m_parameters variable,
      this type is the same type returned by m_parameters.size().
  **/
  using size_type = decltype(m_parameters)::size_type;

  /**
     const_iterator. This specifies the constant iterator type that is exposed
     by this class. Since this class only exposes iterators to the m_parameters
     variable, we simply re-use the iterators from that variable.
  **/
  using const_iterator = decltype(m_parameters)::const_iterator;

  // GETTERS

  /** name. This function returns a const reference to the name of this event
  log. The behaviour of this function is undefined if the object is not formed
  correctly. This function does not throw and the access given does not allow
  one to modify the name of this event log.
      \return a const string reference to the name of this event log.
  **/
  inline const std::string &name() const noexcept;

  /** cbegin. This function returns a const iterator to the first element of the
      parameters vector. This function does not throw and does not allow
  modifications to this object. This function is undefined if invoked on an
  invalid object.
      \return a const iterator to the first element of the parameter vector.
  **/
  inline const_iterator cbegin() const noexcept;

  /** cend. This function returns a const iterator to the end of the parameters
  vector. Note that in keeping with the C++ standard library this is, in fact,
  one after the last valid element.
      \return a const iterator to the end element of the parameter vector.
  **/
  inline const_iterator cend() const noexcept;

  /** size. This function returns the number of elements in the parameters
  vector. This function is well-defined when called on a valid object. This
  function does not throw and does not allow direct modifications.
      \return the number of elements in the parameter vector.
  **/
  inline size_type size() const noexcept;

  /** aux. This function returns a reference to the m_aux pointer.
      Note that this function
      does not transfer membership from this event log. In other words, if the
  event log is deleted then the data referred to by aux will also be deleted.
  This function will either return a valid pointer (if the data member has been
  initialised) and will return nullptr otherwise. This function does not throw,
  but it is not const as modifications are permitted via the returned reference.
      \return a copy of the m_aux member variable.
  **/
  inline std::unique_ptr<Tree::Data> &aux() noexcept;

  /** time. This function returns the time that this event log was created.
      This function creates a copy of the internal member and returns it by
  value: as a result, this function does not modify the internal state of this
  object, and modifications cannot be carried out via this function. This
  function does not throw and does not modify this method. It is well-defined if
  the object has been initialised and undefined otherwise.
      \return the time this event log was created.
  **/
  inline std::chrono::time_point<std::chrono::system_clock> time() const noexcept;

  // Friend functions.
  // These operators are defined as friends to prevent the compiler
  // from instantiating a huge number of overloads.

  /**
     ==. This function implements an equality operator for EventLog.
     Semantically speaking, two event logs are equal if they were created at the
  same time, with the same name, containing the same parameters and the same
  data object. Put differently, two event logs compare equal if one can be
  considered as a copy of the other. This method does not throw and does not
  modify either argument. The behaviour of this function is well-defined if the
  event logs are valid objects and undefined otherwise. \return true if the
  objects are equal and false otherwise.
  **/
  inline friend bool operator==(const EventLog &lhs, const EventLog &rhs) noexcept;
  /**
     !=. This function implements an inequality operator for EventLog.
     Semantically speaking, two event logs are not equal if any of their
  parameters differ in value. Put differently, if `lhs` cannot be considered as
  a copy of `rhs` (and vice-versa) then `lhs` != `rhs`. This method does not
  throw and does not modify either argument. The behaviour of this function is
  well-defined if the event logs are valid objects and undefined otherwise.
     \return true if the objects are not equal and false otherwise.
  **/
  inline friend bool operator!=(const EventLog &lhs, const EventLog &rhs) noexcept;
  /**
     operator<<. Prints a string representation of the object ``lhs``
     to the specified ostream. This method may throw (as it does I/O).
     \param os: the output stream to which this object is printed.
     \param log: the log to be written to the ostream.
     \return a reference to the input ostream parameter.
  **/

  inline friend std::ostream &operator<<(std::ostream &os, const EventLog &log);

  // CONSTRUCTORS

  /**
     EventLog(). This is the default constructor for this class. Here we simply
     allow the compiler to automatically generate the right constructor for us.
  **/
  EventLog() = default;

  /**
     operator=. This implements the copy assignment operator. Here we simply
  allow the compiler to automatically generate the right operator for us.
     \param other: the log that is to be copied.
  **/
  EventLog &operator=(const EventLog &other) = default;

  /**
     EventLog(const EventLog& other). This is a copy constructor for another
  EventLog object. This constructor uses the automatic copy methods for the
  value member variables (e.g m_name) and copies the pointer to data manually.
  If the pointer `other.m_aux` is the nullptr, then this constructor will return
  a Log with m_aux = nullptr. Else, this constructor will deep copy the Data
  object pointed to by `other.m_aux` and store the pointer in this object's
  `m_aux`. This method may throw, but it does not modify the passed in
  parameter. \param other: the event log that is to be copied.
  **/
  EventLog(const EventLog &other)
      : m_name{other.m_name}, m_time{other.m_time}, m_parameters{other.m_parameters}, m_aux{nullptr}
  {
    // We must first check that we can safely dereference here.
    if (other.m_aux)
    {
      m_aux = other.m_aux->copy();
    }
  }

  /**
     EventLog. These constructors set the name field.
     This constructor should be most useful when creating event
     logs with human-readable names. \param name: the name of this log.
  **/
  EventLog(const std::string &name) : m_name{name}, m_time{}, m_parameters{}, m_aux{nullptr} {}
  EventLog(std::string name) : m_name{name}, m_parameters{}, m_time{}, m_aux{nullptr} {}
  EventLog(const char *const name) : m_name{name}, m_parameters{}, m_time{}, m_aux{nullptr} {}

  EventLog(const std::string &key, const std::string &value)
      : m_name{"Inserted"}, m_parameters{}, m_time{}, m_aux{nullptr}
  {
    m_parameters.emplace_back(key, value);
  }

  EventLog(std::string key, std::string value)
      : m_name{"Inserted"}, m_parameters{}, m_time{}, m_aux{nullptr}
  {
    m_parameters.emplace_back(std::move(key), std::move(value));
  }

  EventLog(const char *const key, const char *const value)
      : m_name{"Inserted"}, m_parameters{}, m_time{}, m_aux{nullptr}
  {
    m_parameters.emplace_back(key, value);
  }

  EventLog(const char *const key, const std::string value) : EventLog(key, value.c_str()) {}
  EventLog(const std::string value, const char *const key) : EventLog(value.c_str(), key) {}

  // MANIPULATORS

  /**
     emplace_back.
     This method accepts a key/value pair (`key`,`value`) and places a copy of
  this pair into the back of the parameter vector. This method may throw an
  exception in the case of memory being depleted. Note that this method does not
  pre-reserve any memory and, as a result, it should ideally be used after a
  corresponding call to ``reserve``.
     \param key: the string to be inserted into the back of the event log,
  representing a key.
     \param value: the string to be inserted into the back of the event log,
  representing a value.
  **/
  void emplace_back(const std::string &key, const std::string &value);

  /// Overload of emplace back for rvalue refs.
  void emplace_back(std::string &&key, std::string &&value);

  /**
     emplace_back. This method accepts a pair of strings (wrapped in
  std::string) and places a copy of this pair into the back of the parameter
  vector. This method may throw an exception in the case of memory being
  depleted. Note that this method does not pre-reserve any memory and, as a
  result, it should ideally be used after a corresponding call to ``reserve``.
     \param value: the pair to be inserted into the back of the event log,
  representing a key/value pair.
  **/
  void emplace_back(const std::pair<std::string, std::string> &value);

  /// Overload of emplace_back for rvalue ref.
  void emplace_back(std::pair<std::string, std::string> &&value);

  /**
     reserve.
     This method accepts a ``size`` variable of type ``size_type`` and reserves
  at least
     ``size`` many elements in the parameter vector.
     This method should be used in situations where speed is important
     and where the number of parameters needed is known ahead of time.
     \param size: the minimum number of elements to be reserved.
  **/
  void reserve(const size_type size);

  /**
     capacity. This method returns the capacity of the parameter vector as a
  variable of type
     ``size_type``. This method does not throw and does not modify this object.
     \return: the capacity of the `param` vector.
  **/
  size_type capacity() const noexcept;

  /**
     clear. This method clears the underlying parameter vector. This function
  follows the same contract as std::vector: in particular, whilst it clears the
  memory associated with the event log, it does not free any memory.
  **/
  void clear();

  /**
     resize. This method resizes the parameter vector to contain `size` many
  elements. This function follows the  same contract as std::vector and as a
  result this function may also change the capacity of the parameter vector.
  This is not guaranteed. \param size: the new size of the parameter vector.
  **/
  void resize(const size_type size);

  /**
     set_zero. This method frees the memory associated with the parameter vector
  by swapping it's memory with a zero-sized vector. Use this function if you
  want to guaranteed freeing while also not preserving existing allocations.
  **/
  void set_zero();

  // UTILITY
  /**
     to_string. Produces a string representation of this event log.
     The format of this string is as follows:
     (NAME, time) \n
     (All key/value pairs) \n
     (string representation of data) \n
     This function may throw, but it does not modify the current object.
     \return a string representing this object.
  **/
  std::string to_string() const;
};

//// INLINE FUNCTIONS

inline const std::string &EventLog::name() const noexcept { return m_name; }
inline std::chrono::time_point<std::chrono::system_clock> EventLog::time() const noexcept
{
  return m_time;
}
inline EventLog::size_type EventLog::size() const noexcept { return m_parameters.size(); }
inline std::unique_ptr<Data> &EventLog::aux() noexcept { return m_aux; }
inline EventLog::const_iterator EventLog::cbegin() const noexcept { return m_parameters.cbegin(); }
inline EventLog::const_iterator EventLog::cend() const noexcept { return m_parameters.cend(); }

inline bool operator==(const EventLog &lhs, const EventLog &rhs) noexcept
{
  const bool primitive =
      lhs.m_name == rhs.m_name && lhs.m_time == rhs.m_time && lhs.m_parameters == rhs.m_parameters;

  if (lhs.m_aux == nullptr || rhs.m_aux == nullptr)
  {
    return primitive & (lhs.m_aux == rhs.m_aux);
  }

  return primitive & (*(lhs.m_aux) == *(rhs.m_aux));
}

inline bool operator!=(const EventLog &lhs, const EventLog &rhs) noexcept { return !(lhs == rhs); }

inline std::ostream &operator<<(std::ostream &os, const EventLog &lhs)
{
  os << lhs.to_string();
  return os;
}

}  // namespace Tree
#endif
#include "Tree_EventLog.hpp"

void Tree::EventLog::clear() { m_parameters.clear(); }

void Tree::EventLog::resize(const Tree::EventLog::size_type size) { m_parameters.resize(size); }

void Tree::EventLog::set_zero()
{
  using vec_type = decltype(m_parameters);
  vec_type().swap(m_parameters);
}

void Tree::EventLog::emplace_back(const std::string &key, const std::string &value)
{
  emplace_back(std::pair<std::string, std::string>(key, value));
}

void Tree::EventLog::emplace_back(const std::pair<std::string, std::string> &value)
{
  m_parameters.emplace_back(value);
}

void Tree::EventLog::emplace_back(std::pair<std::string, std::string> &&value)
{
  m_parameters.emplace_back(std::move(value));
}

void Tree::EventLog::emplace_back(std::string &&key, std::string &&value)
{
  emplace_back(std::pair<std::string, std::string>(std::move(key), std::move(value)));
}

void Tree::EventLog::reserve(const Tree::EventLog::size_type size) { m_parameters.reserve(size); }

Tree::EventLog::size_type Tree::EventLog::capacity() const noexcept
{
  return m_parameters.capacity();
}

std::string Tree::EventLog::to_string() const
{
  // Firstly we need to convert the time to a string
  // This is a remarkably involved process for some reason.
  // The neatest way I found of doing this was found at:
  // https://gist.github.com/polaris/adee936198995a6f8c697c419d21f734
  std::time_t t  = std::chrono::system_clock::to_time_t(m_time);
  std::string ts = std::ctime(&t);
  ts.resize(ts.size() - 1);

  auto str = "Name:" + m_name + "\nTime:" + Util::time_to_string(m_time) + "\nParameters:\n";

  for (const auto &p : m_parameters)
  {
    str += p.first + "," + p.second + "\n";
  }

  str += "Aux data: ";

  if (m_aux == nullptr)
  {
    return str + "None";
  }

  return str + m_aux->to_string();
}
#ifndef INCLUDED_TREE_LOGGER
#define INCLUDED_TREE_LOGGER

#include "Tree_Tree.hpp"

namespace Tree
{

/**
   Logger. This is the primary template class for the Tree library.
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
inline typename Tree::Logger<LogType, StoragePolicy, LockPolicy>::const_iterator
Tree::Logger<LogType, StoragePolicy, LockPolicy>::cbegin() const noexcept
{
  return m_store.cbegin();
}

template <typename LogType, template <typename> class StoragePolicy, typename LockPolicy>
inline typename Tree::Logger<LogType, StoragePolicy, LockPolicy>::const_iterator
Tree::Logger<LogType, StoragePolicy, LockPolicy>::cend() const noexcept
{
  return m_store.cend();
}

template <typename LogType, template <typename> class StoragePolicy, typename LockPolicy>
inline typename Tree::Logger<LogType, StoragePolicy, LockPolicy>::size_type
Tree::Logger<LogType, StoragePolicy, LockPolicy>::size() const noexcept
{
  return m_store.size();
}

template <typename LogType, template <typename> class StoragePolicy, typename LockPolicy>
inline typename Tree::Logger<LogType, StoragePolicy, LockPolicy>::size_type
Tree::Logger<LogType, StoragePolicy, LockPolicy>::capacity() const noexcept
{
  return m_store.capacity();
}

template <typename LogType, template <typename> class StoragePolicy, typename LockPolicy>
inline void Tree::Logger<LogType, StoragePolicy, LockPolicy>::insert(LogType &&log)
{
  typename LockPolicy::LockType lock(m_lock.getLock());
  m_store.insert(log);
}

template <typename LogType, template <typename> class StoragePolicy, typename LockPolicy>
inline void Tree::Logger<LogType, StoragePolicy, LockPolicy>::insert(const LogType &log)
{
  typename LockPolicy::LockType lock(m_lock.getLock());
  m_store.insert(log);
}

template <typename LogType, template <typename> class StoragePolicy, typename LockPolicy>
inline void Tree::Logger<LogType, StoragePolicy, LockPolicy>::clear() noexcept
{
  typename LockPolicy::LockType lock(m_lock.getLock());
  m_store.clear();
}

template <typename LogType, template <typename> class StoragePolicy, typename LockPolicy>
template <typename Ostream>
inline Ostream &Tree::Logger<LogType, StoragePolicy, LockPolicy>::operator<<(Ostream &os)
{
  // All of the state is kept in the store, so we'll just use that.
  os << m_store;
  return os;
}
}  // namespace Tree

#endif
#include "Tree_Logger.hpp"
#ifndef INCLUDED_TREE_MUTEX_LOCK
#define INCLUDED_TREE_MUTEX_LOCK

#include <mutex>

#include "Tree_Tree.hpp"

namespace Tree
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

}  // namespace Tree
#endif
#include "Tree_MutexLock.hpp"
#ifndef INCLUDED_TREE_NO_LOCK
#define INCLUDED_TREE_NO_LOCK

#include "Tree_AnyType.hpp"
#include "Tree_Tree.hpp"

namespace Tree
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
  using LockType = Tree::AnyType;
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

}  // namespace Tree
#endif
#include "Tree_NoLock.hpp"
#ifndef INCLUDED_TREE_TEST_DATA
#define INCLUDED_TREE_TEST_DATA

#include "Tree_Data.hpp"
#include "Tree_Tree.hpp"
#include <string>

namespace Tree
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
}  // namespace Tree
#endif
#include "Tree_TestData.hpp"

Tree::TestData::~TestData() = default;
bool Tree::TestData::operator==(const Data & /*other*/) const noexcept { return true; }
std::string Tree::TestData::to_string() const { return "None"; }
std::unique_ptr<Tree::Data> Tree::TestData::copy() const
{
  return std::make_unique<Tree::TestData>();
}
#ifndef INCLUDED_LOGGER_UTIL
#define INCLUDED_LOGGER_UTIL

#include <chrono>
#include <string>

#include "Tree_Tree.hpp"

namespace Tree
{

namespace Util
{
/**
   time_to_string. Given a time point as input (denoted as `time`), this method
returns a string representation of the `time` variable. This method differs from
other approaches in that it does not rely on string streams or similar. This
method may throw due to allocation failures. Note that this function does not
modify the parameter. \param time: the time to be converted to a string. \return
a string representing the `time` parameter.
**/
std::string time_to_string(const std::chrono::time_point<std::chrono::system_clock> &time);
}  // namespace Util
}  // namespace Tree
#endif
#include "Tree_Util.hpp"

std::string
Tree::Util::time_to_string(const std::chrono::time_point<std::chrono::system_clock> &time)
{
  // This is a remarkably involved process for some reason.
  // The neatest way I found of doing this was found at:
  // https://gist.github.com/polaris/adee936198995a6f8c697c419d21f734
  std::time_t t  = std::chrono::system_clock::to_time_t(time);
  std::string ts = std::ctime(&t);
  ts.resize(ts.size() - 1);
  return ts;
}
#ifndef INCLUDED_TREE_DECL
#define INCLUDED_TREE_DECL

/**
   This component contains some useful definitions that can be used
   inside Tree.
**/

#include "Tree_Tree.hpp"

namespace Tree
{
/**
   SingleThreadedEventLogger. This declaration instantiates a contiguously
stored event logger with no locking. This logger does not lock on insertions or
any other similar operations. Thus, this logger should only be used in a
single-threaded context.
**/
using SingleThreadedEventLogger =
    Tree::Logger<Tree::EventLog, Tree::ContiguousLogStorage, Tree::NoLock>;
/**
   MultiThreadedEventLogger. This declaration instantiates a contiguously stored
event logger with locking based on a mutex. This logger locks the logs using a
std::mutex: this only allows a single thread to write to the logger at once.
**/
using MultiThreadedEventLogger =
    Tree::Logger<Tree::EventLog, Tree::ContiguousLogStorage, Tree::MutexLock>;
};  // namespace Tree

#endif
