project(Graphs)
cmake_minimum_required(VERSION 3.10)

set(SOURCE src/ListGraph.cpp
           src/MatrixGraph.cpp
           src/SetGraph.cpp
           src/ArcGraph.cpp)
include_directories(include)
add_executable(main main.cpp ${SOURCE})