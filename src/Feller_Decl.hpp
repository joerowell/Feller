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
#ifndef INCLUDED_TREE_DECL
#define INCLUDED_TREE_DECL

/**
   This component contains some useful definitions that can be used
   inside Tree.
**/

#include "Tree_Tree.hpp"

namespace Tree
{
/**
   SingleThreadedEventLogger. This declaration instantiates a contiguously
stored event logger with no locking. This logger does not lock on insertions or
any other similar operations. Thus, this logger should only be used in a
single-threaded context.
**/
using SingleThreadedEventLogger =
    Tree::Logger<Tree::EventLog, Tree::ContiguousLogStorage, Tree::NoLock>;
/**
   MultiThreadedEventLogger. This declaration instantiates a contiguously stored
event logger with locking based on a mutex. This logger locks the logs using a
std::mutex: this only allows a single thread to write to the logger at once.
**/
using MultiThreadedEventLogger =
    Tree::Logger<Tree::EventLog, Tree::ContiguousLogStorage, Tree::MutexLock>;
};  // namespace Tree

#endif
