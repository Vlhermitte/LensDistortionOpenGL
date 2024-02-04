# Locate the GLM library

set(_glm_HEADERS_SEARCH_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_path(GLM_INCLUDE_DIR "glm/glm.hpp"
        PATHS ${_glm_HEADERS_SEARCH_DIRS}
)

