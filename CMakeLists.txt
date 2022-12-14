cmake_minimum_required(VERSION 3.5)

project(Simple-Matrix-Lib LANGUAGES CXX)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP)
include_directories(${OpenMP_INCLUDE_DIR})
link_directories(${OpenMP_LIBRARY_DIR})

add_library(${PROJECT_NAME} INTERFACE
	matrix.cpp matrix.hpp)

add_executable(test_main main.cpp
	utils.hpp utils.cpp
	helper.hpp helper.cpp)

add_executable(test_bas bastest.cpp)
add_executable(test_add addtest.cpp)
add_executable(test_sub subtest.cpp)
add_executable(test_mul multest.cpp)
add_executable(test_sta statest.cpp)
add_executable(test_ops opstest.cpp)
add_executable(test_mat mattest.cpp)
add_executable(test_iof ioftest.cpp)

add_test(NAME basics COMMAND test_bas)
add_test(NAME addition COMMAND test_add)
add_test(NAME substraction COMMAND test_sub)
add_test(NAME multiplication COMMAND test_mul)
add_test(NAME statistics COMMAND test_sta)
add_test(NAME operations COMMAND test_ops)
add_test(NAME maths COMMAND test_mat)
add_test(NAME files COMMAND test_iof)

target_link_libraries(test_main PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(test_bas PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(test_add PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(test_sub PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(test_mul PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(test_sta PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(test_ops PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(test_mat PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(test_iof PUBLIC OpenMP::OpenMP_CXX)

set_source_files_properties(helper.cpp PROPERTIES HEADER_FILE_ONLY TRUE)
set_source_files_properties(matrix.cpp PROPERTIES HEADER_FILE_ONLY TRUE)
