cmake_minimum_required(VERSION 3.10)
project(
  LibraryManagementSystem
  VERSION 0.3
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT MSVC)
  add_compile_options(-Wall -Wextra)
endif()

add_library(library_core src/Book.cpp src/Library.cpp src/Utils.cpp)
target_include_directories(library_core PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(library_system src/main.cpp)
target_link_libraries(library_system PRIVATE library_core)

enable_testing()
add_executable(library_tests tests/test_library.cpp)
target_link_libraries(library_tests PRIVATE library_core)
add_test(NAME library_tests COMMAND library_tests)
