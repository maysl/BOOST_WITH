cmake_minimum_required(VERSION 2.8)
project(boost_with)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wunused -Werror")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -std=c++14")
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()
message(STATUS "build type is '${CMAKE_BUILD_TYPE}'")
include_directories(.)

add_library(bwlib STATIC boost/with.hpp)
set_target_properties(bwlib PROPERTIES LINKER_LANGUAGE CXX)

#
# tests
#_______________________________________________________________________________
add_subdirectory(gtest-1.7.0)
include_directories(gtest-1.7.0/include)
file(GLOB TEST_SOURCES test/*.cpp)
add_executable(test_runner ${TEST_SOURCES})
target_link_libraries(test_runner gtest gtest_main)
add_custom_target(run_all_tests ALL
                  COMMAND ./test_runner
                  DEPENDS test_runner
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
