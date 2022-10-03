cmake_minimum_required(VERSION 3.0)

project(is_prime_project)

add_library(isprime SHARED is_prime.cpp)

add_executable(main main.cpp)

target_link_libraries(main isprime)