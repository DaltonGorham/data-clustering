cmake_minimum_required(VERSION 3.10)
project(data_clustering)

set(CMAKE_CXX_STANDARD 20)

add_executable(data_clustering main.cpp FileParser.cpp Utilities.h)
