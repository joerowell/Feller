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
#include "Feller_Util.hpp"

std::string
Feller::Util::time_to_string(const std::chrono::time_point<std::chrono::system_clock> &time)
{
  // This is a remarkably involved process for some reason.
  // The neatest way I found of doing this was found at:
  // https://gist.github.com/polaris/adee936198995a6f8c697c419d21f734
  std::time_t t  = std::chrono::system_clock::to_time_t(time);
  std::string ts = std::ctime(&t);
  ts.resize(ts.size() - 1);
  return ts;
}
