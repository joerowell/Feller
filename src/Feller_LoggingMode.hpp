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

#ifndef INCLUDED_FELLER_LOGGING_MODE
#define INCLUDED_FELLER_LOGGING_MODE

namespace Feller
{
/**
   LoggingMode. This enum provides a series of individual logging levels
   that describe how aggressive the logger should be when it comes to tracking
   events. Note that this class is far from comprehensive, and it is very possible
   that there are more possible levels that would suit different circumstances.

   This enum class operates under a strict ordering. Lower priority logging strategies should come
early with low values assigned: similarly, higher priority strategies should be later with higher
values. Higher strategies imply lower ones.

   Please note that if you add a member to this enum then you must update the "SIZE" member at the
end. Whilst annoying, this helps with testability.
**/
enum class LoggingMode
{
  NOTHING    = 0,
  IMPORTANT  = 1,
  EVERYTHING = 2,
  SIZE       = 3
};
}  // namespace Feller

#endif
