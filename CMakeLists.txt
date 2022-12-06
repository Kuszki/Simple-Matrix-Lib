cmake_minimum_required(VERSION 3.5)

project(Simple-Matrix-Lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Simple-Matrix-Lib main.cpp matrix.hpp)

install(TARGETS Simple-Matrix-Lib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
