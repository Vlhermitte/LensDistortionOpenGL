# Locate the GLFW3 library

if (WIN32)
    set(_glfw3_HEADERS_SEARCH_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    set(_glfw3_LIB_SEARCH_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
    )

    find_path(GLFW3_INCLUDE_DIR
        NAMES GLFW/glfw3.h
        PATHS ${_glfw3_HEADERS_SEARCH_DIRS}
    )

    find_library(GLFW3_LIBRARY
        NAMES glfw3
        PATHS ${_glfw3_LIB_SEARCH_DIRS}
    )

    if (NOT GLFW3_INCLUDE_DIR OR NOT GLFW3_LIBRARY)
        message(FATAL_ERROR "Could not find GLFW3 library")
    endif()
endif ()