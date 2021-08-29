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
