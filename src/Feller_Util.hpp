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
#ifndef INCLUDED_FELLER_UTIL
#define INCLUDED_FELLER_UTIL

#include <chrono>
#include <string>

#include "Feller_Feller.hpp"

namespace Feller
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
}  // namespace Feller
#endif
