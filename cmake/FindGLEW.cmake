# Locate the GLEW library

if (WIN32)
    set(_glew_HEADERS_SEARCH_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    set(_glew_LIBRARIES_SEARCH_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}/lib
    )

    find_path(GLEW_INCLUDE_DIR
            NAMES GL/glew.h
            PATHS ${_glew_HEADERS_SEARCH_DIRS}
    )

    find_library(GLEW_LIBRARY
            NAMES GLEW glew32
            PATHS ${_glew_LIBRARIES_SEARCH_DIRS}
    )

    if (NOT GLEW_INCLUDE_DIR OR NOT GLEW_LIBRARY)
        message(FATAL_ERROR "Could not find GLEW library")
    endif()
endif ()





