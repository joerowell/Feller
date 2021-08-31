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
#include "Tree.hpp"
#include <iostream>

int main()
{
  Tree::SingleThreadedEventLogger logger;
  using Log = Tree::SingleThreadedEventLogger::log_type;

  logger.insert(Log{"Entering loop"});
  const unsigned random_size = rand() % 4096;

  for (unsigned i = 0; i < random_size; i++)
  {
    logger.insert(Log{"Doing something with:", std::to_string(i)});
  }
  logger.insert(Log{"Leaving"});

  // This should be 2 + random_size
  std::cout << "Number of entries:" << logger.size() << std::endl;
}
