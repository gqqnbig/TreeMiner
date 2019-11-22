cmake_minimum_required(VERSION 3.13)
project(TreeMiner)

set(CMAKE_CXX_STANDARD 14)

add_compile_definitions(debug)

add_executable(TreeMiner
        TreeMiner.cpp)
