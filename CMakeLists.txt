cmake_minimum_required(VERSION 3.9)
project(CSCI340_OS)

set(CMAKE_CXX_STANDARD 11)

add_executable(CSCI340_OS main.cpp process.h ready_queue.h cpu.h drive.h ram.h tools.h)