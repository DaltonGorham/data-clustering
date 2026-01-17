cmake_minimum_required(VERSION 3.10)
project(data_clustering)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(data_clustering
    src/main.cpp
    src/FileParser.cpp
    src/Dataset.cpp
)
