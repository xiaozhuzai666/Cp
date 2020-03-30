cmake_minimum_required(VERSION 3.15)
project(Cp)

set(CMAKE_CXX_STANDARD 14)

add_executable(Cp main.cpp game.cpp)
include_directories("D:\\x86_64-8.1.0-release-posix-sjlj-rt_v6-rev0\\mingw64\\include\\graphics.h")