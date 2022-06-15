cmake_minimum_required(VERSION 3.22)
project(Matrix)

set(CMAKE_CXX_STANDARD 14)

add_executable(#Matrix main.cpp Matrix.cpp Matrix.h
        test.cpp
        )
