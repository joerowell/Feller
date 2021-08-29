#ifndef INCLUDED_FELLER_EVENT_LOG
#define INCLUDED_FELLER_EVENT_LOG

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "Feller_Feller.hpp"

#include "Feller_Data.hpp"
#include "Feller_Util.hpp"

namespace Feller
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
  inline std::unique_ptr<Feller::Data> &aux() noexcept;

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
  EventLog(std::string name) : m_name{name}, m_time{}, m_parameters{}, m_aux{nullptr} {}
  EventLog(const char *const name) : m_name{name}, m_time{}, m_parameters{}, m_aux{nullptr} {}

  /// These constructors build a log and immediately insert two values
  /// into the parameters set.
  EventLog(const std::string &key, const std::string &value)
      : m_name{"Inserted"}, m_time{}, m_parameters{}, m_aux{nullptr}
  {
    m_parameters.emplace_back(key, value);
  }

  EventLog(std::string key, std::string value)
      : m_name{"Inserted"}, m_time{}, m_parameters{}, m_aux{nullptr}
  {
    m_parameters.emplace_back(std::move(key), std::move(value));
  }

  EventLog(const char *const key, const char *const value)
      : m_name{"Inserted"}, m_time{}, m_parameters{}, m_aux{nullptr}
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

}  // namespace Feller
#endif
