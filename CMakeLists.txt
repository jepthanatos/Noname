cmake_minimum_required(VERSION 3.12)
project(Noname VERSION 0.0.1)

# Set the install prefix to the current working directory
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

link_libraries(stdc++fs)

enable_testing()
include(CTest)

add_subdirectory(tests)
add_subdirectory(src)
