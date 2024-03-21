# Locate the ASSIMP library

if (WIN32)
    find_package(ASSIMP REQUIRED)

    if (ASSIMP_FOUND)
        include_directories(${ASSIMP_INCLUDE_DIRS})
        target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIBRARIES})
    endif()
endif ()