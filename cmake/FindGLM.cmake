# Locate the GLM library

if (WIN32)
    set(_glm_HEADERS_SEARCH_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    find_path(GLM_INCLUDE_DIR "glm/glm.hpp"
            PATHS ${_glm_HEADERS_SEARCH_DIRS}
    )
endif ()