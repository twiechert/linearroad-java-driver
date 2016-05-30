cmake_minimum_required(VERSION 2.8)

project (test)
set(CMAKE_BUILD_TYPE Release)
file(GLOB SOURCES "*.cpp")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(datadriver SHARED ${SOURCES})
