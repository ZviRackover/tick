cmake_minimum_required (VERSION 2.8)
project (tick)

#version number
set (Tick_VERSION_MAJOR 1)
set (Tick_VERSION_MINOR 0)

add_executable (tick main.cpp)

install (TARGETS tick DESTINATION bin)
