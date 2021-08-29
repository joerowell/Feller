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
#ifndef INCLUDED_TREE
#define INCLUDED_TREE

/**
   \brief
   Feller. This is the main namespace for the Feller library.
   This namespace simply forward declares all of the structs that are used
inside this project. For more information, see the top-level readme.
**/

namespace Feller
{

/**
 \brief The purpose of this component is to provide a placeholder type
 for template expansions. In particular, this class should be used to
 consume arbitrary input.
**/
struct AnyType;

/**
  \brief The purpose of this component is to provide a straightforward
  contiguous container for logs of a specified type. Use this if your
application would benefit in some sense from a contiguous representation of your
logs.
**/
template <typename LogType> class ContiguousLogStorage;

/**
 \brief The purpose of this component is to allow you to extend the amount of
 data that is collected in each log.
**/
struct Data;

/**
 \brief The purpose of this class is to enable you to test other classes using
 the \ref Data class without needing to encroach on other classes. Note that
this test case is solely defined to allow you to write simpler unit tests: it
should not replace proper integration tests.
**/
struct TestData;

/**
  \brief The purpose of this component is to provide a simple
  class that represents an event log. Each event log corresponds to a unique
event in the lifetime of the program which may or may not be useful. This
component should be used inside the lifespan of the wider logging system: while
it is a small component, it can only really derive any useful meaning from
broader use.
**/
class EventLog;

/**
 * \brief The purpose of this component is to provide a simple policy class that
 * represents a Lock. This policy class should be used for ensuring that the
 * underlying \ref Logger is thread safe.
 **/
struct MutexLock;

/**
 \brief The purpose of this component is to provide a policy class that does
 not provide any locking mechanisms. In particular, this class is designed to
 enable concurrent operations to occur on a \ref Logger without any guarantees
of thread safety.
**/
struct NoLock;

/** \brief The purpose of this component is to provide a generic logging
  facility for your application. This component can be viewed as the primary
  entry facility for the Feller logging package. Note that this component
  primarily combines other, smaller classes to create a cohesive whole.
**/
template <typename LogType, template <typename> class StoragePolicy, typename LockPolicy>
class Logger;
/**
 \brief The purpose of this namespace is to contain any utility functions that
are associated with the Feller project. Note that this component should not depend
on any other files used in this project other than standard system headers.
**/
namespace Util
{
}
}  // namespace Feller

#endif
