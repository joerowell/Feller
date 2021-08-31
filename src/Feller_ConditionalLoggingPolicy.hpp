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

#ifndef INCLUDED_FELLER_CONDITIONAL_LOGGING_POLICY
#define INCLUDED_FELLER_CONDITIONAL_LOGGING_POLICY

#include <type_traits>

#include "Feller_Feller.hpp"
#include "Feller_LoggingMode.hpp"

namespace Feller
{
/**
   ConditionalLoggingPolicy. This policy class provides the ability to decide
   what logs should be collected at runtime. This policy is useful in situations
   where you either can (or need to) change the type or level of log collection during the
   running of the program.
**/
class ConditionalLoggingPolicy
{
private:
  /**
     mode. This specifies the current logging mode. The way to read this mode is the following:
     any logging request that is less than or equal to the current mode is selected, with anything
  above it rejected.
  **/
  Feller::LoggingMode m_mode{Feller::LoggingMode::EVERYTHING};

public:
  /**
     shouldLog. This method returns true if `mode =< m_mode` and false otherwise.
     This allows the caller to know if logging should occur.
     The result is undefined if mode == LoggingMode::SIZE.
     \param mode: the type of logging request.
     \return true if logging should occur, false otherwise.
  **/
  inline bool shouldLog(const Feller::LoggingMode mode) noexcept;

  /**
     switchMode. This method switches the mode of the logger to `mode`. This method
     can be used to adjust the granularity of the logger.
   **/
  inline void switchMode(const Feller::LoggingMode mode) noexcept;

  /**
     mode. This method returns the logging mode of this logger. This simply returns the `m_mode`
     member variable.
     \return the logging mode of this logger.
   **/
  inline Feller::LoggingMode mode() noexcept;
};

/// INLINE METHODS
inline bool ConditionalLoggingPolicy::shouldLog(const Feller::LoggingMode mode) noexcept
{
  return (m_mode != Feller::LoggingMode::NOTHING) &&
         static_cast<std::underlying_type<Feller::LoggingMode>::type>(mode) <=
             static_cast<std::underlying_type<Feller::LoggingMode>::type>(m_mode);
}

inline void ConditionalLoggingPolicy::switchMode(const Feller::LoggingMode mode) noexcept
{
  m_mode = mode;
}

inline Feller::LoggingMode ConditionalLoggingPolicy::mode() noexcept { return m_mode; }
}  // namespace Feller

#endif
