########################################

add_executable(example_triangle
    Main.cpp
    TriangleApplication.cpp
)

target_link_libraries(example_triangle PRIVATE
    Lumen::Engine
)

target_include_directories(example_triangle PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

set_target_properties(example_triangle PROPERTIES
    CXX_STANDARD ${CMAKE_CXX_STANDARD}
    CXX_STANDARD_REQUIRED ON
)

########################################

apply_compiler_warnings(example_triangle)
apply_linker_optimizations(example_triangle)

########################################
