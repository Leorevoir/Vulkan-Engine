#######################################

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

message(STATUS "INFO: Source directory set to ${SOURCE_DIR}")
message(STATUS "INFO: Include directory set to ${INCLUDE_DIR}")

file(GLOB_RECURSE Vulkan-Engine_SOURCES
    "${SOURCE_DIR}/*.cpp"
    "${SOURCE_DIR}/*.cxx"
    "${SOURCE_DIR}/*.cc"
)

if(EXISTS ${INCLUDE_DIR})
    file(GLOB_RECURSE Vulkan-Engine_HEADERS
        "${INCLUDE_DIR}/*.h"
        "${INCLUDE_DIR}/*.hpp"
    )
endif()

#######################################
