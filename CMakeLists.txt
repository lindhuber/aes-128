cmake_minimum_required(VERSION 3.10)
project(AES_128_Project)

set(CMAKE_CXX_STANDARD 17)

add_executable(aes main.cpp args_parser.hpp)
