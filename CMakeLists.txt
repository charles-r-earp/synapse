cmake_minimum_required(VERSION 3.5)
project(pinch)
# c++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_subdirectory(synapse)
add_subdirectory(tests)
#add_subdirectory(bench)