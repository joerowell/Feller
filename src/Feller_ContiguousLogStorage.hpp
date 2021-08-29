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
#ifndef INCLUDED_FELLER_CONTIGUOUS_LOG_STORAGE
#define INCLUDED_FELLER_CONTIGUOUS_LOG_STORAGE

#include <sstream>
#include <string>
#include <vector>

#include "Feller_Feller.hpp"

namespace Feller
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
inline void Feller::ContiguousLogStorage<LogType>::insert(const LogType &log)
{
  this->emplace_back(log);
}

template <typename LogType> inline void Feller::ContiguousLogStorage<LogType>::insert(LogType &&log)
{
  this->emplace_back(std::move(log));
}

}  // namespace Feller

#endif
