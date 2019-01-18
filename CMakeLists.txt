cmake_minimum_required(VERSION 3.5)
project(BloomShell)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall  -pedantic")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.45.0 REQUIRED )
#include_directories(${CMAKE_SOURCE_DIR}/ext/dynamic_bitset/include)
include_directories(${Boost_INCLUDE_DIRS})



add_library(replxx STATIC IMPORTED)
set_target_properties(replxx PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/ext/replxx/build/libreplxx.a)
include_directories(${CMAKE_SOURCE_DIR}/ext/replxx/include)

add_subdirectory(src)
