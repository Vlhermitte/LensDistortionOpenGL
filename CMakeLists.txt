cmake_minimum_required(VERSION 3.22)
project(LensDistortionOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Package Finder Functions
function(find_package_and_message package)
    # Convert the package name to upper and lower case for flexibility (linux needs lower case and macOS needs upper case)
    string(TOUPPER ${package} PACKAGE_UPPER)
    string(TOLOWER ${package} PACKAGE_LOWER)

    find_package(PkgConfig QUIET)
    if (PKG_CONFIG_FOUND)
        pkg_check_modules(${PACKAGE_LOWER} QUIET ${PACKAGE_LOWER})
        pkg_check_modules(${PACKAGE_UPPER} QUIET ${PACKAGE_UPPER})
    endif()

    # Try finding the package using both upper and lower case
    if (NOT ${PACKAGE_LOWER}_FOUND AND NOT ${PACKAGE_UPPER}_FOUND)
        find_package(${PACKAGE_LOWER} QUIET)
        find_package(${PACKAGE_UPPER} QUIET)
    endif()

    # Check which one was found and set the correct variables
    if (${PACKAGE_LOWER}_FOUND)
        set(PACKAGE_FOUND ${PACKAGE_LOWER})
    elseif(${PACKAGE_UPPER}_FOUND)
        set(PACKAGE_FOUND ${PACKAGE_UPPER})
    else()
        message(FATAL_ERROR "${package} not found")
    endif()

    # Print the found package
    message(STATUS "Found ${PACKAGE_FOUND} in ${${PACKAGE_FOUND}_INCLUDE_DIRS}")
endfunction()

# Find packages
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
    message(STATUS "Found OpenGL in ${OPENGL_INCLUDE_DIR}")
else ()
    message(FATAL_ERROR "OpenGL not found")
endif ()

find_package_and_message(GLFW3)
find_package_and_message(GLEW)
find_package_and_message(GLM)
find_package_and_message(ASSIMP)

find_package(Python REQUIRED COMPONENTS Interpreter Development)
if (Python_FOUND)
    message(STATUS "Found Python in ${Python_INCLUDE_DIRS}")
else ()
    message(FATAL_ERROR "Python not found")
endif ()

link_directories(${CMAKE_SOURCE_DIR}/lib)
link_directories(${GLFW3_LIBRARY_DIRS} ${GLEW_LIBRARY_DIRS} ${ASSIMP_LIBRARY_DIRS})
link_directories(${Python_LIBRARY_DIRS})


# Find DLLs
if (WIN32)
    file(GLOB DLLS "${CMAKE_SOURCE_DIR}/dlls/*.dll")
    file(COPY ${DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif ()
# Include directories
include_directories(
        ${CMAKE_SOURCE_DIR}/HeaderFile
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/Engine
        ${CMAKE_SOURCE_DIR}/HeaderFile/
        ${CMAKE_SOURCE_DIR}/HeaderFile/Engine
        ${CMAKE_SOURCE_DIR}/HeaderFile/GUI
        ${CMAKE_SOURCE_DIR}/Shaders
        ${CMAKE_SOURCE_DIR}/include
        ${GLFW3_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${ASSIMP_INCLUDE_DIRS}
        ${Python_INCLUDE_DIRS}
)

file(GLOB_RECURSE SOURCES
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/include/stb/*.cpp
        ${CMAKE_SOURCE_DIR}/include/imgui/*.cpp)

file(GLOB_RECURSE HEADERS
        ${CMAKE_SOURCE_DIR}/HeaderFile/*.h
        ${CMAKE_SOURCE_DIR}/HeaderFile/*.hpp
        ${CMAKE_SOURCE_DIR}/include/stb/*.h
        ${CMAKE_SOURCE_DIR}/include/stb/*.hpp
        ${CMAKE_SOURCE_DIR}/include/imgui/*.h
        ${CMAKE_SOURCE_DIR}/include/imgui/*.hpp)


# Add executable
add_executable(${PROJECT_NAME}
        src/main.cpp
        ${SOURCES}
        ${HEADERS}
        HeaderFile/GameState.h
)


# Link libraries
if (WIN32 OR UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME}
            ${OPENGL_LIBRARIES} ${GLFW3_LIBRARIES} ${GLEW_LIBRARIES} ${ASSIMP_LIBRARIES} ${Python_LIBRARIES}
    )
elseif (APPLE)
    target_link_libraries(${PROJECT_NAME}
            "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo"
            ${GLFW3_LIBRARIES} ${GLEW_LIBRARIES} ${ASSIMP_LIBRARIES} ${Python_LIBRARIES}
    )
endif ()

# Define GL_SILENCE_DEPRECATION to silence deprecation warnings
target_compile_definitions(${PROJECT_NAME} PRIVATE GL_SILENCE_DEPRECATION)