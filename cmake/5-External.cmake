#######################################

find_package(Vulkan REQUIRED)
message(STATUS "Found Vulkan: ${Vulkan_INCLUDE_DIRS}")

find_package(PkgConfig REQUIRED)
pkg_check_modules(SHADERC REQUIRED shaderc)
message(STATUS "Found shaderc via pkg-config: ${SHADERC_LIBRARIES}")

find_package(glfw3 3.3 REQUIRED)
message(STATUS "Found GLFW3: ${glfw3_INCLUDE_DIRS}")

#######################################
