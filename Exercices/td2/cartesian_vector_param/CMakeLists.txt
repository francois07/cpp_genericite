cmake_minimum_required(VERSION 3.0)

project(cartesian_vector_project)

add_library(cartesianvector SHARED vector.cpp)

add_executable(main main.cpp)

target_link_libraries(main cartesianvector)