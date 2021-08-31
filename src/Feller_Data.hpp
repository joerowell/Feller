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
#ifndef INCLUDED_FELLER_DATA
#define INCLUDED_FELLER_DATA

#include <memory>

#include "Feller_Feller.hpp"

namespace Feller
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
}  // namespace Feller
#endif
