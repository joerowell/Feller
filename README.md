# Feller

Feller is a small C++17 logging library. Feller provides a generic logging class that can be customised via template parameters. Feller also provides some basic types of logs that can be used to profile your application, with the ability to extend these types via both static and dynamic polymorphism.

# Examples:
## Basic example:

``` cpp
#include "Feller.hpp"
#include <iostream>

int main() {
  Feller::SingleThreadedEventLogger logger;
  using Log = Feller::SingleThreadedEventLogger::log_type;

  logger.insert(Log{"Entering loop"});
  const unsigned random_size = rand() % 4096;

  for (unsigned i = 0; i < random_size; i++) {
    logger.insert(Log{"Doing something with:", std::to_string(i)});
  }
  
  logger.insert(Log{"Leaving"});

  // This should be 2 + random_size
  std::cout << "Number of entries:" << logger.size() << std::endl;
}

```

## New Data type:
``` cpp
#include "Feller.hpp"
#include <iostream>

// We'll pretend this includes "IsItPrime"
#include "FancyTypeThatUsesData.hpp"

int main() {
  Feller::SingleThreadedEventLogger logger;
  using Log = Feller::SingleThreadedEventLogger::log_type;

  logger.insert(Log{"Entering loop"});
  const unsigned random_size = rand() % 4096;

  for (unsigned i = 0; i < random_size; i++) {
    Log l{"Doing something with:", std::to_string(i)};
    l.aux() = IsItPrime{i};
    logger.insert(std::move(l));
  }
  
  logger.insert(Log{"Leaving"});

  // This should be 2 + random_size
  std::cout << "Number of entries:" << logger.size() << std::endl;
  
  // This will print out whether the number is prime or not!
  for(auto& v : logger) {
     std::cout << v.aux() << std::endl;
  }
}

```


## Why another logging library?

There's absolutely no need for another logging library in C++: there's a plethora of existing solutions that all work well. 

However, Feller has its advantages:
- Feller is small. The code-base is around 1000 lines, with over half of that being documentation. 
  This makes it suitable for inclusion as a single header file.
- Feller uses policy-based design. The logger itself is comprised of a few template parameters that
  control how the logger behaves at any given time. Thus, the behaviour of the logger can be tweaked
  in a fine-grained manner. 
- Feller is well-tested. Code is not committed to the main branch unless 95% test coverage is reached.
- Feller is fully documented. Every method, class and namespace all sport full doxygen. 
- Feller is compiled with clang-tidy and a lot of warnings turned on. We even set ``-Werror``!
- Feller is (somewhat) battle-tested. Feller was born out of use in another project where it is extensively used for logging. There haven't been any major use cases there that weren't covered by Feller. 
Thus, if you need a small logging library that suits these criteria, then Feller may be the library for you.

## Project layout
Feller has the following layout:
	- src. src contains all of the code for this project. Everything is placed inside the 
	  Feller namespace.

## How to build

If you are looking for a header only solution, then you should start with the ``Feller.hpp`` file. 
This file is auto-generated, so any code duplication is minimised. If for whatever reason you want to re-generate this file, then you can run the `generate_feller_header` script.

If you are looking for a full library, then you can build Feller in the following way. Note that you will require CMake for this to work.

``` bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ../
make
```

This will produce a shared object called ``libFeller``, which can then be linked against.
You can also build the doxygen documentation by replacing the above ``make`` with ``make doc``.

## Running tests

If you wish to run tests, then you will need to following dependencies:
- [GoogleTest](https://github.com/google/googletest)
- [Clang-tidy](https://clang.llvm.org/extra/clang-tidy/)
- [AddressSanitizer](https://clang.llvm.org/docs/AddressSanitizer.html)

With these installed, you can build and run the tests using:
``` bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ../
make
cd ../
./generate_coverage
```

As an example, you can the test ``testEventLog`` by running the following commands:

``` bash
LD_PRELOAD=<path to libasan> ./testEventLog 
```






