cmake_minimum_required(VERSION 3.11)

project(matrix_chain LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

enable_testing()

add_executable(matrix_chain src/driver.cpp)
target_compile_features(matrix_chain PUBLIC cxx_std_23)
target_include_directories(matrix_chain PRIVATE ${CMAKE_SOURCE_DIR}/includes)

add_subdirectory(tests/unit_tests)
add_subdirectory(tests/e2e_tests)
add_subdirectory(benchmark)