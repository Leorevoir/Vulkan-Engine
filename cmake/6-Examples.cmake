########################################

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples")
    file(GLOB EXAMPLE_DIRS LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/examples/*")

    foreach(EXAMPLE_DIR ${EXAMPLE_DIRS})
        if(IS_DIRECTORY ${EXAMPLE_DIR})
            get_filename_component(EXAMPLE_NAME ${EXAMPLE_DIR} NAME)
            set(TARGET_NAME "example_${EXAMPLE_NAME}")

            add_subdirectory(${EXAMPLE_DIR} ${CMAKE_BINARY_DIR}/examples/${EXAMPLE_NAME})
            message(STATUS "Configured example target: ${TARGET_NAME}")
        endif()
    endforeach()
endif()

########################################
