cmake_minimum_required(VERSION 3.27)
project(LensDistortionOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Package Finder Functions
function(find_package_and_message package)
    find_package(PkgConfig QUIET)
    if (PKG_CONFIG_FOUND)
        pkg_check_modules(${package} REQUIRED ${package})
    else ()
        find_package(${package} REQUIRED)
    endif ()
    if (${package}_FOUND)
        message(STATUS "Found ${package} in ${${package}_INCLUDE_DIRS}")
    else ()
        message(FATAL_ERROR "${package} not found")
    endif ()
endfunction()

# Find packages
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
    message(STATUS "Found OpenGL in ${OPENGL_INCLUDE_DIR}")
else ()
    message(FATAL_ERROR "OpenGL not found")
endif ()

find_package_and_message(glfw3)
find_package_and_message(glew)
find_package_and_message(glm)
find_package_and_message(assimp)

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