cmake_minimum_required(VERSION 3.0)

project(ExpandRand)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/expandRand.cpp)