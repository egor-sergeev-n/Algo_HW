project(Graphs)
cmake_minimum_required(VERSION 3.10)

set(SOURCE src/ListGraph.cpp)
include_directories(include)
add_executable(main main.cpp ${SOURCE})