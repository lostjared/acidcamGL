cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(acidcamGL LANGUAGES CXX C)

set(ACIDCAMGL_MAJOR_VERSION 1)
set(ACIDCAMGL_MINOR_VERSION 0)
set(ACIDCAMGL_VERSION ${ACIDCAMGL_MAJOR_VERSION}.${ACIDCAMGL_MINOR_VERSION})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "============================================")
message(STATUS "Building ${PROJECT_NAME} v${ACIDCAMGL_VERSION}")
message(STATUS "Build configuration: ${CMAKE_BUILD_TYPE}")
message(STATUS "============================================")

if(CMAKE_CXX_COMPILER_LOADED)
    message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
endif()

if(CMAKE_C_COMPILER_LOADED)
    message(STATUS "C Compiler: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND COMPILER_FLAGS "-O3")
else()
    message(STATUS "Debug Mode enabled")
    list(APPEND COMPILER_FLAGS "-g")
endif()

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# Standard RPATH for /usr/local
set(CMAKE_INSTALL_RPATH "/usr/local/lib;/usr/lib")

include(CheckIncludeFileCXX)
include(CheckIncludeFile)
include(CheckLibraryExists)

message(STATUS "")
message(STATUS "--- Checking Dependencies ---")

set(OpenCV_FOUND FALSE)

# OpenCV Search Strategy
message(STATUS "Looking for OpenCV...")
set(OpenCV_DIR_HINTS
    /usr/local/lib/cmake/opencv4
    /usr/lib/cmake/opencv4
    /usr/share/opencv4
    /usr/lib64/cmake/opencv4
)

foreach(hint ${OpenCV_DIR_HINTS})
    if(EXISTS "${hint}" AND NOT OpenCV_FOUND)
        set(OpenCV_DIR "${hint}")
        find_package(OpenCV QUIET CONFIG)
        if(OpenCV_FOUND)
            message(STATUS "Found OpenCV ${OpenCV_VERSION} at: ${OpenCV_DIR}")
            break()
        endif()
    endif()
endforeach()

if(NOT OpenCV_FOUND)
    unset(OpenCV_DIR CACHE)
    find_package(OpenCV REQUIRED)
endif()

message(STATUS "OpenCV Libraries: ${OpenCV_LIBS}")

check_include_file_cxx("opencv2/opencv.hpp" HAVE_OPENCV_HPP)
if(NOT HAVE_OPENCV_HPP)
    set(CMAKE_REQUIRED_INCLUDES ${OpenCV_INCLUDE_DIRS})
    check_include_file_cxx("opencv2/opencv.hpp" HAVE_OPENCV_HPP_WITH_PATH)
endif()

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
message(STATUS "OpenGL Found")

find_package(glfw3 REQUIRED)
message(STATUS "GLFW3 Found")

find_package(PkgConfig REQUIRED)
if(PKG_CONFIG_FOUND)
    message(STATUS "PkgConfig Found: ${PKG_CONFIG_EXECUTABLE}")
else()
    message(FATAL_ERROR "PkgConfig not found!")
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# ------------------------------------------------------------------
# LIBACIDCAM CONFIGURATION
# ------------------------------------------------------------------

# Ensure we check PkgConfig first
pkg_check_modules(acidcam REQUIRED acidcam)

if(acidcam_FOUND)
    message(STATUS "acidcam library found")
    message(STATUS "acidcam Include Dirs: ${acidcam_INCLUDE_DIRS}")
    message(STATUS "acidcam Libraries: ${acidcam_LIBRARIES}")
else()
    # Fallback: Manual search in /usr/local/lib
    message(STATUS "PkgConfig failed, trying manual search in /usr/local...")
    find_library(ACIDCAM_LIB_MANUAL NAMES acidcam PATHS /usr/local/lib /usr/lib)
    find_path(ACIDCAM_INC_MANUAL NAMES acidcam/ac.h PATHS /usr/local/include /usr/include)

    if(ACIDCAM_LIB_MANUAL AND ACIDCAM_INC_MANUAL)
        set(acidcam_LIBRARIES ${ACIDCAM_LIB_MANUAL})
        set(acidcam_INCLUDE_DIRS ${ACIDCAM_INC_MANUAL})
        message(STATUS "Found acidcam manually: ${ACIDCAM_LIB_MANUAL}")
    else()
        message(FATAL_ERROR "acidcam library not found! Checked pkg-config and /usr/local/lib.")
    endif()
endif()

set(GLM_INCLUDE_DIR "")
if(EXISTS "${CMAKE_SOURCE_DIR}/source/glm/glm.hpp")
    set(GLM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/source")
    message(STATUS "Using bundled GLM")
else()
    check_include_file_cxx("glm/glm.hpp" HAVE_SYSTEM_GLM)
    if(HAVE_SYSTEM_GLM)
        message(STATUS "Using system GLM")
    else()
        # Look in standard locations only
        foreach(glm_path "/usr/include" "/usr/local/include")
            if(EXISTS "${glm_path}/glm/glm.hpp")
                set(GLM_INCLUDE_DIR "${glm_path}")
                message(STATUS "Found GLM in: ${glm_path}")
                break()
            endif()
        endforeach()
    endif()
endif()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/source/src/glad.c")
    message(FATAL_ERROR "GLAD source file not found at ${CMAKE_SOURCE_DIR}/source/src/glad.c")
endif()

message(STATUS "Using bundled GLAD library")

check_library_exists(dl dlopen "" HAVE_LIBDL)

message(STATUS "")
message(STATUS "--- Dependency Check Complete ---")
message(STATUS "")

add_library(glad STATIC ${CMAKE_SOURCE_DIR}/source/src/glad.c)
target_include_directories(glad PRIVATE ${CMAKE_SOURCE_DIR}/source/include)

set(ACIDCAMGL_SOURCES
    ${CMAKE_SOURCE_DIR}/source/main.cpp
    ${CMAKE_SOURCE_DIR}/source/gl_shader.cpp
    ${CMAKE_SOURCE_DIR}/source/gl_window.cpp
    ${CMAKE_SOURCE_DIR}/source/keymap.cpp
    ${CMAKE_SOURCE_DIR}/source/acidcam_window.cpp
    ${CMAKE_SOURCE_DIR}/source/ipc_client.cpp
    ${CMAKE_SOURCE_DIR}/source/ffmpeg_write.cpp
    ${CMAKE_SOURCE_DIR}/source/plugin-program.cpp
    ${CMAKE_SOURCE_DIR}/source/stereo.cpp
)

# Standardized Include Directories
set(ACIDCAMGL_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/source
    ${CMAKE_SOURCE_DIR}/source/include
    ${OpenCV_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${acidcam_INCLUDE_DIRS}
    /usr/local/include
)

if(GLM_INCLUDE_DIR)
    list(APPEND ACIDCAMGL_INCLUDE_DIRS ${GLM_INCLUDE_DIR})
endif()

# Standardized Link Directories
set(ACIDCAMGL_LINK_DIRS
    ${acidcam_LIBRARY_DIRS}
    /usr/lib
    /usr/local/lib
)

add_executable(acidcamGL ${ACIDCAMGL_SOURCES})
target_include_directories(acidcamGL PRIVATE ${ACIDCAMGL_INCLUDE_DIRS})
target_link_directories(acidcamGL PRIVATE ${ACIDCAMGL_LINK_DIRS})

target_link_libraries(acidcamGL 
    PRIVATE
        Threads::Threads
        glfw
        OpenGL::GL
        glad
        ${OpenCV_LIBS}
        ${acidcam_LIBRARIES}
        ${CMAKE_DL_LIBS}
)

target_compile_options(acidcamGL PRIVATE ${COMPILER_FLAGS})
install(TARGETS acidcamGL DESTINATION bin)

message(STATUS "")
message(STATUS "============================================")
message(STATUS "Configuration Summary:")
message(STATUS "  Project: ${PROJECT_NAME} v${ACIDCAMGL_VERSION}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Acidcam Lib: ${acidcam_LIBRARIES}")
message(STATUS "============================================")
