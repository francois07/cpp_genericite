cmake_minimum_required(VERSION 3.0)

project(find_motive_project)

add_library(findmotive SHARED find_motive.cpp)

add_executable(main main.cpp)

target_link_libraries(main findmotive)