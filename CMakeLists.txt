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
# NOTE: Vulkan-Engine
########################################
file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable(vulkan_engine ${SRC_FILES})

target_link_libraries(vulkan_engine PRIVATE
    vulkan
    pthread
    ${CMAKE_DL_LIBS}
)

target_include_directories(vulkan_engine PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

if(UNIX AND NOT APPLE)
    add_definitions(-DVKE_USE_PLATFORM_XCB_KHR)
    target_link_libraries(vulkan_engine PRIVATE xcb)
# elseif(APPLE)
# elseif(WIN32)
endif()

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

########################################
# NOTE: debug
########################################
option(ENABLE_DEBUG "Enable debug macros and flags" OFF)

if(ENABLE_DEBUG)
    target_compile_definitions(vulkan_engine PRIVATE DEBUG=1)
endif()


# ########################################
# # NOTE: unit-tests
# ########################################

if(ENABLE_TESTING)

    file(GLOB_RECURSE TEST_FILES "tests/*.cpp")
    file(GLOB_RECURSE SRC_FILES_NO_MAIN "src/VKE/*.cpp")

    add_executable(unit_tests ${TEST_FILES} ${SRC_FILES_NO_MAIN})

    if(UNIX AND NOT APPLE)
        add_definitions(-DVKE_USE_PLATFORM_XCB_KHR)
        target_link_libraries(unit_tests PRIVATE xcb)
    endif()

    target_link_libraries(unit_tests PRIVATE
        vulkan
        pthread
        ${CMAKE_DL_LIBS}
        criterion
        glm
    )

    target_include_directories(unit_tests PRIVATE 
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/tests
    )

    if(UNIX)
        add_definitions(-DVK_USE_PLATFORM_XCB_KHR)
        target_link_libraries(vulkan_engine PRIVATE xcb)
    endif()

    enable_testing()
    add_test(NAME unit_tests COMMAND unit_tests)

endif()
