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
#include "Feller_NoLock.hpp"
#include "gtest/gtest.h"

#define _CONCAT(x, y) x##y
#define CONCAT(x, y) _CONCAT(x, y)

TEST(NoLock, fullLock)
{
  // The point of this method is to show that, in fact,
  // you can lock a NoLock many many times while the other
  // locks are still in scope.

  Feller::NoLock nl{};

#define LOCK                                                                                       \
  [[maybe_unused]] Feller::NoLock::LockType CONCAT(_anonymous, __COUNTER__)(nl.getLock());

#define LOCK2 /*                                                                                   \
               */                                                                                  \
  LOCK        /*                                                                                   \
               */                                                                                  \
      LOCK
#define LOCK4 /*                                                                                   \
               */                                                                                  \
  LOCK2       /*                                                                                   \
               */                                                                                  \
      LOCK2

#define LOCK8 /*                                                                                   \
               */                                                                                  \
  LOCK4       /*                                                                                   \
               */                                                                                  \
      LOCK4
#define LOCK16 /*                                                                                  \
                */                                                                                 \
  LOCK8        /*                                                                                  \
                */                                                                                 \
      LOCK8
#define LOCK32 /*                                                                                  \
                */                                                                                 \
  LOCK16       /*                                                                                  \
                */                                                                                 \
      LOCK16
#define LOCK64 /*                                                                                  \
                */                                                                                 \
  LOCK32       /*                                                                                  \
                */                                                                                 \
      LOCK32
#define LOCK128 /*                                                                                 \
                 */                                                                                \
  LOCK64        /*                                                                                 \
                 */                                                                                \
      LOCK64
  LOCK128;
}
