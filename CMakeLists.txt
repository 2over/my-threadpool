cmake_minimum_required(VERSION 3.19)
project(my_threadpool)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(my_threadpool main.cpp)
