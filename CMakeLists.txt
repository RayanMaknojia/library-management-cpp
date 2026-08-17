cmake_minimum_required(VERSION 3.10)
project(LibraryManagementSystem VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(library_system
    src/main.cpp
    src/Book.cpp
    src/Library.cpp
)
