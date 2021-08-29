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
#include "Feller_EventLog.hpp"

void Feller::EventLog::clear() { m_parameters.clear(); }

void Feller::EventLog::resize(const Feller::EventLog::size_type size) { m_parameters.resize(size); }

void Feller::EventLog::set_zero()
{
  using vec_type = decltype(m_parameters);
  vec_type().swap(m_parameters);
}

void Feller::EventLog::emplace_back(const std::string &key, const std::string &value)
{
  emplace_back(std::pair<std::string, std::string>(key, value));
}

void Feller::EventLog::emplace_back(const std::pair<std::string, std::string> &value)
{
  m_parameters.emplace_back(value);
}

void Feller::EventLog::emplace_back(std::pair<std::string, std::string> &&value)
{
  m_parameters.emplace_back(std::move(value));
}

void Feller::EventLog::emplace_back(std::string &&key, std::string &&value)
{
  emplace_back(std::pair<std::string, std::string>(std::move(key), std::move(value)));
}

void Feller::EventLog::reserve(const Feller::EventLog::size_type size)
{
  m_parameters.reserve(size);
}

Feller::EventLog::size_type Feller::EventLog::capacity() const noexcept
{
  return m_parameters.capacity();
}

std::string Feller::EventLog::to_string() const
{
  // Firstly we need to convert the time to a string
  // This is a remarkably involved process for some reason.
  // The neatest way I found of doing this was found at:
  // https://gist.github.com/polaris/adee936198995a6f8c697c419d21f734
  std::time_t t  = std::chrono::system_clock::to_time_t(m_time);
  std::string ts = std::ctime(&t);
  ts.resize(ts.size() - 1);

  auto str = "Name:" + m_name + "\nTime:" + Util::time_to_string(m_time) + "\nParameters:\n";

  for (const auto &p : m_parameters)
  {
    str += p.first + "," + p.second + "\n";
  }

  str += "Aux data: ";

  if (m_aux == nullptr)
  {
    return str + "None";
  }

  return str + m_aux->to_string();
}
