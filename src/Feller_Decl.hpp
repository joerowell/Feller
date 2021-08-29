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
