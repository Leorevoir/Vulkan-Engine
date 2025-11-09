cmake_minimum_required(VERSION 3.16)
project(LumenEngine LANGUAGES CXX)

########################################

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(1-Options)
include(2-Optimizations)
include(3-Warnings)
include(4-Linker)
include(5-External)
include(6-Examples)

########################################

add_subdirectory(src)

########################################
