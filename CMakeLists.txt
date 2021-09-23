cmake_minimum_required(VERSION 3.16)
project(hw1)

set(CMAKE_CXX_STANDARD 17)

add_executable(hw1 main.cpp BigInt.cpp BigInt.h)

# gtest настраивал как здесь: https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(runTests tests.cpp BigInt.cpp BigInt.h)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)