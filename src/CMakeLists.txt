######################################## liblumen_utils.so

set(UTILS_SOURCES)

file(GLOB_RECURSE utils_cpps
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp"
)
list(APPEND UTILS_SOURCES ${utils_cpps})

add_library(lumen_utils SHARED ${UTILS_SOURCES})

set_target_properties(lumen_utils PROPERTIES
    CXX_STANDARD ${CMAKE_CXX_STANDARD}
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(lumen_utils PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_library(Lumen::Utils ALIAS lumen_utils)

apply_compiler_warnings(lumen_utils)
apply_linker_optimizations(lumen_utils)

######################################## liblumen_backend.so

set(BACKEND_SOURCES)

file(GLOB_RECURSE backend_cpps
    "${CMAKE_CURRENT_SOURCE_DIR}/vulkan_backend/*.cpp"
)
list(APPEND BACKEND_SOURCES ${backend_cpps})

add_library(lumen_backend SHARED ${BACKEND_SOURCES})

set_target_properties(lumen_backend PROPERTIES
    CXX_STANDARD ${CMAKE_CXX_STANDARD}
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(lumen_backend PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(lumen_backend PUBLIC
    Vulkan::Vulkan
    glfw
    Lumen::Utils
)

add_library(Lumen::Backend ALIAS lumen_backend)

apply_compiler_warnings(lumen_backend)
apply_linker_optimizations(lumen_backend)

######################################## liblumen.so

set(ENGINE_SOURCES)

file(GLOB_RECURSE engine_cpps
    "${CMAKE_CURRENT_SOURCE_DIR}/lumen/*.cpp"
)
list(APPEND ENGINE_SOURCES ${engine_cpps})

add_library(lumen SHARED ${ENGINE_SOURCES})

set_target_properties(lumen PROPERTIES
    CXX_STANDARD ${CMAKE_CXX_STANDARD}
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(lumen PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(lumen PUBLIC
    Lumen::Utils
    Lumen::Backend
)
add_library(Lumen::Engine ALIAS lumen)

apply_compiler_warnings(lumen)
apply_linker_optimizations(lumen)
