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
#ifndef INCLUDED_FELLER_ANY_TYPE
#define INCLUDED_FELLER_ANY_TYPE

#include "Feller_Feller.hpp"

namespace Feller
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
}  // namespace Feller
#endif
