cmake_minimum_required(VERSION 3.10)

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(.)
enable_testing()
