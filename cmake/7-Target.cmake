#######################################

add_executable(liblumen ${Vulkan-Engine_SOURCES})
message(STATUS "INFO: Created target 'liblumen' as a EXE.")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_include_directories(liblumen PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    message(STATUS "INFO: Added ./include to the include path for target liblumen")
endif()

if(Vulkan-Engine_HEADERS)
    target_sources(liblumen PRIVATE ${Vulkan-Engine_HEADERS})
endif()

target_link_libraries(liblumen PRIVATE vulkan pthread glfw glm)

apply_compiler_warnings(liblumen)
apply_linker_optimizations(liblumen)

#######################################
