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

#ifndef INCLUDED_FELLER_STATIC_LOGGING_POLICY
#define INCLUDED_FELLER_STATIC_LOGGING_POLICY

#include <type_traits>

#include "Feller_Feller.hpp"
#include "Feller_LoggingMode.hpp"

namespace Feller
{
/**
   StaticLoggingPolicy. This policy class provides the ability to set the type
   of Logging policy that should be used statically (i.e at compile-time).
   The reason for this choice is performance: the compiler can statically
   determine whether it needs to optimise for different logging scenarios.
   Since this mode cannot be changed at runtime, this mode should be used when
   the logging policy is neessarily set ahead of time (i.e if you cannot afford any logging but
   don't want to re-write all of your code to adjust for this change).
   \tparam logging: the logging mode.
**/

template <Feller::LoggingMode logging> class StaticLoggingPolicy
{
public:
  /**
     shouldLog. This method returns true if `mode <= logging` and false otherwise.
     This allows the caller to statically know if logging should occur or not.
     This result is undefined if mode == LoggingMode::SIZE.
     \param mode: the type of logging request.
     \return true if the log should be stored, false otherwise.
  **/

  inline constexpr bool shouldLog(const Feller::LoggingMode mode) noexcept;

  /**
     switchMode. This method simply returns, regardless of the parameter ``mode``.
     \param mode: an unused parameter describing the logging level.
  **/
  inline constexpr void switchMode(const Feller::LoggingMode /*unused*/) noexcept;

  /**
     mode. This method returns the logging mode of the static logger.
     \return the logging mode of this logger.
  **/
  inline constexpr Feller::LoggingMode mode() noexcept;
};

/// INLINE METHODS
template <Feller::LoggingMode logging>
inline constexpr bool
StaticLoggingPolicy<logging>::shouldLog(const Feller::LoggingMode mode) noexcept
{
  return (logging != Feller::LoggingMode::NOTHING) &
         (static_cast<std::underlying_type<Feller::LoggingMode>::type>(mode) <=
          static_cast<std::underlying_type<Feller::LoggingMode>::type>(logging));
}

template <Feller::LoggingMode logging>
inline constexpr void StaticLoggingPolicy<logging>::switchMode(const Feller::LoggingMode) noexcept
{
}

template <Feller::LoggingMode logging>
inline constexpr Feller::LoggingMode StaticLoggingPolicy<logging>::mode() noexcept
{
  return logging;
}
}  // namespace Feller

#endif
