cmake_minimum_required(VERSION 3.16)
project(my_threadpool)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(my_threadpool main.cpp include/common.h core/Taskpool.cpp core/Taskpool.h core/Task.cpp core/Task.h core/Thread.cpp core/Thread.h core/Monitor.cpp core/Monitor.h core/ParkEvent.cpp core/ParkEvent.h core/Threadpool.cpp core/Threadpool.h)
