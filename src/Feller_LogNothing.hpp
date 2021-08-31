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

#ifndef INCLUDED_FELLER_LOG_NOTHING
#define INCLUDED_FELLER_LOG_NOTHING

#include "Feller_StaticLoggingPolicy.hpp"
#include "Feller_LoggingMode.hpp"

namespace Feller
{
/**
   LogNothing. This class provides a policy class for the \ref Logger class.
   As the name suggests, this class recommends that the logger logs nothing: in other words,
   the logger collects no data.
   Since this class statically makes this recommendation, the methods that it provides essentially
 do nothing.
 **/
using LogNothing = StaticLoggingPolicy<Feller::LoggingMode::NOTHING>;
}  // namespace Feller

#endif
