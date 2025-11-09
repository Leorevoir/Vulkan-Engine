#######################################

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

#######################################

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

#######################################

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#######################################

message(STATUS "INFO: CXX standard set to ${CMAKE_CXX_STANDARD}")

#######################################

if(ENABLE_DEBUG)
    add_compile_definitions(LUMEN_DEBUG)
    message(STATUS "INFO: LUMEN_DEBUG flag enabled")
endif()
