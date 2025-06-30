cmake_minimum_required(VERSION 3.16)
project(vulkan_engine VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

########################################
# INFO: enable multithreading for build
########################################
include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
    set(CMAKE_BUILD_PARALLEL_LEVEL ${N})
endif()

########################################
# INFO: CMake cache management for faster builds
########################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

########################################
# NOTE: GLFW configuration
########################################
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_INSTALL OFF)

if(DEFINED ENV{XDG_SESSION_TYPE})
    if($ENV{XDG_SESSION_TYPE} STREQUAL "wayland")
        message(STATUS "Detected Wayland session")
        message(STATUS "Including Wayland support")
        set(GLFW_BUILD_WAYLAND ON)
        set(GLFW_BUILD_X11 OFF)
    elseif($ENV{XDG_SESSION_TYPE} STREQUAL "x11")
        message(STATUS "Detected X11 session")
        set(GLFW_BUILD_WAYLAND OFF)
        set(GLFW_BUILD_X11 ON)
    endif()
endif()

if(NOT DEFINED GLFW_BUILD_WAYLAND AND NOT DEFINED GLFW_BUILD_X11)
    message(WARNING "Unknown session type; defaulting to X11")
    set(GLFW_BUILD_X11 ON)
    set(GLFW_BUILD_WAYLAND OFF)
endif()

add_subdirectory(external/glfw EXCLUDE_FROM_ALL)

########################################
# NOTE: Vulkan-Engine
########################################
file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable(vulkan_engine ${SRC_FILES})

target_link_libraries(vulkan_engine PRIVATE
    glfw
    vulkan
    pthread
    ${CMAKE_DL_LIBS}
)

target_include_directories(vulkan_engine PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external
)

target_compile_options(vulkan_engine PRIVATE
    -Wall
    -Wextra
    -Werror
    -pedantic
    -Wconversion
    -Wsign-conversion
    -Wshadow
    -Wnull-dereference
    -Wundef
    -Wuninitialized
    -Winit-self
    -Wredundant-decls
    -Wcast-align
    -Wcast-qual
    -Wmissing-declarations
    -Wswitch-default
    -Wdouble-promotion
    -Wformat=2
    -Wwrite-strings
)
