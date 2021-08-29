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
