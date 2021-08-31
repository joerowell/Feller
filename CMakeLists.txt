Cmake_minimum_required(VERSION 3.10)
project(Feller VERSION 1.0 LANGUAGES CXX DESCRIPTION "A small C++17 logging library")

if(CMAKE_BUILD_TYPE MATCHES Debug)
  ################################
  # Clang-tidy
  ################################
  ## NB the "extra arg is meant to tell CMake not to run
  ## clang-tidy on system headers.
  ## This whole number of arguments are mostly telling clang-tidy to ignore any of the errors in
  ## google test.
  set(CMAKE_CXX_CLANG_TIDY clang-tidy;-checks=*,-portability-*,-objc-*,-fuchsia-*,-abseil-*,-hicpp-*,-cppcoreguidelines-owning-memory,-llvm-include-order,-llvm-qualifed-auto,-modernize-use-trailing-return-type,-cppcoreguidelines-avoid-magic-numbers,-cppcoreguidelines-pro-bounds-array-to-pointer-decay,-readability-magic-numbers,-cppcoreguidelines-avoid-goto,-cppcoreguidelines-pro-type-vararg,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cert-msc30-c,-cert-msc50-cpp,-hicpp-no-array-decay,-cppcoreguidelines-pro-bounds-constant-array-index,-cppcoreguidelines-macro-usage,-cppcoreguidelines-pro-type-static-cast-downcast,-hicpp-avoid-goto,-hicpp-vararg,-cert-err58-cpp;--extra-arg=-isystem;)

  ################################
  # GTest
  ################################
  ADD_SUBDIRECTORY (googletest)
  enable_testing()
  include_directories(SYSTEM ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
  
  add_library(FellerDebug SHARED
    src/Feller_TestData.cpp
    src/Feller_Data.cpp
    src/Feller_Util.cpp
    src/Feller_EventLog.cpp
    src/Feller_MutexLock.cpp
    src/Feller_NoLock.cpp
    src/Feller_Logger.cpp
    src/Feller_LoggingMode.cpp
    src/Feller_StaticLoggingPolicy.cpp
    src/Feller_LogEverything.cpp
    src/Feller_LogNothing.cpp
    src/Feller_ContiguousLogStorage.cpp)

  
  # Clang doesn't seem to work well with GCOV at the moment.
  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang") 
    target_compile_options(FellerDebug PRIVATE -g3 -ggdb -std=c++17  -fsanitize=address,undefined -Wall -Wextra -Werror -Wpedantic -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wsign-conversion -Wmisleading-indentation -Wnull-dereference -Wdouble-promotion -Wformat=2 -Wcast-qual -Wsign-promo -Wdisabled-optimization)
    set(gcov_name "")
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(FellerDebug PRIVATE -g3 -ggdb -std=c++17  -fsanitize=address,undefined -Wall -Wextra -Werror -Wpedantic -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wsign-conversion -Wmisleading-indentation -Wnull-dereference -Wdouble-promotion -Wformat=2 -Wcast-qual -Wsign-promo -Wdisabled-optimization -fprofile-arcs -ftest-coverage)
    set(gcov_name "gcov")
  endif()
  
  target_link_libraries(FellerDebug ${gcov_name} asan ubsan)
  
  add_executable(testTestData src/Feller_TestData.t.cpp)
  add_executable(testLog src/Feller_EventLog.t.cpp)
  add_executable(testLock src/Feller_MutexLock.t.cpp)
  add_executable(testNoLock src/Feller_NoLock.t.cpp)
  add_executable(testContiguousLogStorage src/Feller_ContiguousLogStorage.t.cpp)
  add_executable(testLogger src/Feller_Logger.t.cpp)
  add_executable(testStaticLoggingPolicy src/Feller_StaticLoggingPolicy.t.cpp)
  add_executable(testConditionalLoggingPolicy src/Feller_ConditionalLoggingPolicy.t.cpp)
  
  ### Force position independent code
  set_target_properties(testTestData PROPERTIES COMPILE_FLAGS "-std=c++17")
  set_target_properties(testLog PROPERTIES COMPILE_FLAGS "-std=c++17")
  set_target_properties(testLock PROPERTIES COMPILE_FLAGS "-std=c++17 ")
  set_target_properties(testNoLock PROPERTIES COMPILE_FLAGS "-std=c++17")
  set_target_properties(testLogger PROPERTIES COMPILE_FLAGS "-std=c++17")
  set_target_properties(testContiguousLogStorage PROPERTIES COMPILE_FLAGS "-std=c++17")
  set_target_properties(testStaticLoggingPolicy PROPERTIES COMPILE_FLAGS "-std=c++17")
  set_target_properties(testConditionalLoggingPolicy PROPERTIES COMPILE_FLAGS "-std=c++17")
  ### Link the target here
  target_link_libraries(testTestData FellerDebug gtest gtest_main ${gcov_name} asan ubsan)
  target_link_libraries(testLog FellerDebug gtest gtest_main ${gcov_name} asan ubsan)
  target_link_libraries(testLock FellerDebug gtest gtest_main ${gcov_name} asan ubsan)
  target_link_libraries(testNoLock FellerDebug gtest gtest_main ${gcov_name} asan ubsan)
  target_link_libraries(testLogger FellerDebug gtest gtest_main ${gcov_name} asan ubsan)
  target_link_libraries(testContiguousLogStorage FellerDebug gtest gtest_main ${gcov_name} asan ubsan)
  target_link_libraries(testStaticLoggingPolicy FellerDebug gtest gtest_main ${gcov_name} asan ubsan)
  target_link_libraries(testConditionalLoggingPolicy FellerDebug gtest gtest_main ${gcov_name} asan ubsan)
  
  # Add here if you want ctest to pick up these tests.
  # There's no reason not to have all tests here, as it makes
  # running CI as simple as running Ctest
  add_test(Log testLog)
  add_test(TestData testTestData)
  add_test(Lock testLock)
  add_test(NoLock testNoLock)
  add_test(Logger testLogger)
  add_test(ContiguousLogStorage testContiguousLogStorage)
  add_test(StaticLoggingPolicy testStaticLoggingPolicy)
  add_test(ConditionalLoggingPolicy testConditionalLoggingPolicy)
endif()

##################################
# Doxygen
##################################
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API        
documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
         message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile)
    
    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    message("Doxygen build started.")

    add_custom_target(doc
                      COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile_in}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
                      COMMENT "Generating API documentation with Doxygen"
                      VERBATIM)
endif()

##################################
# Release mode
##################################
if (CMAKE_BUILD_TYPE MATCHES Release)

  # These are compiler flags - these should not be used
  # to give you a view of what the final code will look like: these are just for testing.
  add_definitions("-Ofast -march=native -std=c++17 -march=native -fPIE -fPIC")

  # Generate the output library
  add_library(Feller SHARED
    src/Feller_TestData.cpp
    src/Feller_Data.cpp
    src/Feller_Util.cpp
    src/Feller_EventLog.cpp
    src/Feller_MutexLock.cpp
    src/Feller_NoLock.cpp
    src/Feller_Logger.cpp
    src/Feller_ContiguousLogStorage.cpp)

  add_executable(Example src/Feller_example.m.cpp)
  
endif()
